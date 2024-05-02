#pragma once
#include "entities.h"
#include <string>
#include <random>
#include <utility>
//"box2d/box2d.h" and "body_type.h" already included in "entities.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Food
// 
////////////////////////////////////////////////////////////////////////////////
class Food : public Objects
{
public:
    Food(std::shared_ptr<b2World> b2_world);
    virtual ~Food() = default;
    
    virtual void Move(float current_map_x_speed) = 0;
    bool IsEaten();
};


////////////////////////////////////////////////////////////////////////////////
// 
// class WildStrawberry
// 
////////////////////////////////////////////////////////////////////////////////
class WildStrawberry : public Food 
{
public:
    WildStrawberry(std::shared_ptr<b2World>);
    WildStrawberry(WildStrawberry&&) noexcept;
    ~WildStrawberry();
   
    void Update() override;
    void Draw(sf::RenderWindow& window) override;
    void Move(float current_map_x_speed) override;

private:    
    void SetTexture() override;
    void SetPhysics() override;

    std::pair<float, float> GeneratePosMetres();
   
    float body_radius_;
    const float min_pos_x_metres_, max_pos_x_metres_, min_pos_y_metres_;
};
