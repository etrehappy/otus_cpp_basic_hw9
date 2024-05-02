#pragma once

#include "box2d/box2d.h"
#include "settings.h"
#include "body_type.h"

class WorldContactListener : public b2ContactListener 
{
public:
	WorldContactListener() = default;
	~WorldContactListener() = default;

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override {};

private:

	/*!
	* @brief Checkes contact food or platforms with borders.
	* @param[in] a, b — destroyable (e.g. berries) and undestroyable (e.g. ground) b2Body
	*/
	bool IsOnGroundOrFrame(const uintptr_t a, const uintptr_t b) const;

	bool IsCharacterEatFood(const uintptr_t a, const uintptr_t b) const;

	/*!
	* @brief Changes TypePhysicalBody of one of the two b2Body. 
	* @param[in] a, b — destroyable (e.g. berries) and undestroyable (e.g. ground) b2Body
	*/
	void SetDestroyable(b2Body* a, b2Body* b) const;

	/*!
	* @brief Changes TypePhysicalBody of one of the two b2Body.
	* @param[in] a, b — edible (e.g. berries) and main character b2Body	* 
	*/
	void SetEaten(b2Body* a, b2Body* b);
};

