cmake_minimum_required(VERSION 3.26)

project(containers_prj)

add_executable(containers 
	main.cpp			main.h
	u_vector.ipp		u_vector.h
	u_list.ipp			u_list.h
	u_forward_list.cpp	u_forward_list.h
	
	my_exception.h
)

target_compile_options(containers PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX >
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

set_target_properties(containers PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT containers)
endif ()

