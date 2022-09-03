// Copyright 2022 Canvas02 <Canvas02@protonmail.com>.
// SPDX-License-Identifier: MIT

#include "engine.hpp"

#include <spdlog/spdlog.h>
#include <SDL_main.h>

int main(int argc, char* argv[]) {
	spdlog::set_level(spdlog::level::trace);

	spdlog::debug("Program: Program start");
	Engine engine("GLE", 1280, 720);

	engine.init();
	engine.run();
	engine.cleanup();

	spdlog::debug("Program: Program end");
	return 0;
}