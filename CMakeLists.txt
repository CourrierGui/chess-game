cmake_minimum_required(VERSION 3.1)

if(${CMAKE_VERSION} VERSION_LESS 3.17)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.17)
endif()

project(chess VERSION 1.0
	DESCRIPTION "Simple 2D chess game"
	LANGUAGES CXX
)

add_library(target-flags INTERFACE)
target_compile_options(target-flags
	INTERFACE
	-Wall -Wextra -Wpedantic
	)
target_compile_features(target-flags INTERFACE cxx_std_20)


find_package(pangolin REQUIRED)

add_library(irrKlanglib SHARED IMPORTED)
set_target_properties(irrKlanglib PROPERTIES
	IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/irrKlang/irrKlang-64bit-1.6.0/bin/linux-gcc-64/libIrrKlang.so"
)

add_library(game-utils SHARED
	src/game.cpp
	src/board.cpp
	src/piece.cpp
)
target_include_directories(game-utils
	PUBLIC
		include
		extern/irrKlang/irrKlang-64bit-1.6.0/include
)
target_link_libraries(game-utils
	PRIVATE
		target-flags
	PUBLIC
		pangolin::pangolin
		pangolin::pgl-gui
		irrKlanglib
)

add_executable(chess apps/chess.cpp)
target_include_directories(chess PUBLIC include)
target_link_libraries(chess
	PUBLIC game-utils
	PRIVATE target-flags
)
