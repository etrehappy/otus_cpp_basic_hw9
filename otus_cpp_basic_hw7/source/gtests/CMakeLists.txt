cmake_minimum_required(VERSION 3.26)

project(prj_test_containers)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  #URL "C:/Libraries/GoogleTest/googletest-03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(PARENT_SOURCE_DIR 
	 "${CMAKE_SOURCE_DIR}/.."
)

add_executable(containers_tests
	tests_main.cpp
	tests_main.h

	vector_tests.cpp		
	list_tests.cpp			
	extra_2_destructor.cpp

	${PARENT_SOURCE_DIR}/u_list.ipp
	${PARENT_SOURCE_DIR}/u_vector.ipp
)

target_include_directories(containers_tests PRIVATE ${PARENT_SOURCE_DIR})
target_link_libraries(containers_tests PRIVATE gtest gtest_main)
target_link_libraries(containers_tests PRIVATE gmock gmock_main)

target_compile_options(containers_tests PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX >
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

set_target_properties(containers_tests PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT containers_tests)	
endif()