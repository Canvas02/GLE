# Copyright 2022 Canvas02 <Canvas02@protonmail.com>.
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)
project(GLE CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# option(GLE_DEBUG "Enable Debug Options in GLE" ON)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
find_package(tinyobjloader CONFIG REQUIRED)

set(SDL_SSE2 ON)
set(SDL_OPENGL ON)
add_subdirectory("vendor/SDL2")

add_subdirectory("vendor/glad2")

# set(TRACY_ENABLE ON)
# set(TRACY_ONLY_LOCALHOST ON)
# set(TRACY_ON_DEMAND ON)
# add_subdirectory("vendor/tracy")

set(GLE_FILES_SRC
	"src/main.cpp"
	"src/engine.cpp"
	"include/engine.hpp"
	"src/shader.cpp"
	"include/shader.hpp"

	"include/common/types.hpp"
)

set(GLE_FILES_RES_SHADERS
)
 
add_executable(Engine ${GLE_FILES_SRC})
target_include_directories(Engine PRIVATE "include")

# if(GLE_DEBUG)
target_compile_definitions(Engine PRIVATE GLE_DEBUG_CONTEXT=1)
# endif(GLE_DEBUG)

target_include_directories(Engine PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(Engine PRIVATE
	${OPENGL_LIBRARIES}

	SDL2::SDL2
	SDL2::SDL2main
	glad2

	glm::glm
	spdlog::spdlog
	tinyobjloader::tinyobjloader
)