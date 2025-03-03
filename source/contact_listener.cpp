#include "contact_listener.h"

void WorldContactListener::BeginContact(b2Contact* contact)  
{  
    b2Body* a = contact->GetFixtureA()->GetBody();
    b2Body* b = contact->GetFixtureB()->GetBody();    
     
    if (IsOnGroundOrFrame(a->GetUserData().pointer, b->GetUserData().pointer))
    {
        SetDestroyable(a, b);
    }
    else if (IsCharacterEatFood(a->GetUserData().pointer, b->GetUserData().pointer))
    {
        SetEaten(a, b);
    }       
}

inline bool WorldContactListener::IsOnGroundOrFrame(const uintptr_t a, const uintptr_t b) const
{    
    using TPB = TypePhysicalBody;

    bool a_is_food_or_stick{a == static_cast<uintptr_t>(TPB::kWildStrawberry) ||
        (a == static_cast<uintptr_t>(TPB::kStick))};
    bool b_is_ground_or_frame{b == static_cast<uintptr_t>(TPB::kGround) ||
        b == static_cast<uintptr_t>(TPB::kOutsideFrame)};

    bool b_is_food_or_stick{b == static_cast<uintptr_t>(TPB::kWildStrawberry) ||
        (b == static_cast<uintptr_t>(TPB::kStick))};
    bool a_is_ground_or_frame{a == static_cast<uintptr_t>(TPB::kGround) ||
        a == static_cast<uintptr_t>(TPB::kOutsideFrame)};
    
    return ((a_is_food_or_stick && b_is_ground_or_frame) ||
        (b_is_food_or_stick && a_is_ground_or_frame));
}

inline bool WorldContactListener::IsCharacterEatFood(const uintptr_t a, const uintptr_t b) const
{
    using TPB = TypePhysicalBody;

    bool a_is_food{a == static_cast<uintptr_t>(TPB::kWildStrawberry)};
    bool b_is_character{a == static_cast<uintptr_t>(TPB::kMainCharacterDynamic)};

    bool a_is_character{a == static_cast<uintptr_t>(TPB::kMainCharacterDynamic)};
    bool b_is_food{b == static_cast<uintptr_t>(TPB::kWildStrawberry)};

    return ((a_is_food && b_is_character) || (a_is_character && b_is_food));
}

inline void WorldContactListener::SetDestroyable(b2Body* a, b2Body* b) const
{
    using TPB = TypePhysicalBody;

    if ((a->GetUserData().pointer == static_cast<uintptr_t>(TPB::kGround)) ||
        (a->GetUserData().pointer == static_cast<uintptr_t>(TPB::kOutsideFrame)))
    {
        b->GetUserData().pointer = static_cast<uintptr_t>(TPB::kDestroyable);       
    }
    else 
    {
        a->GetUserData().pointer = static_cast<uintptr_t>(TPB::kDestroyable);       
    }
}

inline void WorldContactListener::SetEaten(b2Body* a, b2Body* b)
{
    using TPB = TypePhysicalBody;

    if (a->GetUserData().pointer == static_cast<uintptr_t>(TPB::kMainCharacterDynamic))
    {
        b->GetUserData().pointer = static_cast<uintptr_t>(TPB::kEaten);
    }
    else
    {
        a->GetUserData().pointer = static_cast<uintptr_t>(TPB::kEaten);
    }
}