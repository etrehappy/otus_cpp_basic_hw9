cmake_minimum_required(VERSION 3.26)

project(statistics_prj)

add_executable(statistics	
	statistics.cpp		statistics.h
	min.cpp				min.h
	max.cpp				max.h
	mean.cpp			mean.h
	std_deviation.cpp	std_deviation.h
	pct90.cpp			pct90.h
	pct95.cpp			pct95.h
)

set_target_properties(statistics PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

if (MSVC)    
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT statistics)
endif ()