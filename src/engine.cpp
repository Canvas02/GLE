// Copyright 2022 Canvas02 <Canvas02@protonmail.com>.
// SPDX-License-Identifier: MIT

#include "engine.hpp"

#include <SDL.h>
#include <glad/gl.h>
#include <SDL_opengl.h>

#include <spdlog/spdlog.h>

static auto g_sdl_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
static u64 g_delta_time = SDL_GetTicks64();

void Engine::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		spdlog::critical("SDL: Failed to initialize");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Initialize video");
	}

	_window = SDL_CreateWindow(
		_window_title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_window_extent.x,
		_window_extent.y,
		g_sdl_window_flags
	);
  if (_window == nullptr) {
		spdlog::critical("SDL: Failed to create window");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		spdlog::trace("SDL: Created a window");
	}

	_context = SDL_GL_CreateContext(_window);
  if (_context == nullptr) {
		spdlog::critical("SDL: Failed to create opengl context");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		spdlog::trace("SDL: Created an opengl context");
	}

	
	if (SDL_GL_MakeCurrent(_window, _context) != 0) {
		spdlog::error("SDL: Failed to make context current");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Made context current");
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0) {
		spdlog::error("SDL: Failed to set SDL_GL_CONTEXT_MAJOR_VERSION to 4");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Set SDL_GL_CONTEXT_MAJOR_VERSION to 4");
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) != 0) {
		spdlog::error("SDL: Failed to set SDL_GL_CONTEXT_MINOR_VERSION to 6");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Set SDL_GL_CONTEXT_MINOR_VERSION to 6");
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
		spdlog::error("SDL: Failed to set SDL_GL_CONTEXT_PROFILE_MASK to SDL_GL_CONTEXT_PROFILE_CORE");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Set SDL_GL_CONTEXT_PROFILE_MASK to SDL_GL_CONTEXT_PROFILE_CORE");
	}

	#if defined(GLE_DEBUG_CONTEXT)
	auto context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG;
	#else
	auto context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
	#endif

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags) != 0) {
		spdlog::error("SDL: Failed to set SDL_GL_CONTEXT_FLAGS");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Set SDL_GL_CONTEXT_FLAGS");
		#if defined(GLE_DEBUG_CONTEXT)
		spdlog::trace("SDL: Configured to use debug context");
		#endif
	}

	if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) != 0) {
		spdlog::error("SDL: Failed to set SDL_GL_ACCELERATED_VISUAL to 1");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
		spdlog::trace("SDL: Set SDL_GL_ACCELERATED_VISUAL to 1");
	}

	if(SDL_GL_SetSwapInterval(static_cast<i32>(_swap_interval)) != 0) {
		spdlog::error("SDL: Failed to set swap interval");
		spdlog::error("SDL: error log:\n{}", SDL_GetError());
	} else {
	spdlog::trace(
		"SDL: Using swap interval: {}",
		_swap_interval == SwapInterval::Immediate ? "Immediate" :
		_swap_interval == SwapInterval::AdaptiveSync ? "AdaptiveSync" :
		_swap_interval == SwapInterval::VSync ? "VSync" :
		"!___UNREACHABLE___!"
	);
	}

	auto gl_version = gladLoaderLoadGL();
	if (gl_version == 0) {
		spdlog::critical("Glad: Failed to load opengl");
		exit(EXIT_FAILURE);
	} else {
		spdlog::info("Glad: Loaded opengl version {}.{}", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));
	}

	glViewport(0, 0, _window_extent.x, _window_extent.y);

	m_is_init = true;
}

void Engine::run() {
	bool quit = false;
	bool fullscreen = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_f:
						fullscreen = !fullscreen;
						if (fullscreen) {
							SDL_SetWindowFullscreen(_window, g_sdl_window_flags | SDL_WINDOW_FULLSCREEN);
						} else {
							SDL_SetWindowFullscreen(_window, g_sdl_window_flags);
						}
						break;
					
					default:
						break;
				}
			} else if (e.type == SDL_WINDOWEVENT_RESIZED) {
				i32 width, height;
				SDL_GetWindowSize(_window, &width, &height);

				glViewport(0, 0, width, height);
			}
		}

		g_delta_time = SDL_GetTicks64() - g_delta_time;
		spdlog::debug("SDL: g_delta_time = {}", g_delta_time);

		draw();
		SDL_GL_SwapWindow(_window);
	}
}

void Engine::draw() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::cleanup() {
	if (m_is_init) {
		SDL_DestroyWindow(_window);
		SDL_Quit();
	}
}