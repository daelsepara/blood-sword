#ifndef __BLOOD_SWORD_HPP__
#define __BLOOD_SWORD_HPP__

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "controls.hpp"
#include "constants.hpp"
#include "input.hpp"
#include "engine.hpp"
#include "tactical_map.hpp"

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title);

#endif