#pragma once
#include <nlohmann/json.hpp>
#include <fstream>

#include "entities.h"
#include "contact_listener.h" 
//#include "box2d/box2d.h"

class MainCharacter : public Entities
{
private:
    MainCharacter(const float x, const float y, std::shared_ptr<b2World> b2_world);
    MainCharacter(const MainCharacter&) = delete;
    MainCharacter& operator=(const MainCharacter&) = delete;

public:
    /*!
     * @brief Creating the singleton сharacter
     * @param[in] x, y — position pixel
     * @return сharacter
     */
    static MainCharacter& Create(const float x, const float y,
        std::shared_ptr<b2World> b2_world);

    ~MainCharacter() override;

    void Update() override;
    void Draw(sf::RenderWindow& window) override;
    
    /*!
     * @brief Moves the chatacter vertically/horizontally and animates it even if 
            the chatacter is standing
     * @param[in] key — A, D, W, S
     * @param[in] key_2 — W, S, Shift, Space, nothing
     */
    void Move(sf::Keyboard::Key key, 
        sf::Keyboard::Key key_2 = sf::Keyboard::Unknown);

    void Save() const;
    void LoadSave();
    void BoostExp(const unsigned int boost_size);
    const unsigned long long GetExpPoints() const;
    const double GetExpPointsToUp() const;
    const unsigned int GetLevel() const;    

private:
    struct Settings;

    void SetTexture(const float x, const float y);
    void SetPhysics();

    void CheckLevel();
    void Animate(sf::IntRect& );
    void MoveHorizontal(float& current_speed, float& opposite_speed, 
        const float speed_increase, const float max_speed);
    void MoveVertical(float& current_speed, const float speed_increase, 
        const float max_speed, const float horizontal_speed = 0);    
    void ChangeSpeed(float& current_speed, const float speed_increase, 
        const float max_speed);
     
    /* using ptr just to declaration struct outside Map1 */
    std::unique_ptr<Settings> set_chrct_;

    sf::Texture texture_;
    sf::Sprite sprite_;
    const sf::Vector2i sprite_frame_;
    const float scale_size_;
    sf::IntRect standing_rect_, to_right_rect_, to_left_rect_;   
    std::shared_ptr<b2World> physical_world_;
    b2Body* physic_body_;

    sf::Clock character_time_;      
};

struct MainCharacter::Settings
{
    Settings(float x, float y);
    const float start_pos_x_, start_pos_y_, phys_radius_;
    const b2Vec2 max_speed_metres_;
    const b2Vec2 speed_increase_;
    float speed_to_left_metres_, speed_to_right_metres_, speed_to_up_metres_;
    const float frame_time_;
    unsigned long long exp_points_; //experience
    double exp_to_up_;
    float coef_exp_to_up_;
    unsigned int level_;
};
