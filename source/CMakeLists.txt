cmake_minimum_required(VERSION 3.26)

project(game_prj)

cmake_policy(SET CMP0135 NEW) # for json

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
if (SFML_FOUND)    
    message(STATUS "The SFML installed in the system will be used")    
else ()
    message(STATUS "SFML not found. Download from github.com")
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
endif ()


find_package(box2d REQUIRED)
if(box2d_FOUND)
    message(STATUS "The Box2D installed in the system will be used")     
else()
    message(STATUS "The Box2D not found. Download from github.com") 
    include(FetchContent)      
    FetchContent_Declare(
      Box2D
      GIT_REPOSITORY https://github.com/erincatto/Box2D.git
      GIT_TAG        v2.4.1 
    )
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Build the Box2D unit tests" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Build the Box2D Testbed" FORCE)
    FetchContent_MakeAvailable(Box2D)
endif()


include(FetchContent)
FetchContent_Declare(json 
	URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)				
FetchContent_MakeAvailable(json)


set(main_files main.cpp)
set(game_core 
    game_core.h             game_core.cpp
    contact_listener.h      contact_listener.cpp    
    settings.h              settings.cpp
    body_type.h
)
set(entities 
    entities.h          entities.cpp 
    main_character.h    main_character.cpp 
    food.h              food.cpp 
    platforms.h         platforms.cpp 
)
set(maps 
    maps.h  maps.cpp 
    map1.h  map1.cpp    
    menu.h  menu.cpp 
    ui.h    ui.cpp 
)

add_executable(game 
    ${main_files}
    ${game_core}
    ${entities}
    ${maps}
)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/../game_files
    $<TARGET_FILE_DIR:game>/game_files
    COMMENT "Copying game_files to executable directory"
)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
    source_group("Main" FILES ${main_files})
    source_group("GameCore" FILES ${game_core})
    source_group("Entities" FILES ${entities})
    source_group("Maps" FILES ${maps})
endif() 

target_link_libraries(game PRIVATE sfml-graphics)
target_link_libraries(game PRIVATE sfml-audio)
target_link_libraries(game PRIVATE nlohmann_json::nlohmann_json)

if(box2d_FOUND)
    target_link_libraries(game PRIVATE box2d::box2d)   
else()
    target_link_libraries(game PRIVATE box2d)
endif()

target_compile_features(game PRIVATE cxx_std_17)