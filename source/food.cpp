#include "food.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Food
// 
////////////////////////////////////////////////////////////////////////////////
Food::Food(std::shared_ptr<b2World> b2_world)
    : Objects{b2_world}
{
}

bool Food::IsEaten()
{
    if (physic_body_->GetUserData().pointer ==
        static_cast<uintptr_t>(TypePhysicalBody::kEaten))
        return true;
    else
        return false;

}


////////////////////////////////////////////////////////////////////////////////
// 
// class WildStrawberry
// 
////////////////////////////////////////////////////////////////////////////////
WildStrawberry::WildStrawberry(std::shared_ptr<b2World> b2_world)
    : Food(b2_world), body_radius_{18.f},
    min_pos_x_metres_{-0.5f * gset::kWindowMode.width * gset::kPxToMetres},
    max_pos_x_metres_{1.5f * gset::kWindowMode.width * gset::kPxToMetres},
    min_pos_y_metres_{-10.f * gset::kPxToMetres}
{        
    SetTexture();
    SetPhysics();
}

WildStrawberry::WildStrawberry(WildStrawberry&& rhs) noexcept
    :Food(rhs.Objects::physical_world_),
    min_pos_x_metres_{rhs.min_pos_x_metres_}, max_pos_x_metres_{rhs.max_pos_x_metres_},
    min_pos_y_metres_{rhs.min_pos_y_metres_}
{
    Objects::sprite_ = std::move(rhs.Objects::sprite_);
    body_radius_ = rhs.body_radius_;
        
    b2BodyDef wstrawberry_def;
    wstrawberry_def.type = rhs.Objects::physic_body_->GetType();
    wstrawberry_def.position = rhs.Objects::physic_body_->GetPosition();
    wstrawberry_def.angle = rhs.Objects::physic_body_->GetAngle();

    auto rhs_fixture = rhs.Objects::physic_body_->GetFixtureList();
    b2FixtureDef wstrawberry_fixture_def{};
    wstrawberry_fixture_def.shape = rhs_fixture->GetShape();
    wstrawberry_fixture_def.density = rhs_fixture->GetDensity();
    wstrawberry_fixture_def.friction = rhs_fixture->GetFriction();
    wstrawberry_fixture_def.restitution = rhs_fixture->GetRestitution();
    wstrawberry_fixture_def.filter = rhs_fixture->GetFilterData();

    Objects::physic_body_ = Objects::physical_world_->CreateBody(&wstrawberry_def);
    Objects::physic_body_->SetLinearDamping(rhs.Objects::physic_body_->GetLinearDamping());
    Objects::physic_body_->CreateFixture(&wstrawberry_fixture_def);
    Objects::physic_body_->GetUserData().pointer = rhs.Objects::physic_body_->GetUserData().pointer;

}

WildStrawberry::~WildStrawberry()
{    
    if (Objects::physic_body_ != nullptr) 
    {
        Objects::physical_world_->DestroyBody(Objects::physic_body_);
        Objects::physic_body_ = nullptr;
    }
}

void WildStrawberry::Update()
{
    Objects::sprite_.setPosition(Objects::physic_body_->GetPosition().x * gset::kMetresToPx, Objects::physic_body_->GetPosition().y * gset::kMetresToPx);

    Objects::sprite_.setRotation(Objects::physic_body_->GetAngle() * gset::kDegree);
}

void WildStrawberry::Draw(sf::RenderWindow& window)
{    
    window.draw(Objects::sprite_);
}

void WildStrawberry::Move(float current_map_x_speed)
{
    Objects::physic_body_->ApplyForceToCenter({current_map_x_speed,0}, true);
}


void WildStrawberry::SetTexture()
{
    sf::Texture& texture = Food::TextureСatalog::GetInstance().GetWildStrawberry();
    Objects::sprite_.setTexture(texture);
    Objects::sprite_.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
}

void WildStrawberry::SetPhysics()
{
    /* Step1: Setings for a body */
    b2BodyDef body_def{};
    body_def.type = b2_dynamicBody;
    std::pair<float, float> position = GeneratePosMetres();
    body_def.position.Set(position.first, position.second); 

    b2CircleShape food_shape{};
    food_shape.m_radius = body_radius_ * gset::kPxToMetres;

    b2FixtureDef fixture_def{};
    fixture_def.shape = &food_shape;
    fixture_def.density = 0.5f;
    fixture_def.friction = 1.0f;
    fixture_def.restitution = 0.1f;
    fixture_def.filter.categoryBits = static_cast<uint16>(CollisionCategory::kFood);
    fixture_def.filter.maskBits = static_cast<uint16>(CollisionCategory::kAlways);

    /* Step2: Creating the body */
    Objects::physic_body_ = Objects::physical_world_->CreateBody(&body_def);
    Objects::physic_body_->SetLinearDamping(10.f);
    Objects::physic_body_->CreateFixture(&fixture_def);
    Objects::physic_body_->GetUserData().pointer = static_cast<uintptr_t>(TypePhysicalBody::kWildStrawberry);    
}

std::pair<float, float> WildStrawberry::GeneratePosMetres()
{    
    std::random_device rd;
    std::mt19937 gen(rd());
 
    std::uniform_int_distribution<> pos_x(min_pos_x_metres_, max_pos_x_metres_);

    return std::pair<float, float> {(float)pos_x(gen), min_pos_y_metres_};
}

