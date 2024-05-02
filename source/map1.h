#pragma once
#include <SFML/Audio.hpp>
#include <memory>
#include <list>
#include <thread>
#include <functional>

#include "settings.h"
#include "contact_listener.h" 
#include "maps.h"
#include "food.h"
#include "platforms.h"
//"main_character.h" and "box2d/box2d.h" already included in "maps.h"

class Map1 : public Maps
{
public:
    Map1();
    virtual ~Map1();

    void Update() override;
    void Draw(sf::RenderWindow&) override;    
    void Save() const override;
    void LoadSave() override;

private:
    struct Settings;
    struct Backgrounds;

    void SetMapBackground();
    
    void SetGroundPhysics();
    void SetPhysicsOutsideFrame();
    void SetPhysicsMainFrame();

    void GenerateEntities();
    const bool IsTimeElapsed(float& last_time, const float interval);
    void ProcessMovementInput();
    void MoveHorizontally(bool);
    void SlowStop();

    void UpdateFood();
    void DrawFood(sf::RenderWindow&);
    void UpdatePlatforms();
    void DrawPlatforms(sf::RenderWindow&);
        
    sf::Clock map1_time_;

    /* using ptr just to declaration structs outside Map1 */
    std::unique_ptr<Settings> set_map_; 
    std::unique_ptr<Backgrounds> backgrounds_map_;    
    
    std::shared_ptr<b2World> physical_world_;
    b2Body  *ground_body_, *outside_frame_body_, *main_frame_body_;
    WorldContactListener contact_listener_;

    std::list<WildStrawberry> wstrawberry_list_;
    std::list<Stick> stick_list_;
    std::vector<Food*> drawable_food_;
    std::vector<Platforms*> drawable_platforms_;
    MainCharacter* main_charecter_; 
    sf::Music music_;
};

struct Map1::Settings
{
    Settings();
    ~Settings() = default;

    sf::Vector2f speed_to_right_, speed_to_left_;
    sf::Vector2f* current_map_speed_;
    const float max_speed_, min_start_speed_, speed_reduction_, speed_increase_;
    const bool to_right_, to_left_;
    const float character_start_pos_x_, character_start_pos_y_;
    float b2D_time_step_;
    int32 b2D_velocity_iterations_, b2D_position_iterations_;
    float wstrawberry_interval_time_, wstrawberry_last_time_, wstrawberry_speed_coeff_,
        wstrawberry_exp_amount_;
    float stick_interval_time_, stick_last_time_, stick_speed_coeff_;
    size_t stick_max_quantity_;
    bool move_food_; 
};

/*!
*@struct Menu::Backgrounds menu.h
*@brief Each map uses [Backgrounds], but each of these structs may contain
        a different number of textures and different member functions
*/
struct Map1::Backgrounds
{
public:
    Backgrounds();
    ~Backgrounds() = default;

    void Draw(sf::RenderWindow&) const;
    void Move(const sf::Vector2f&);

    Parallax sky_, decor_, mid_decor_, foreground_, ground_;

private:
    float coeff_speed_decor_, coeff_speed_mid_decor_,
        coeff_speed_foreground_, coeff_speed_ground_;
};
