#include "game_core.h"
#include <SFML/Graphics.hpp>
#include <windows.h>

int main()
{
    #ifdef _WIN32     
     ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    #endif

    GameCore game{};
    game.MainMenu();
    
   return 0;
}