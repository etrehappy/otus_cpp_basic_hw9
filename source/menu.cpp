#include "menu.h"

////////////////////////////////////////////////////////////////////////////////
// 
// class Menu
// 
////////////////////////////////////////////////////////////////////////////////
Menu::Menu()
    : Maps(), backgrounds_{std::make_unique<Backgrounds>()}, 
    start_button_texture_{}, load_button_texture_{}, exit_button_texture_{},
    button_start_new_game_{}, button_load_save_{}, button_exit_{},
    start_new_game_{false}, load_save_{false}, pressed_exit_to_desktop_{false},    
    music_{}    
{   
    SetMapBackground();
    SetButton();
    gset::LoadMusic(music_, gset::kMainMenuTheme);    
    music_.play();
}

void Menu::Update(sf::RenderWindow& global_window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(global_window));

        start_new_game_ = button_start_new_game_.getGlobalBounds().contains(mouse_pos);
        load_save_ = button_load_save_.getGlobalBounds().contains(mouse_pos);
        pressed_exit_to_desktop_ = button_exit_.getGlobalBounds().contains(mouse_pos);

        if (start_new_game_ || load_save_ || pressed_exit_to_desktop_)
        {            
            map_open_ = false;
            music_.stop();
            return;
        }
    }
}

void Menu::Draw(sf::RenderWindow& window)
{
    backgrounds_->Draw(window);
    window.draw(button_start_new_game_);
    window.draw(button_load_save_);
    window.draw(button_exit_);
}

const bool Menu::IsUserWantToExit(std::shared_ptr<Maps> map) const
{
    ExitMenu exit_menu_{};

    exit_menu_.WaitingUserChoice();

    if (exit_menu_.IsSaveGame())
    {
        if (!std::filesystem::exists(gset::kSaveDirectory))
        {   
            try
            {
                std::filesystem::create_directory(gset::kSaveDirectory);                
            }
            catch (const std::filesystem::filesystem_error& ex)
            {
                std::ofstream log(gset::kLogError, std::ios::app);
                log << "\n***\nFailed to create a save folder. " << ex.what();
                log.close();
                std::terminate();
            }
        }

        map->Save();
        return true;
    }
    else if (exit_menu_.IsExitWithoutSave()) { return true; }

    return false;
}

const bool Menu::IsStartGame() const
{
    return start_new_game_;
}

const bool Menu::IsLoadSave() const
{
    return load_save_;
}

const bool Menu::IsExitToDesktop() const
{
    return pressed_exit_to_desktop_;
}

void Menu::SetMapBackground()
{
    /* Step1: Set Texture */
    sf::Texture sky, decor, mid_decor, foreground, ground;

    gset::LoadTexture(sky, gset::kMainMenuBgSky);
    gset::LoadTexture(decor, gset::kMainMenuBgDecor);
    gset::LoadTexture(mid_decor, gset::kMainMenuBgMidDecor);
    gset::LoadTexture(foreground, gset::kMainMenuBgForeground);
    gset::LoadTexture(ground, gset::kMainMenuBgGround);  

    backgrounds_->sky_.SetTexture(sky);
    backgrounds_->decor_.SetTexture(decor);
    backgrounds_->mid_decor_.SetTexture(mid_decor);
    backgrounds_->foreground_.SetTexture(foreground);
    backgrounds_->ground_.SetTexture(ground);

    /* Step2: Set position */
    backgrounds_->sky_.SetPosition();
    backgrounds_->decor_.SetPosition();
    backgrounds_->mid_decor_.SetPosition();
    backgrounds_->foreground_.SetPosition();
    backgrounds_->ground_.SetPosition();
}

void Menu::SetButton()
{   
    gset::LoadTexture(start_button_texture_, gset::kMainMenuStartButton);
    gset::LoadTexture(load_button_texture_, gset::kMainMenuLoadButton);
    gset::LoadTexture(exit_button_texture_, gset::kMainMenuExitButton);

    int buttons_number = 1;    
    const float buttons_pos_x = gset::kWindowMode.width / 2;
    const float buttons_pos_y = gset::kWindowMode.height / 3;
    const int buttons_y_offset = start_button_texture_.getSize().y + 10;
    const float button_center_x = start_button_texture_.getSize().x / 2.f;
    const float button_center_y = start_button_texture_.getSize().y / 2.f;

    /* StartGame */
    button_start_new_game_.setTexture(start_button_texture_);
    button_start_new_game_.setPosition(buttons_pos_x,
        buttons_pos_y + (buttons_y_offset * buttons_number));
    button_start_new_game_.setOrigin(button_center_x, button_center_y);

    /* LoadGame */
    if (std::filesystem::exists(gset::kSaveFilePath))
    {
        ++buttons_number;

        button_load_save_.setTexture(load_button_texture_);
        button_load_save_.setPosition(buttons_pos_x,
            buttons_pos_y + (buttons_y_offset * buttons_number));
        button_load_save_.setOrigin(button_center_x, button_center_y);       
    }
    
    /* Exit */
    ++buttons_number;

    button_exit_.setTexture(exit_button_texture_);
    button_exit_.setPosition(buttons_pos_x,
        buttons_pos_y + (buttons_y_offset * buttons_number));
    button_exit_.setOrigin(button_center_x, button_center_y);
}




////////////////////////////////////////////////////////////////////////////////
// 
// struct Menu::ExitMenu
// 
////////////////////////////////////////////////////////////////////////////////
Menu::ExitMenu::ExitMenu()
    :window_exit_{sf::VideoMode{258, 240}, "",
        sf::Style::None, sf::ContextSettings(0, 0, 4)}, 
    bg_color_{sf::Color{142,98,71}}, event_{}, save_game_{false},
    button_save_texture_{}, button_exit_texture_{},
    exit_without_save_{false}, button_save_game_{}, button_exit_without_save_{}
{
    window_exit_.setFramerateLimit(gset::kFrameRateLimit);
    ExitMenu::SetButton();
}

void Menu::ExitMenu::WaitingUserChoice()
{
    while (window_exit_.isOpen())
    {
        while (window_exit_.pollEvent(event_))
        {           
            if (event_.type == sf::Event::Closed ||
                (event_.type == sf::Event::KeyPressed &&
                    event_.key.code == sf::Keyboard::Escape))
            {
                window_exit_.close();
                break;
            }
        }

        ExitMenu::Update();
        ExitMenu::Draw();
    }
}

const bool Menu::ExitMenu::IsSaveGame() const
{
    return save_game_;
}

const bool Menu::ExitMenu::IsExitWithoutSave() const
{
    return exit_without_save_;
}

void Menu::ExitMenu::SetButton()
{
    gset::LoadTexture(button_save_texture_, gset::kExMenuSaveButton);
    gset::LoadTexture(button_exit_texture_, gset::kExMenuExitButton);

    const float pos_x = window_exit_.getSize().x / 2.f;
    const float start_pos_y = button_save_texture_.getSize().y / 2.f;
    const float button_center_x = button_save_texture_.getSize().x / 2.f;
    const float button_center_y = start_pos_y;

    /* SaveGame */
    button_save_game_.setTexture(button_save_texture_);
    button_save_game_.setPosition(pos_x, start_pos_y);
    button_save_game_.setOrigin(button_center_x, button_center_y);

    /* Exit */
    button_exit_without_save_.setTexture(button_exit_texture_);
    button_exit_without_save_.setPosition(pos_x, start_pos_y * 3);
    button_exit_without_save_.setOrigin(button_center_x,button_center_y);
}

void Menu::ExitMenu::Update()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window_exit_));

        save_game_ = button_save_game_.getGlobalBounds().contains(mouse_pos);
        exit_without_save_ = button_exit_without_save_.getGlobalBounds().contains(mouse_pos);

        if (save_game_ || exit_without_save_)
        {
            window_exit_.close();
            return;
        }
    }
}

void Menu::ExitMenu::Draw()
{    
    window_exit_.clear(bg_color_);
    window_exit_.draw(button_save_game_);
    window_exit_.draw(button_exit_without_save_);
    window_exit_.display();
}




////////////////////////////////////////////////////////////////////////////////
// 
// struct Menu::Backgrounds
// 
////////////////////////////////////////////////////////////////////////////////
Menu::Backgrounds::Backgrounds()
    :sky_{}, decor_{}, mid_decor_{}, foreground_{}, ground_{}
{
}

void Menu::Backgrounds::Draw(sf::RenderWindow& window) const
{
    sky_.Draw(window);
    decor_.Draw(window);
    mid_decor_.Draw(window);
    foreground_.Draw(window);
    ground_.Draw(window);
}



