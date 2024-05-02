#include "map1.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Map1
// 
////////////////////////////////////////////////////////////////////////////////
Map1::Map1()
    :Maps(), map1_time_{},
    set_map_(std::make_unique<Settings>()),
    backgrounds_map_{std::make_unique<Backgrounds>()},    
    physical_world_{std::make_shared<b2World>(gset::kGravity)},
    ground_body_{nullptr}, outside_frame_body_{nullptr}, main_frame_body_{nullptr},
    contact_listener_{},
    wstrawberry_list_{}, stick_list_{}, drawable_food_{}, drawable_platforms_{}
{
    main_charecter_ = &(MainCharacter::Create(set_map_->character_start_pos_x_,
        set_map_->character_start_pos_y_, physical_world_));

    Maps::ui_ = std::make_unique<UI>(*main_charecter_);

    SetMapBackground();

    SetGroundPhysics();
    SetPhysicsOutsideFrame();
    SetPhysicsMainFrame();
    physical_world_->SetContactListener(&contact_listener_);       

    gset::LoadMusic(music_, gset::kMap1Theme);
    music_.play();
    music_.setLoop(true);
}

Map1::~Map1()
{
    physical_world_->DestroyBody(ground_body_);
    physical_world_->DestroyBody(outside_frame_body_);
    physical_world_->DestroyBody(main_frame_body_);
    ground_body_ = nullptr;
    outside_frame_body_ = nullptr;
    main_frame_body_ = nullptr;

    music_.stop();
}

void Map1::Update()
{
    physical_world_->Step(set_map_->b2D_time_step_,
        set_map_->b2D_velocity_iterations_,
        set_map_->b2D_position_iterations_);

    GenerateEntities();
    ProcessMovementInput();

    std::thread t_character([this]()
        {
            main_charecter_->Update();
            Maps::ui_->Update();
        });

    std::thread t_food([this]()
        { UpdateFood(); });

    std::thread t_platforms([this]()
        { UpdatePlatforms(); });

    t_character.join();
    t_food.join();
    t_platforms.join();
    
    Maps::ui_->Update();
}

void Map1::Draw(sf::RenderWindow& window)
{
    backgrounds_map_->Draw(window);
    main_charecter_->Draw(window);
    if (!drawable_food_.empty()) { DrawFood(window); }
    if (!drawable_platforms_.empty()) { DrawPlatforms(window); }
    Maps::ui_->Draw(window);
}

void Map1::Save() const
{
    main_charecter_->Save();    
}

void Map1::LoadSave()
{
    main_charecter_->LoadSave();
}


void Map1::SetMapBackground()
{
    /* Step1: Set Texture */

    sf::Texture sky, decor, mid_decor, foreground, ground;  

    gset::LoadTexture(sky, gset::kMap1BgSky);
    gset::LoadTexture(decor, gset::kMap1BgDecor);
    gset::LoadTexture(mid_decor, gset::kMap1BgMidDecor);
    gset::LoadTexture(foreground, gset::kMap1BgForeground);
    gset::LoadTexture(ground, gset::kMap1BgGround);

    backgrounds_map_->sky_.SetTexture(sky);
    backgrounds_map_->decor_.SetTexture(decor);
    backgrounds_map_->mid_decor_.SetTexture(mid_decor);
    backgrounds_map_->foreground_.SetTexture(foreground);
    backgrounds_map_->ground_.SetTexture(ground);
   
    /* Step2: Set position */

    backgrounds_map_->sky_.SetPosition();
    backgrounds_map_->decor_.SetPosition();
    backgrounds_map_->mid_decor_.SetPosition();
    backgrounds_map_->foreground_.SetPosition();
    backgrounds_map_->ground_.SetPosition();
}

void Map1::SetGroundPhysics()
{
    float half_window_width_meters = (gset::kWindowMode.width / 2) * gset::kPxToMetres;
    float half_ground_height_meters = (gset::kWindowMode.height / 18) * gset::kPxToMetres;

    /* Step1: Setings for a body */

    b2BodyDef ground_def{};
    ground_def.type = b2_staticBody;
    ground_def.position.Set(half_window_width_meters,
        (half_window_width_meters + half_ground_height_meters));    

    b2PolygonShape ground_rect{};
    b2Vec2 ground_vec[4]{};
    ground_vec[0].Set(-half_window_width_meters, -half_ground_height_meters);
    ground_vec[1].Set(-half_window_width_meters, half_ground_height_meters);
    ground_vec[2].Set(half_window_width_meters, half_ground_height_meters);
    ground_vec[3].Set(half_window_width_meters, -half_ground_height_meters);
    ground_rect.Set(ground_vec, 4);

    b2FixtureDef ground_fixture_def{};
    ground_fixture_def.shape = &ground_rect;
    ground_fixture_def.density = 2.0f;
    ground_fixture_def.friction = 0.7f;
    ground_fixture_def.filter.categoryBits = static_cast<uint16>(CollisionCategory::kMainFrame);   
    ground_fixture_def.filter.maskBits = static_cast<uint16>(CollisionCategory::kAlways);

    /* Step2: Creating the body */

    ground_body_ = physical_world_->CreateBody(&ground_def);
    ground_body_->CreateFixture(&ground_fixture_def);
    ground_body_->GetUserData().pointer = static_cast<uintptr_t>(TypePhysicalBody::kGround);
}

void Map1::SetPhysicsOutsideFrame()
{
    float window_width_metres = gset::kWindowMode.width  * gset::kPxToMetres;
    float window_height_metres = gset::kWindowMode.height * gset::kPxToMetres;
    
    /* Step1: Setings for a body */

    b2BodyDef out_frame_def{};
    out_frame_def.type = b2_dynamicBody; //need b2_dynamicBody for contactlistener
    out_frame_def.gravityScale = 0;
    out_frame_def.position.Set(-window_width_metres, 0);   

    b2ChainShape out_frame_chain{};
    b2Vec2 out_frame_vec[4]{}; //Shape is a rectangle without a top line  
    out_frame_vec[0].Set(0, 0);
    out_frame_vec[1].Set(0, window_height_metres);
    out_frame_vec[2].Set(window_width_metres * 3, window_height_metres);
    out_frame_vec[3].Set(window_width_metres * 3, 0);
    out_frame_chain.CreateChain(out_frame_vec, 4, out_frame_vec[0], out_frame_vec[3]);    

    b2FixtureDef out_frame_fixture_def;
    out_frame_fixture_def.shape = &out_frame_chain;
    out_frame_fixture_def.isSensor = true;
    out_frame_fixture_def.filter.categoryBits = static_cast<uint16>(CollisionCategory::kOutFrame);
    out_frame_fixture_def.filter.maskBits = static_cast<uint16>(CollisionCategory::kAlways);

    /* Step2: Creating the body */

    outside_frame_body_ = physical_world_->CreateBody(&out_frame_def);
    outside_frame_body_->CreateFixture(&out_frame_fixture_def);
    outside_frame_body_->GetUserData().pointer = static_cast<uintptr_t>(TypePhysicalBody::kOutsideFrame);
}

void Map1::SetPhysicsMainFrame()
{
    float window_width_metres = 0.8 * gset::kWindowMode.width * gset::kPxToMetres;
    float window_height_metres = gset::kWindowMode.height * gset::kPxToMetres;    

    /* Step1: Setings for a body */
    b2BodyDef main_frame_def{};
    main_frame_def.type = b2_staticBody;    
    main_frame_def.position.Set(0.1 * gset::kWindowMode.width * gset::kPxToMetres, 0);

    b2ChainShape main_frame_chain{};
    b2Vec2 main_frame_vec[4]{};
    main_frame_vec[0].Set(0, 0);
    main_frame_vec[1].Set(0, window_height_metres);
    main_frame_vec[2].Set(window_width_metres, window_height_metres);
    main_frame_vec[3].Set(window_width_metres, 0);
    main_frame_chain.CreateLoop(main_frame_vec, 4);
    

    b2FixtureDef frame_fixture_def{};
    frame_fixture_def.shape = &main_frame_chain;
    frame_fixture_def.filter.categoryBits = static_cast<uint16>(CollisionCategory::kFood);
    frame_fixture_def.filter.maskBits = static_cast<uint16>(CollisionCategory::kMainCharacter);   
   

    /* Step2: Creating the body */
    main_frame_body_ = physical_world_->CreateBody(&main_frame_def);    
    main_frame_body_->CreateFixture(&frame_fixture_def);
    main_frame_body_->GetUserData().pointer = static_cast<uintptr_t>(TypePhysicalBody::kMainFrameBody);
}


void Map1::GenerateEntities()
{
    if (IsTimeElapsed(set_map_->wstrawberry_last_time_, 
        set_map_->wstrawberry_interval_time_))
    {
        wstrawberry_list_.emplace_back(WildStrawberry{physical_world_});
    }

    if (IsTimeElapsed(set_map_->stick_last_time_, set_map_->stick_interval_time_) &&
        stick_list_.size() < set_map_->stick_max_quantity_)
    {
        stick_list_.emplace_back(Stick{physical_world_});
    }
}

const bool Map1::IsTimeElapsed(float& last_time, const float interval)
{
    float current_time = map1_time_.getElapsedTime().asSeconds();

    if (current_time - last_time >= interval)
    {
        last_time = current_time;
        return true;
    }

    return false;
}

void Map1::ProcessMovementInput()
{
    using kb = sf::Keyboard;

    if (kb::isKeyPressed(kb::A) && kb::isKeyPressed(kb::W))
    {
        MoveHorizontally(set_map_->to_right_);
        main_charecter_->Move(kb::A, kb::W);
        set_map_->move_food_ = true;
    }
    else if (kb::isKeyPressed(kb::D) && kb::isKeyPressed(kb::W))
    {
        MoveHorizontally(set_map_->to_left_);
        main_charecter_->Move(kb::D, kb::W);
        set_map_->move_food_ = true;
    }

    else if (kb::isKeyPressed(kb::A) && kb::isKeyPressed(kb::S))
    {
        MoveHorizontally(set_map_->to_right_);
        main_charecter_->Move(kb::A, kb::S);
    }

    else if (kb::isKeyPressed(kb::D) && kb::isKeyPressed(kb::S))
    {
        MoveHorizontally(set_map_->to_left_);
        main_charecter_->Move(kb::D, kb::S);
    }
    else if (kb::isKeyPressed(kb::A))
    {
        MoveHorizontally(set_map_->to_right_);
        main_charecter_->Move(kb::A);
        set_map_->move_food_ = true;
    }
    else if (kb::isKeyPressed(kb::D))
    {
        MoveHorizontally(set_map_->to_left_);
        main_charecter_->Move(kb::D);
        set_map_->move_food_ = true;
    }
    else if (kb::isKeyPressed(kb::W))
    {
        main_charecter_->Move(kb::W);
        set_map_->move_food_ = false;
    }
    else if (kb::isKeyPressed(kb::S))
    {
        main_charecter_->Move(kb::S);
        set_map_->move_food_ = false;
    }
    else
    {
        main_charecter_->Move(kb::Unknown); // just animation
        SlowStop();
        set_map_->move_food_ = false;
    }
}

void Map1::MoveHorizontally(bool move_right)
{
    /* Step1: preparation */
    sf::Vector2f* opposite_speed{nullptr};
    float speed_reduction{0.f}, speed_increase{0.f};

    if (move_right)
    {
        opposite_speed = &set_map_->speed_to_left_;
        speed_reduction = -set_map_->speed_reduction_;
        set_map_->current_map_speed_ = &set_map_->speed_to_right_;
        speed_increase = set_map_->speed_increase_;
    }
    else
    {
        opposite_speed = &set_map_->speed_to_right_;
        speed_reduction = set_map_->speed_reduction_;
        set_map_->current_map_speed_ = &set_map_->speed_to_left_;
        speed_increase = -set_map_->speed_increase_;
    }

    /* Step2: move */
    if (std::abs(opposite_speed->x) > set_map_->min_start_speed_)
    {        
        opposite_speed->x -= speed_reduction;
        backgrounds_map_->Move(*opposite_speed);
    }
    else
    {
        if (std::abs(set_map_->current_map_speed_->x) < set_map_->max_speed_)
        {
            set_map_->current_map_speed_->x += speed_increase;
        }

        backgrounds_map_->Move(*set_map_->current_map_speed_);
    }
}

void Map1::SlowStop()
{
    if (set_map_->speed_to_left_.x < -set_map_->min_start_speed_ ||
        set_map_->speed_to_right_.x > set_map_->min_start_speed_)
    {
        float higher_speed = std::max<float>(std::abs(set_map_->speed_to_left_.x),
            set_map_->speed_to_right_.x);

        if (set_map_->speed_to_right_.x == higher_speed)
        {
            backgrounds_map_->Move(set_map_->speed_to_right_);
            set_map_->speed_to_right_.x -= set_map_->speed_reduction_;
            set_map_->speed_to_left_.x = -set_map_->min_start_speed_;
        }
        else
        {
            backgrounds_map_->Move(set_map_->speed_to_left_);
            set_map_->speed_to_left_.x += set_map_->speed_reduction_;
            set_map_->speed_to_right_.x = set_map_->min_start_speed_;
        }
    }
}


void Map1::UpdateFood()
{
    std::list<WildStrawberry>::iterator erasable_berry{wstrawberry_list_.end()};

    for (auto& it = wstrawberry_list_.begin(); it != wstrawberry_list_.end(); it++)
    {
        if (it->IsDestroyable())
        {
            erasable_berry = it;
        }
        else if (it->IsEaten())
        {
            erasable_berry = it;
            main_charecter_->BoostExp(set_map_->wstrawberry_exp_amount_);
        }            
        else
        {
            if (set_map_->move_food_) 
            { 
                it->Move(set_map_->current_map_speed_->x * set_map_->wstrawberry_speed_coeff_);
            }

            it->Update();
            drawable_food_.push_back(&(*it));            
        }
    }

    if (erasable_berry != wstrawberry_list_.end())
    {
        wstrawberry_list_.erase(erasable_berry);
    }
}

void Map1::DrawFood(sf::RenderWindow& window)
{
    for (auto it = drawable_food_.begin(); it != drawable_food_.end(); it++)
    {  
        (*it)->Draw(window);
    }

    drawable_food_.clear();    
}

void Map1::UpdatePlatforms()
{
    std::list<Stick>::iterator erasable_stick{stick_list_.end()};

    for (auto& it = stick_list_.begin(); it != stick_list_.end(); it++)
    {
        it->Move(set_map_->current_map_speed_->x / set_map_->stick_speed_coeff_);

        if (it->IsDestroyable())
        {
            erasable_stick = it;
        }
        else
        {
            it->Update();
            drawable_platforms_.push_back(&(*it));            
        }        
    }

    if (erasable_stick != stick_list_.end())
    {
        stick_list_.erase(erasable_stick);
    }
}

void Map1::DrawPlatforms(sf::RenderWindow& window)
{
    for (auto it = drawable_platforms_.begin(); it != drawable_platforms_.end(); it++)
    {
        (*it)->Draw(window);
    }

    drawable_platforms_.clear();
}




////////////////////////////////////////////////////////////////////////////////
// 
// struct Map1::Settings
// 
////////////////////////////////////////////////////////////////////////////////
Map1::Settings::Settings()
    :speed_to_right_{0.f,0.f}, speed_to_left_{0.f,0.f}, 
    current_map_speed_{&speed_to_right_},
    max_speed_{3.f}, min_start_speed_{0.5f},
    speed_reduction_{max_speed_ / 40.f}, speed_increase_{max_speed_ / 250.f},
    to_right_{1.f}, to_left_{0.f},
    character_start_pos_x_{400.f}, character_start_pos_y_{300.f},
    b2D_time_step_{1.f / 60.f}, 
    b2D_velocity_iterations_{8}, b2D_position_iterations_{3},
    wstrawberry_interval_time_{5.f}, wstrawberry_last_time_{0.f}, 
    wstrawberry_speed_coeff_{0.1f}, wstrawberry_exp_amount_{1},
    stick_interval_time_{3.f}, stick_last_time_{0.f}, stick_speed_coeff_{2.f},
    stick_max_quantity_{30}, move_food_{false}
{
}




////////////////////////////////////////////////////////////////////////////////
// 
// struct Map1::Backgrounds
// 
////////////////////////////////////////////////////////////////////////////////
Map1::Backgrounds::Backgrounds()
    :sky_{}, decor_{}, mid_decor_{}, foreground_{}, ground_{},
    coeff_speed_decor_{0.5}, coeff_speed_mid_decor_{1},
    coeff_speed_foreground_{2}, coeff_speed_ground_{6}
{
}

void Map1::Backgrounds::Draw(sf::RenderWindow& window) const
{
    sky_.Draw(window);
    decor_.Draw(window);
    mid_decor_.Draw(window);
    foreground_.Draw(window);
    ground_.Draw(window);
}

void Map1::Backgrounds::Move(const sf::Vector2f& speed_map)
{
    decor_.Move(speed_map.x * coeff_speed_decor_, speed_map.y);
    mid_decor_.Move(speed_map.x * coeff_speed_mid_decor_, speed_map.y);
    foreground_.Move(speed_map.x * coeff_speed_foreground_, speed_map.y);
    ground_.Move(speed_map.x * coeff_speed_ground_, speed_map.y);

}


