#include "ui.h"

UI::UI(const MainCharacter& character)
    : exp_rect_{}, exp_bar_{}, bar_width_{200.f}, bar_height_{20.f}, font_{}, 
    text_pos_x_{15.f}, sf_text_level_{}, string_out_{},
    main_character_{character}, prev_main_chrct_level_{main_character_.GetLevel()}    
{       
    SetBar();
    SetText();    
}

void UI::Update()
{
    if (main_character_.GetLevel() > prev_main_chrct_level_)
    {
        UpdateLevelText(main_character_.GetLevel());
        prev_main_chrct_level_ = main_character_.GetLevel();
    }

    UpdateExpBar();
}

void UI::Draw(sf::RenderWindow& window) const
{
    window.draw(sf_text_level_); 
    window.draw(exp_rect_);
    window.draw(exp_bar_);
}

void UI::SetBar()
{
    /*Set bar's outline*/
    const sf::Vector2f exp_rect_size{bar_width_, bar_height_};
    exp_rect_.setSize(exp_rect_size);
    exp_rect_.setFillColor(sf::Color::Transparent);
    exp_rect_.setOutlineThickness(1.f);
    exp_rect_.setOutlineColor(sf::Color::Yellow);
    exp_rect_.setPosition(bar_width_ / 2, bar_height_);

    /*Set bar*/
    exp_bar_.setFillColor(sf::Color::Yellow);
    exp_bar_.setPosition(bar_width_ / 2, bar_height_);
}

void UI::SetText()
{    
    std::filesystem::path font_path{};

#ifdef _WIN32       
    font_path = gset::kWinFont;
#else
    font_path = gset::kFont;
#endif

    gset::LoadFonts(font_, font_path);

    sf_text_level_.setFont(font_);
    sf_text_level_.setOutlineThickness(1.f);
    sf_text_level_.setOutlineColor(sf::Color::Black);
    sf_text_level_.setCharacterSize(28);
    sf_text_level_.setPosition(text_pos_x_, bar_height_ / 2);

    UpdateLevelText(main_character_.GetLevel());
}

void UI::UpdateLevelText(const unsigned int level)
{
    string_out_.str("");
    string_out_ << "Lv. " << level;
    sf_text_level_.setString(string_out_.str());
}

void UI::UpdateExpBar()
{
    float new_x_size_bar = bar_width_ * 
        (main_character_.GetExpPoints() / main_character_.GetExpPointsToUp());

    exp_bar_.setSize(sf::Vector2f{new_x_size_bar, bar_height_});
}

