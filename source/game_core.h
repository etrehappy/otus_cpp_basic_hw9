#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <filesystem>
#include <fstream>
#include "settings.h"
#include "menu.h"
//#include "map1.h" already included in the "menu.h"

class GameCore
{
public:
    GameCore();
    ~GameCore() = default;

    /*!
    * @brief This is a first window in the game
    */
    void MainMenu();
  
private:

    /*!
    * @brief This function used only [GameCore::MainMenu()]
    * @see MainMenu()
    */
    void ProcessEvents();

    /*!
    * @brief This function can be used different maps
    */
    void ProcessEvents(std::shared_ptr<Maps>);

    void Update(std::shared_ptr<Maps>);

    void Draw(std::shared_ptr<Maps>);

    /*!
    * @brief This function doesn't save game, only close window
    */
    void ExitToDesktop();

    void Run(std::shared_ptr<Maps>);       

    sf::RenderWindow window_;    
    sf::Event event_;
    std::shared_ptr<Menu> menu_;
};
