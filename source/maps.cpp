#include "maps.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Maps
// 
////////////////////////////////////////////////////////////////////////////////
Maps::Maps()
    :map_open_{false}, ui_{nullptr}
{
}

const bool Maps::IsOpen() const
{
    return map_open_;
}

void Maps::Open()
{
    map_open_ = true;
}




////////////////////////////////////////////////////////////////////////////////
// 
// class Maps
// 
////////////////////////////////////////////////////////////////////////////////
Maps::Parallax::Parallax()
    : bg1_{}, bg2_{}, texture_{}, texture_size_{0}, overlay_{2}
{
}

void Maps::Parallax::SetTexture(sf::Texture& t)
{
    texture_ = std::move(t);
    bg1_.setTexture(texture_);
    bg2_.setTexture(texture_);

    texture_size_ = texture_.getSize().x;
}

void Maps::Parallax::SetPosition()
{
     bg1_.setPosition(0, 0);
     bg2_.setPosition(texture_.getSize().x - overlay_, 0);
}

void Maps::Parallax::Move(const float x, const float y)
{
    bg1_.move(x, y);
    bg2_.move(x, y);    

    SwapBackround();
}

void Maps::Parallax::SwapBackround()
{
    
    const float bg1_pos = bg1_.getPosition().x;
    const float bg2_pos = bg2_.getPosition().x;
    const bool bg1_off_screen_to_right = (bg1_pos + texture_size_) <= 0;
    const bool bg2_off_screen_to_right = (bg2_pos + texture_size_) <= 0;
    const bool bg1_off_screen_to_left = bg1_pos >= texture_size_;
    const bool bg2_off_screen_to_left = bg2_pos >= texture_size_;    
  
    if (bg1_off_screen_to_right)
    {
        bg1_.setPosition(bg2_pos + texture_size_ - overlay_, 0);
    }

    if (bg2_off_screen_to_right)
    {
        bg2_.setPosition(bg1_pos + texture_size_ - overlay_, 0);
    }     
    
    if (bg1_off_screen_to_left)
    {
        bg1_.setPosition(bg2_pos - texture_size_ + overlay_, 0);
    }

    if (bg2_off_screen_to_left)
    {
        bg2_.setPosition(bg1_pos - texture_size_ + overlay_, 0);
    }    
}

void Maps::Parallax::Draw(sf::RenderWindow& window) const
{
    window.draw(bg1_);              
    window.draw(bg2_);
}
