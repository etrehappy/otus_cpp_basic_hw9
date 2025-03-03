cmake_minimum_required(VERSION 3.26)

project(topk_prj)

	set(BOOK_1 ${CMAKE_SOURCE_DIR}/Book_1_The_Fellowship_Of_The_Ring.txt)
	set(BOOK_2 ${CMAKE_SOURCE_DIR}/Book_2_The_Return_Of_The_King.txt)
	set(BOOK_3 ${CMAKE_SOURCE_DIR}/Book_3_The_Two_Towers.txt)
	set(BOOK_4 ${CMAKE_SOURCE_DIR}/Book_4_Appendices_And_Index.txt)

	set(BOOKS ${BOOK_1} ${BOOK_2} ${BOOK_3}	${BOOK_4})

	if(MSVC)
		set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT topk)
		set(CMAKE_VS_DEBUGGER_COMMAND_ARGUMENTS "${BOOK_1} ${BOOK_2} ${BOOK_3} ${BOOK_4}")			
	endif ()

add_executable(topk 
	original_source_from_hw.cpp	original_source_from_hw.h
	main.cpp					main.h
	most_common_words.cpp		most_common_words.h	
)

add_custom_target(run
    COMMAND topk ${BOOKS}
    DEPENDS topk    
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}	
    COMMENT "##### Running topk with 4 arguments #####"
)

#target_compile_options(topk PRIVATE
#        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX >
#        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
#        )

set_target_properties(topk PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

