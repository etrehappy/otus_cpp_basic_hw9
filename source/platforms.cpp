#include "platforms.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Platforms
// 
////////////////////////////////////////////////////////////////////////////////
Platforms::Platforms(std::shared_ptr<b2World> b2_world)
    : Objects{b2_world}
{
}


////////////////////////////////////////////////////////////////////////////////
// 
// class Stick
// 
////////////////////////////////////////////////////////////////////////////////
Stick::Stick(std::shared_ptr<b2World> b2_world)
    :Platforms(b2_world), width_metres_{90 * gset::kPxToMetres}, 
    height_metres_{5 * gset::kPxToMetres}, 
    min_pos_x_metres_{1.1f * gset::kWindowMode.width * gset::kPxToMetres},
    max_pos_x_metres_{1.5f * gset::kWindowMode.width * gset::kPxToMetres},
    min_pos_y_metres_{0.2f * gset::kWindowMode.height * gset::kPxToMetres},
    max_pos_y_metres_{0.8f * gset::kWindowMode.height * gset::kPxToMetres}
{    
    SetTexture();
    SetPhysics();
    Objects::sprite_.setRotation(Objects::physic_body_->GetAngle() * gset::kDegree);
}

Stick::Stick(Stick&& rhs) noexcept
    :Platforms(rhs.Objects::physical_world_), width_metres_{rhs.width_metres_},
    height_metres_{rhs.height_metres_}, 
    min_pos_x_metres_{rhs.min_pos_x_metres_}, max_pos_x_metres_{rhs.max_pos_x_metres_}, 
    min_pos_y_metres_{rhs.min_pos_y_metres_}, max_pos_y_metres_{rhs.max_pos_y_metres_}
{
    Objects::sprite_ = std::move(rhs.Objects::sprite_);    

    b2BodyDef stick_def;
    stick_def.type = rhs.Objects::physic_body_->GetType();
    stick_def.position = rhs.Objects::physic_body_->GetPosition();
    stick_def.angle = rhs.Objects::physic_body_->GetAngle();
       
    auto rhs_fixture = rhs.Objects::physic_body_->GetFixtureList();
    b2FixtureDef stick_fixture_def{};
    stick_fixture_def.shape = rhs_fixture->GetShape();
    stick_fixture_def.density = rhs_fixture->GetDensity();
    stick_fixture_def.friction = rhs_fixture->GetFriction();
    stick_fixture_def.restitution = rhs_fixture->GetRestitution();
    stick_fixture_def.filter = rhs_fixture->GetFilterData();    

    physic_body_ = Objects::physical_world_->CreateBody(&stick_def);
    physic_body_->CreateFixture(&stick_fixture_def);
    physic_body_->GetUserData().pointer = rhs.Objects::physic_body_->GetUserData().pointer;
   
}

Stick::~Stick()
{
    if (Objects::physic_body_ != nullptr) 
    {
        Objects::physical_world_->DestroyBody(Objects::physic_body_);
        Objects::physic_body_ = nullptr;
    }
}

void Stick::Update()
{    
    Objects::sprite_.setPosition(Objects::physic_body_->GetPosition().x * gset::kMetresToPx, Objects::physic_body_->GetPosition().y * gset::kMetresToPx);    
    
    
}

void Stick::Draw(sf::RenderWindow& window)
{   
    window.draw(Objects::sprite_);
}

void Stick::Move(float current_map_x_speed)
{   
    physic_body_->SetLinearVelocity({current_map_x_speed,0});
}

void Stick::SetTexture()
{    
    sf::Texture& texture = Stick::TextureСatalog::GetInstance().GetStick();
    Objects::sprite_.setTexture(texture);

    const float center_x = texture.getSize().x / 2.f;
    const float center_y = 18.f;
    Objects::sprite_.setOrigin(center_x, center_y);
}

void Stick::SetPhysics()
{  
    /* Step1: Setings for a body */
    b2BodyDef stick_def{};
    stick_def.type = b2_kinematicBody;
    std::pair<float, float> position = GeneratePosMetres();
    stick_def.position.Set(position.first, position.second);
    stick_def.angle = GenerateAngle();
       
    b2PolygonShape stick_rect{};
    b2Vec2 stick_vec[4]{};
    stick_vec[0].Set(-width_metres_, -height_metres_);
    stick_vec[1].Set(-width_metres_, height_metres_);
    stick_vec[2].Set(width_metres_, height_metres_);
    stick_vec[3].Set(width_metres_, -height_metres_);     
    stick_rect.Set(stick_vec, 4);

    b2FixtureDef stick_fixture_def{};
    stick_fixture_def.shape = &stick_rect;
    stick_fixture_def.density = 0.1f;
    stick_fixture_def.friction = 0.1f;
    stick_fixture_def.restitution = 0.5f;
    stick_fixture_def.filter.categoryBits = static_cast<uint16>(CollisionCategory::kPlatforms);
    stick_fixture_def.filter.maskBits = static_cast<uint16>(CollisionCategory::kAlways);

    /* Step2: Creating the body */
    Objects::physic_body_ = Objects::physical_world_->CreateBody(&stick_def);
    Objects::physic_body_->CreateFixture(&stick_fixture_def);
    Objects::physic_body_->GetUserData().pointer = static_cast<uintptr_t>(TypePhysicalBody::kStick);
}

std::pair<float, float> Stick::GeneratePosMetres()
{    
    std::random_device rd;
    std::mt19937 gen(rd());
 
    std::uniform_int_distribution<> pos_x(min_pos_x_metres_, max_pos_x_metres_);
    std::uniform_int_distribution<> pos_y(min_pos_y_metres_, max_pos_y_metres_);

    float left = pos_x(gen);
    float right = pos_x(gen);
    float nearest_x_pos{0};
    if (left < right)
    {
        nearest_x_pos = gset::kWindowMode.width * gset::kPxToMetres - left;
    }
    else
    {
        nearest_x_pos = right;
    }  

    return std::pair<float, float> {(float)nearest_x_pos, (float)pos_y(gen)};
}

float Stick::GenerateAngle()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> degree(0, 360);

    return degree(gen) / gset::kDegree;   
}

/////////////////////////////////////////////////////////////////////////////////////
