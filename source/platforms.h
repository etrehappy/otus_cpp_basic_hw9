#pragma once
#include "entities.h"
#include <random>
#include <utility>
//"box2d/box2d.h" and "body_type.h" already included in "entities.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Platforms
// 
////////////////////////////////////////////////////////////////////////////////
class Platforms : public Objects
{
public:
	Platforms(std::shared_ptr<b2World>);
	virtual ~Platforms() = default;	
	
	virtual void Move(float current_map_x_speed) = 0;	
};


////////////////////////////////////////////////////////////////////////////////
// 
// class Stick
// 
////////////////////////////////////////////////////////////////////////////////
class Stick : public Platforms
{
public:
	Stick(std::shared_ptr<b2World> b2_world);
	Stick(Stick&&) noexcept;
	virtual ~Stick();

	void Update() override;
	void Draw(sf::RenderWindow& window) override;	
	void Move(float current_map_x_speed) override;

private:
	void SetTexture() override;
	void SetPhysics() override;

	/*!
	* @brief When min_pos_x_metres_ > 1, it generates the x-position to the 
		left and right of the main window
	*/
	std::pair<float, float> GeneratePosMetres();

	/*!
	* @return Degree
	*/
	float GenerateAngle();

	const float width_metres_, height_metres_;
	const float min_pos_x_metres_, max_pos_x_metres_, min_pos_y_metres_, max_pos_y_metres_;
	
	
};