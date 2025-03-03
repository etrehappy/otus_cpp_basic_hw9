#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>

#include "map1.h"
//#include "maps.h" already included in "map1.h"

/*!
*@brief This is a first window (main menu) in the game.
*@todo You can't come back main menu once the game starts. But [Menu::ExitMenu] 
*   is created,  when user pressed Esc. This means you should develop a way to
*   come back to main menu or separate [Menu::ExitMenu].
*/
class Menu : public Maps
{
public:
    Menu();
    ~Menu() = default;
        
    /*!
    * @brief Сhecking three buttons in the main menu
    */
    void Update(sf::RenderWindow&);
    
    /*!
    * @return true — if [Update(sf::RenderWindow&)] changed start_new_game_
    */
    const bool IsStartGame() const;

    /*!
    * @return true — if [Update(sf::RenderWindow&)] changed load_save_
    */
    const bool IsLoadSave() const;

    /*!
    * @return true — if [Update(sf::RenderWindow&)] changed pressed_exit_to_desktop_
    */
    const bool IsExitToDesktop() const;

    /*!
    * @brief Uses ExitMenu. 
    *   Throwns an exception and terminates if a save folder can't be created.
    * @return true — for save and exit or just exit
    */
    const bool IsUserWantToExit(std::shared_ptr<Maps> map) const;

    /*!
    * @brief Draws main menu
    */
    void Draw(sf::RenderWindow& window) override;

private:
    void Update() override {/*unused*/};
    void Save() const override {/*unused*/};
    void LoadSave() override {/*unused*/};

    struct Backgrounds;
    struct ExitMenu;

    void SetMapBackground();
    void SetButton(); 

    /* using ptr just to declaration struct outside Menu */
    std::unique_ptr<Backgrounds> backgrounds_;

    sf::Texture start_button_texture_, load_button_texture_, exit_button_texture_;
    sf::Sprite button_start_new_game_, button_load_save_, button_exit_;
    bool start_new_game_, load_save_, pressed_exit_to_desktop_;
    sf::Music music_;    
};

/*!
*@brief [ExitMenu] can be opened everywhere except [Menu] (main menu).
*/
struct Menu::ExitMenu
{
public:
    ExitMenu();
    ~ExitMenu() = default;

    void WaitingUserChoice();
    const bool IsSaveGame() const;
    const bool IsExitWithoutSave() const;    

private:
    void SetButton();    
    void Update();
    void Draw();

    sf::RenderWindow window_exit_;
    sf::Color bg_color_;
    sf::Event event_;

    bool save_game_;
    bool exit_without_save_;

    sf::Texture button_save_texture_, button_exit_texture_;
    sf::Sprite button_save_game_, button_exit_without_save_;  
};

/*! 
*@brief Each map uses [Backgrounds], but each of these structs may contain 
*       a different number of [Parallax]-members and different member functions
*/
struct Menu::Backgrounds
{
    Backgrounds();
    ~Backgrounds() = default;

    void Draw(sf::RenderWindow&) const;

    Parallax sky_, decor_, mid_decor_, foreground_, ground_;
};