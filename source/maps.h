#pragma once
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"

#include "settings.h"
#include "main_character.h"
#include "ui.h"

class Maps
{
public:
    Maps();
    virtual ~Maps() = default;

    virtual void Update() = 0;
    virtual void Draw(sf::RenderWindow&)  = 0;
    virtual void Save() const = 0;
    virtual void LoadSave() = 0;

    const bool IsOpen() const;
    void Open();

protected:
    class Parallax;

    bool map_open_;

private:
    friend class Map1;

    std::unique_ptr<UI> ui_;    
};

/*!
* @brief This class is required to create parallax
*/
class Maps::Parallax
{
public:
    Parallax();
    void SetTexture(sf::Texture& t);
    void SetPosition();
    void Move(const float x, const float y);    
    void Draw(sf::RenderWindow& window) const;

private:
    void SwapBackround();

    sf::Sprite bg1_, bg2_;
    sf::Texture texture_;

    int texture_size_;
    const int overlay_;
};

