#ifndef __BLOOD_SWORD_HPP__
#define __BLOOD_SWORD_HPP__

// Standard Libraries
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// JSON Library for loading/saving games
#include "nlohmann/json.hpp"

// Blood Sword includes
#include "globals.hpp"

#include "abilities.hpp"
#include "assets.hpp"
#include "attributes.hpp"
#include "book.hpp"
#include "character.hpp"
#include "controls.hpp"
#include "combat.hpp"
#include "equipment.hpp"
#include "fonts.hpp"
#include "glyphs.hpp"
#include "input.hpp"
#include "monster.hpp"
#include "party.hpp"
#include "random.hpp"
#include "spell.hpp"

#include "tactical_map.hpp"
#include "engine.hpp"
#include "astar.hpp"

#endif