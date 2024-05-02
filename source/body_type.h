#pragma once
#include <cstdint>

/*! 
* @brief This enum is used to check contacts between different bodies
*/
enum class TypePhysicalBody : uintptr_t
{
    kNone = 0,

    /*characters*/
    kMainCharacterDynamic,

    /*borders*/
    kGround,
    kOutsideFrame,
    kMainFrameBody,

    /*food*/
    kWildStrawberry,

    /*platforms*/
    kStick,

    /*condition*/
    kEaten,
    kDestroyable   
};

/*!
* @brief This enum is used to set categoryBits and maskBits for b2FixtureDef
*/
enum class CollisionCategory : uint16 
{
    /*characters*/
    kMainCharacter = 0x1,

    /*borders*/
    kMainFrame,
    kOutFrame,

    /*food*/   
    kFood,

    /*platforms*/
    kPlatforms,
    
    kAlways = 0xFFFF
};
