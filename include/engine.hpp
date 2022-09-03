// Copyright 2022 Canvas02 <Canvas02@protonmail.com>.
// SPDX-License-Identifier: MIT

#pragma once

#include "common/types.hpp"

#include <glm/glm.hpp>
enum class SwapInterval : i32 {
	Immediate = 0,
	VSync = 1,
	AdaptiveSync = -1 
};

class Engine {
public:
	constexpr Engine(const char* title, i32 width, i32 height)
		: _window_title(title), _window_extent(width, height) {}

	const char* _window_title;
	glm::ivec2 _window_extent;

	SwapInterval _swap_interval{ SwapInterval::AdaptiveSync };

	struct SDL_Window *_window{ nullptr };
	void* _context{ nullptr };			// SDL_GLContext is an opaqe pointer

	void init();
	void run();
	void draw();
	void cleanup();

private:
	bool m_is_init{ false };
};