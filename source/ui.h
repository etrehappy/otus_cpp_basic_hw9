#pragma once
#include <sstream>
#include "main_character.h"
/*!
* @brief It is used now only to show level and experience bar
*/
class UI
{
public:
    UI(const MainCharacter&);
    void Update();
    void Draw(sf::RenderWindow&) const;

private:
    void SetBar();
    void SetText();
    void UpdateLevelText(const unsigned int);
    void UpdateExpBar();    

    sf::RectangleShape exp_rect_, exp_bar_;
    const float bar_width_, bar_height_;
    sf::Font font_;
    const float text_pos_x_;
    sf::Text sf_text_level_;
    std::stringstream string_out_;
    const MainCharacter& main_character_;
    unsigned int prev_main_chrct_level_;
     
};
