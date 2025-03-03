cmake_minimum_required (VERSION 3.8)

project(Guess_number_prj)

add_executable(guess_the_number
	main.cpp
	main.h
)

set_target_properties(guess_the_number PROPERTIES
	CXX_STANDART 20
	CXX_STANDART_REQUIRED ON
)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT guess_the_number)
endif()
