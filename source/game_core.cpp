#include "game_core.h"

GameCore::GameCore()
    : window_{gset::kWindowMode, "The adventure of the bat",
            sf::Style::Titlebar | sf::Style::Close, 
            sf::ContextSettings(0, 0, 4, 1, 0)},
    event_{}, menu_{std::make_shared<Menu>()}
{    
    window_.setFramerateLimit(gset::kFrameRateLimit);   
}

void GameCore::MainMenu()
{
    bool start_new_game{false}, load_save{false}, exit_to_desktop{false};

    menu_->Open();

    while (window_.isOpen() && menu_->IsOpen())
    {
        ProcessEvents();

        menu_->Update(window_);

        start_new_game = menu_->IsStartGame();
        load_save = menu_->IsLoadSave();
        exit_to_desktop = menu_->IsExitToDesktop();

        Draw(menu_);
    }

    if (start_new_game)
    {   
        std::shared_ptr<Map1> map1 = std::make_shared<Map1>();
        map1->Open();

        Run(map1);
        return;
    }

    if (load_save)
    {
        namespace fs = std::filesystem;

        gset::CheckPath(gset::kSaveFilePath);

        std::shared_ptr<Map1> map1 = std::make_shared<Map1>();
        map1->LoadSave();
        map1->Open();

        Run(map1);
        return;
    }

    if (exit_to_desktop)
    {
        ExitToDesktop();
        return;
    }
}

void GameCore::ProcessEvents()
{
    while (window_.pollEvent(event_))
    {
        if (event_.type == sf::Event::Closed ||
            (event_.type == sf::Event::KeyPressed &&
                event_.key.code == sf::Keyboard::Escape))
        {
            if (menu_->IsOpen()) { ExitToDesktop(); }            
        }
    }
}

void GameCore::ProcessEvents(std::shared_ptr<Maps> map)
{
    while (window_.pollEvent(event_))
    {
        if (event_.type == sf::Event::Closed ||
           (event_.type == sf::Event::KeyPressed &&
            event_.key.code == sf::Keyboard::Escape))
        {
            if (menu_->IsUserWantToExit(map)) { ExitToDesktop(); }
        }
    }
}

void GameCore::Update(std::shared_ptr<Maps> map)
{
    if (map->IsOpen()) { map->Update(); }
}

void GameCore::Draw(std::shared_ptr<Maps> map)
{
    window_.clear();

    if (map->IsOpen()) { map->Draw(window_); }

    window_.display();
}


inline void GameCore::ExitToDesktop()
{
    window_.close();
    return;
}


inline void GameCore::Run(std::shared_ptr<Maps> map)
{   
    while (window_.isOpen())
    {        
        ProcessEvents(map);
        Update(map);
        Draw(map);
    }    
}
