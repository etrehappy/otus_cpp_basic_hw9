#include "main_character.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Map1
// 
////////////////////////////////////////////////////////////////////////////////
MainCharacter::MainCharacter(const float pos_x, const float pos_y, std::shared_ptr<b2World> b2_world)
    : set_chrct_(std::make_unique<Settings>(pos_x, pos_y)),
    texture_{}, sprite_{}, sprite_frame_{281, 156}, scale_size_{0.5f},
    standing_rect_{0, 0, sprite_frame_.x, sprite_frame_.y},
    to_right_rect_{0, 1 * sprite_frame_.y, sprite_frame_.x, sprite_frame_.y},
    to_left_rect_{0, 2 * sprite_frame_.y, sprite_frame_.x, sprite_frame_.y},
    physical_world_{b2_world}, physic_body_{}, character_time_{}
{
    SetTexture(pos_x, pos_y);
    SetPhysics();
}

MainCharacter& MainCharacter::Create(const float x, const float y, std::shared_ptr<b2World> b2_world)
{
    static MainCharacter сharacter{x, y, b2_world};
    return сharacter;
}

MainCharacter::~MainCharacter()
{
    physical_world_->DestroyBody(physic_body_);
    physic_body_ = nullptr;
}

void MainCharacter::Update()
{ 
    sprite_.setPosition(physic_body_->GetPosition().x * gset::kMetresToPx,
        physic_body_->GetPosition().y * gset::kMetresToPx);
   
    CheckLevel();
}

void MainCharacter::Draw(sf::RenderWindow& window)
{  
    window.draw(sprite_);
}

void MainCharacter::Move(sf::Keyboard::Key key, sf::Keyboard::Key key_2)
{    
    if (character_time_.getElapsedTime().asSeconds() < set_chrct_->frame_time_)
        return;    

    using kb = sf::Keyboard;
   
    switch (key)
    {
    case kb::Unknown:
        Animate(standing_rect_);
        break;

    case kb::A:
        Animate(to_left_rect_);
        
        if (key_2 == kb::W) //pressed A and W 
        {            
            MoveVertical(set_chrct_->speed_to_up_metres_,
                -set_chrct_->speed_increase_.y,
                set_chrct_->max_speed_metres_.y,
                set_chrct_->speed_to_left_metres_);           
        }
        
        MoveHorizontal(set_chrct_->speed_to_left_metres_,
            set_chrct_->speed_to_right_metres_,
            -set_chrct_->speed_increase_.x,
            set_chrct_->max_speed_metres_.x);        
        break;

    case kb::D:
        Animate(to_right_rect_);        
        if (key_2 == kb::W) //pressed D and W 
        { 
            MoveVertical(set_chrct_->speed_to_up_metres_,
                -set_chrct_->speed_increase_.y,
                set_chrct_->max_speed_metres_.y,
                set_chrct_->speed_to_right_metres_);           
        }

        MoveHorizontal(set_chrct_->speed_to_right_metres_,
            set_chrct_->speed_to_left_metres_,
            set_chrct_->speed_increase_.x,
            set_chrct_->max_speed_metres_.x);
        break;

    case kb::S:
        Animate(standing_rect_);
        break;

    case kb::W:
        Animate(standing_rect_);
        MoveVertical(set_chrct_->speed_to_up_metres_,
            -set_chrct_->speed_increase_.y,
            set_chrct_->max_speed_metres_.y);
        break;

    default:
        break;
    }  

    character_time_.restart();    
}

void MainCharacter::Save() const
{   
    nlohmann::json j;
    j[gset::kMainCharacterLvlKey] = GetLevel();
    j[gset::kMainCharacterExpKey] = GetExpPoints();
    j[gset::kMainCharacterExpToUpKey] = GetExpPointsToUp();

    std::ofstream file{gset::kSaveFilePath, std::ios::out};
    file << j.dump(4);
    file.close();
}

void MainCharacter::LoadSave()
{
    std::ifstream file{gset::kSaveFilePath, std::ios::in};
    nlohmann::json j;
    file >> j;
    file.close();

    set_chrct_->exp_points_ = j[gset::kMainCharacterExpKey];
    set_chrct_->exp_to_up_ = j[gset::kMainCharacterExpToUpKey];
    set_chrct_->level_ = j[gset::kMainCharacterLvlKey];
}

void MainCharacter::BoostExp(const unsigned int boost_size)
{
    set_chrct_->exp_points_ += boost_size;
}

const unsigned long long MainCharacter::GetExpPoints() const
{
    return set_chrct_->exp_points_;
}

const double MainCharacter::GetExpPointsToUp() const
{
    return set_chrct_->exp_to_up_;
}

const unsigned int MainCharacter::GetLevel() const
{
    return set_chrct_->level_;
}


void MainCharacter::SetTexture(const float x, const float y)
{    
    gset::LoadTexture(texture_, gset::kMainCharacterTexture);

    sprite_.setTexture(texture_);
    sprite_.scale(scale_size_, scale_size_);
    sprite_.setPosition(x, y);
    sprite_.setOrigin(sprite_frame_.x / 2.f, sprite_frame_.y / 2.f);
    sprite_.setTextureRect(standing_rect_);
}

void MainCharacter::SetPhysics()
{
    /* Step1: Setings for a body */
    b2BodyDef body_def{};
    body_def.type = b2_dynamicBody;
    body_def.position.Set(set_chrct_->start_pos_x_ * gset::kPxToMetres,
        set_chrct_->start_pos_y_ * gset::kPxToMetres);
    body_def.fixedRotation = true;

    b2CircleShape character_shape{};
    character_shape.m_radius = set_chrct_->phys_radius_ * gset::kPxToMetres;

    b2FixtureDef fixture_def{};
    fixture_def.shape = &character_shape;
    fixture_def.density = 1.1f;
    fixture_def.friction = 0.1f;
    fixture_def.restitution = 0.1f;
    fixture_def.filter.categoryBits = static_cast<uint16>(CollisionCategory::kMainCharacter);    
    fixture_def.filter.maskBits = static_cast<uint16>(CollisionCategory::kAlways);

    /* Step2: Creating the body */
    physic_body_ = physical_world_->CreateBody(&body_def);
    physic_body_->SetLinearDamping(5.1f);
    physic_body_->CreateFixture(&fixture_def);
    physic_body_->GetUserData().pointer = static_cast<uintptr_t>(TypePhysicalBody::kMainCharacterDynamic);
    
}


inline void MainCharacter::CheckLevel()
{
    if (set_chrct_->exp_points_ >= set_chrct_->exp_to_up_)
    {
        ++set_chrct_->level_;
        set_chrct_->exp_to_up_ = set_chrct_->exp_points_ * set_chrct_->coef_exp_to_up_;
        set_chrct_->exp_points_ = 0;
    }
}

inline void MainCharacter::Animate(sf::IntRect& animate_rect)
{
    animate_rect.left += sprite_frame_.x;
    if (animate_rect.left >= texture_.getSize().x) // if end of texture_
    {
        animate_rect.left = 0; // start again
    }

    sprite_.setTextureRect(animate_rect);
}

inline void MainCharacter::MoveHorizontal(float& current_speed, float& opposite_speed, const float speed_increase, const float max_speed)
{   
    opposite_speed = 0.f;

    if (set_chrct_->speed_to_up_metres_ < -1) // i.e. character is still moving upwards
    {
        set_chrct_->speed_to_up_metres_ -= set_chrct_->speed_to_up_metres_/10;
    }
    else
    {
        set_chrct_->speed_to_up_metres_ = 0;
    }

    ChangeSpeed(current_speed, speed_increase, max_speed);
    physic_body_->ApplyForceToCenter(b2Vec2{current_speed, 0}, true);
}

inline void MainCharacter::MoveVertical(float& current_speed, const float speed_increase, const float max_speed, const float horizontal_speed)
{   
    ChangeSpeed(current_speed, speed_increase, max_speed);
    physic_body_->ApplyForceToCenter(b2Vec2{horizontal_speed, current_speed}, true);
}

inline void MainCharacter::ChangeSpeed(float& current_speed,const float speed_increase, const float max_speed)
{    
    if (std::abs(current_speed) < max_speed)
    {
        current_speed += speed_increase;
    }
}


////////////////////////////////////////////////////////////////////////////////
// 
// struct Map1::Settings
// 
////////////////////////////////////////////////////////////////////////////////
MainCharacter::Settings::Settings(float x, float y)
    :start_pos_x_{x}, start_pos_y_{y}, phys_radius_{18.f},
    max_speed_metres_{400.f * gset::kPxToMetres, 1500.f * gset::kPxToMetres},
    speed_increase_{max_speed_metres_.x / 20, max_speed_metres_.y / 5},
    speed_to_left_metres_{0.f}, speed_to_right_metres_{0.f},
    speed_to_up_metres_{0.f},
    frame_time_{0.08f},
    exp_points_{0}, exp_to_up_{3.}, coef_exp_to_up_{1.5f},
    level_{1}
{
}