#include "entities.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Objects
// 
////////////////////////////////////////////////////////////////////////////////
Objects::Objects(std::shared_ptr<b2World> b2_world)
    : sprite_{}, physical_world_(b2_world), physic_body_{nullptr}
{
}

b2Body* Objects::GetBody()
{
    return physic_body_;
}

bool Objects::IsDestroyable()
{
    if (physic_body_->GetUserData().pointer ==
        static_cast<uintptr_t>(TypePhysicalBody::kDestroyable))
        return true;
    else
        return false;
}




////////////////////////////////////////////////////////////////////////////////
// 
// struct Objects::TextureСatalog
// 
////////////////////////////////////////////////////////////////////////////////
Objects::TextureСatalog::TextureСatalog()
{
    gset::LoadTexture(wild_strawberry_t_, gset::kWildStrawberryTexture);
    gset::LoadTexture(stick_t_, gset::kStickTexture);
}

Objects::TextureСatalog& Objects::TextureСatalog::GetInstance()
{
    //Singleton
    static TextureСatalog instance{};
    return instance;
}

sf::Texture& Objects::TextureСatalog::GetWildStrawberry()
{
    return wild_strawberry_t_;
}

sf::Texture& Objects::TextureСatalog::GetStick()
{
    return stick_t_;
}

