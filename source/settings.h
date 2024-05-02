#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <filesystem>
#include <fstream>
#include <exception>
#include "box2d/b2_world.h"

namespace gset //game settings 
{
/// @brief Required to scale pixels to meters
static const float kPxToMetres{0.01f};

/// @brief Required to scale meters to pixels
static const float kMetresToPx{100.f};

/// @brief Required to scale radians to degrees
static const float kDegree{57.29577f};

/// @brief Required to set world in a map1.h
static const b2Vec2 kGravity{0.f, 9.8f};

/// @brief Required to set windows, to set GroundPhysic in the map1, to generate
///     position of entities 
static const sf::VideoMode kWindowMode{1280, 720};

/// @brief Required to set frame_rate in the game_core and menu
static const int kFrameRateLimit{65};


/* --------------------- Required to save ---------------------------------- */
static const std::filesystem::path kLogError{"./log_error.txt"};
static const std::filesystem::path kSaveDirectory{"./Save"};
static const std::filesystem::path kSaveFilePath{"./Save/save.json"};
static const std::string kMainCharacterLvlKey{"main_character_level"};
static const std::string kMainCharacterExpKey{"experience_points"};
static const std::string kMainCharacterExpToUpKey{"need_experience_to_up"};

/* ------------------- Required to load fonts ------------------------------- */
static const std::filesystem::path kWinFont{"C:\\Windows\\Fonts\\times.ttf"};
static const std::filesystem::path kFont{"./game_files/times.ttf"};

/* ------------------- Required to load music ------------------------------- */
static const std::filesystem::path kMainMenuTheme{"./game_files/audio/peritune-spook4(chosic.com).mp3"};
static const std::filesystem::path kMap1Theme{"./game_files/audio/FoamRubber-320bit(chosic.com).mp3"};

/* ----------------- Required to load textures ------------------------------ */
static const std::filesystem::path kExMenuSaveButton{"./game_files//Images/exit_menu_save_button.png"};
static const std::filesystem::path kExMenuExitButton{"./game_files/Images/exit_menu_exit_button.png"};

static const std::filesystem::path kMainMenuStartButton{"./game_files/Images/start_button.png"};
static const std::filesystem::path kMainMenuLoadButton{"./game_files/Images/load_button.png"};
static const std::filesystem::path kMainMenuExitButton{"./game_files/Images/exit_button.png"};

static const std::filesystem::path kMainMenuBgSky{"./game_files/Images/main_menu_bg/Sky.png"};
static const std::filesystem::path kMainMenuBgDecor{"./game_files/Images/main_menu_bg/BG_Decor.png"};
static const std::filesystem::path kMainMenuBgMidDecor{"./game_files/Images/main_menu_bg/Middle_Decor.png"};
static const std::filesystem::path kMainMenuBgForeground{"./game_files/Images/main_menu_bg/Foreground.png"};
static const std::filesystem::path kMainMenuBgGround{"./game_files/Images/main_menu_bg/Ground.png"};

static const std::filesystem::path kMap1BgSky{"./game_files/Images/map1_bg/Sky.png"};
static const std::filesystem::path kMap1BgDecor{"./game_files/Images/map1_bg/BG_Decor.png"};
static const std::filesystem::path kMap1BgMidDecor{"./game_files/Images/map1_bg/Middle_Decor.png"};
static const std::filesystem::path kMap1BgForeground{"./game_files/Images/map1_bg/Foreground.png"};
static const std::filesystem::path kMap1BgGround{"./game_files/Images/map1_bg/Ground.png"};

static const std::filesystem::path kMainCharacterTexture{"./game_files/Images/bat.png"};

static const std::filesystem::path kWildStrawberryTexture{"./game_files/Images/wild_strawberry.png"};

static const std::filesystem::path kStickTexture{"./game_files/Images/stick.png"};

/*!
* @brief Overwrites log-file, throwns exception, not terminates
*/
void CheckPath(const std::filesystem::path&);

/*!
* @brief Adds to log-file, throwns exception, not terminates
*/
void LoadFonts(sf::Font&, const std::filesystem::path&);

/*!
* @brief Adds to log-file, throwns exception, not terminates
*/
void LoadMusic(sf::Music&, const std::filesystem::path&);

/*!
* @brief Adds to log-file, throwns exception, (!) terminates
* @todo All textures are loaded only when they are started being used.
*       This is wrong for the game.
*/
void LoadTexture(sf::Texture&, const std::filesystem::path&);

} // namespace gset



