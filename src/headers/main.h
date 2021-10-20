#pragma once

#include "classes.h"
#include <libtcod.h>
#include <SDL2/SDL.h>

const auto modkeys=KMOD_CTRL | KMOD_SHIFT | KMOD_ALT;
const std::string TILESET_FILE="src/res/alphabet.png";
const tcod::TilesetPtr tileset=tcod::load_tilesheet(TILESET_FILE, {16, 16}, tcod::CHARMAP_CP437);
const std::string save_file_version="1.1"; // DO NOT MODIFY
const std::string username="Wayfarrer"; // Please don't put weird characters and keep the length under 30 characters. The program will grumble if you don't follow my instructions.

// Tweak these colors if your eyes go ouch. You can play around with the colors and rebuild the program when you're ready.
static constexpr TCOD_ColorRGB BLACK{32, 32, 32}; // def_background
static constexpr TCOD_ColorRGB FULL_BLACK{0, 0, 0};
static constexpr TCOD_ColorRGB WHITE{208, 208, 208}; // def_foreground
static constexpr TCOD_ColorRGB CYAN{0, 255, 255}; // player_char (exclusive)
static constexpr TCOD_ColorRGB LIGHT_RED{255, 50, 50}; // enemy (exclusive)
static constexpr TCOD_ColorRGB GREEN{50, 153, 50};
static constexpr TCOD_ColorRGB BLUE{0, 127, 255};
static constexpr TCOD_ColorRGB GRAY{115, 115, 115};
static constexpr TCOD_ColorRGB PURPLE{120, 81, 169};
static constexpr TCOD_ColorRGB YELLOW{255, 255, 0};
static constexpr TCOD_ColorRGB DARK_RED{196, 2, 51}; // artifact (exclusive)
static constexpr TCOD_ColorRGB LIGHT_BLUE{173, 216, 230}; // water (exclusive)
static constexpr TCOD_ColorRGB MAGENTA{255, 0, 255}; // NPC (exclusive)

// Don't touch any of these. For god's sake.
const std::string empty_line="                                                                                ";

void clear_and_draw_dialog(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialog);

int SDL_getch(tcod::ConsolePtr &main_win, tcod::ContextPtr &context);

std::pair<int, int> SDL_getch_ex(tcod::ConsolePtr &main_win, tcod::ContextPtr &context);

bool SDL_getch_y_or_n(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue);

void SDL_wclear_main_win(tcod::ConsolePtr &main_win, tcod::ContextPtr &context);

void SDL_wclear_dialog_bar(tcod::ConsolePtr &main_win, tcod::ContextPtr &context);

void SDL_wclear_stats_bar(tcod::ConsolePtr &main_win, tcod::ContextPtr &context);

std::string get_string(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue, std::string original);

unsigned int get_int(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue);

long long int get_llint(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue);

unsigned long long int get_ullint(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue);

void print_bold_with_condition(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &output, const TCOD_ColorRGB &foreground, int line, bool is_bold);

void end_program(int sig);

void end_program(int sig, const std::string &error);

void save_data(Player user, Level Current, Csr csr_pos, const std::vector<Monster> &monsters, const Npc &npc, No_Delete perm_config);
