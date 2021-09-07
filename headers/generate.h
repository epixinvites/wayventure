#pragma once
#include <random>
#include "classes.h"

// For readability
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

void generate_monsters(std::vector<monster> &monsters, level Current, Csr csr_pos);
Item generate_loot_from_monster_type(char type);
Item generate_loot_from_rarity_type(char type);
monster_stats create_monster(level Current, char type);
bool return_chance(int chance);
void generate_doors(std::vector<std::pair<int,int>> &doors, level Current);
int generate_gold(char type);
double rarity_value(char rarity);
void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item &item);
