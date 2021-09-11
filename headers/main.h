#pragma once
#include <libtcod.h>
#include <SDL2/SDL.h>
const auto modkeys = KMOD_CTRL | KMOD_SHIFT | KMOD_ALT;

static constexpr TCOD_ColorRGB BLACK{10,10,10};
static constexpr TCOD_ColorRGB CYAN{0,255,255};
static constexpr TCOD_ColorRGB WHITE{255,255,255};

constexpr char RARITY_COMMON = 'c';
constexpr char RARITY_UNCOMMON = 'u';
constexpr char RARITY_RARE = 'r';
constexpr char RARITY_EPIC = 'e';
constexpr char RARITY_LEGENDARY = 'l';
constexpr char RARITY_ARTIFACT = 'a';
constexpr char TYPE_HELMET = 'h';
constexpr char TYPE_CHESTPLATE = 'c';
constexpr char TYPE_GREAVES = 'g';
constexpr char TYPE_BOOTS = 'b';
constexpr char TYPE_SHIELD = 's';
constexpr char TYPE_WEAPON = 'w';
constexpr char DUNGEON_ENEMY = 'e';
constexpr char DUNGEON_ROOM_BOSS = 'b';
constexpr char DUNGEON_LEVEL_BOSS = 'x';
constexpr char DUNGEON_FINAL_BOSS = 'f';
