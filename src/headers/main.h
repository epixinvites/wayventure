// Wayventure, a complex old-school dungeon adventure.
// Copyright (C) 2021 Zhi Ping Ooi
//
// This file is part of Wayventure.
//
// Wayventure is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "classes.h"
#include <libtcod.h>
#include <SDL2/SDL.h>

const auto modkeys=KMOD_CTRL | KMOD_SHIFT | KMOD_ALT;
const std::string TILESET_FILE="src/res/alphabet.png";
static tcod::Tileset tileset=tcod::load_tilesheet(TILESET_FILE, {16, 16}, tcod::CHARMAP_CP437);
const std::string save_file_version="0.2"; // DO NOT MODIFY
const std::string username="Wayfarrer"; // Please don't put weird characters and keep the length under 30 characters. The program will grumble if you don't follow my instructions.

// Tweak these colors if your eyes go ouch. You can play around with the colors and rebuild the program when you're ready.
static constexpr tcod::ColorRGB BLACK{32, 32, 32}; // def_background
static constexpr tcod::ColorRGB FULL_BLACK{0, 0, 0};
static constexpr tcod::ColorRGB WHITE{208, 208, 208}; // def_foreground
static constexpr tcod::ColorRGB CYAN{0, 255, 255}; // player_char (exclusive)
static constexpr tcod::ColorRGB LIGHT_RED{255, 50, 50}; // enemy (exclusive)
static constexpr tcod::ColorRGB GREEN{50, 153, 50};
static constexpr tcod::ColorRGB BLUE{0, 127, 255};
static constexpr tcod::ColorRGB GRAY{115, 115, 115};
static constexpr tcod::ColorRGB PURPLE{120, 81, 169};
static constexpr tcod::ColorRGB YELLOW{255, 255, 0};
static constexpr tcod::ColorRGB DARK_RED{196, 2, 51}; // artifact (exclusive)
static constexpr tcod::ColorRGB LIGHT_BLUE{173, 216, 230}; // water (exclusive)
static constexpr tcod::ColorRGB MAGENTA{255, 0, 255}; // NPC (exclusive)

// Don't touch any of these. For god's sake.
const std::string empty_line="                                                                                ";

const int DUNGEON_LEVEL_MAX=5;
const int DUNGEON_X_MAX=5;
const int DUNGEON_Y_MAX=5;

void clear_and_draw_dialog(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialog);

int SDL_getch(tcod::Console &main_win, tcod::ContextPtr &context);

std::pair<int, int> SDL_getch_ex(tcod::Console &main_win, tcod::ContextPtr &context);

bool SDL_getch_y_or_n(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue);

void SDL_wclear_main_win(tcod::Console &main_win, tcod::ContextPtr &context);

void SDL_wclear_dialog_bar(tcod::Console &main_win, tcod::ContextPtr &context);

void SDL_wclear_stats_bar(tcod::Console &main_win, tcod::ContextPtr &context);

std::string get_string(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue, std::string original);

unsigned int get_int(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue);

long long int get_llint(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue);

unsigned long long int get_ullint(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue);

void print_bold_with_condition(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &output, const TCOD_ColorRGB &foreground, int line, bool is_bold);

void print_bold_with_condition_ex(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &output, const TCOD_ColorRGB &foreground, const TCOD_ColorRGB &background, int x, int y, TCOD_alignment_t alignment, bool is_bold);

void end_program(int sig);

void end_program(int sig, const std::string &error);

void save_data(const Dungeon &dungeon_data, Player &user, const No_Delete &perm_config);
