#pragma once
#include <random>
#include "classes.h"

// For readability

void generate_monsters(std::vector<monster> &monsters, level Current, Csr csr_pos);
Item generate_loot_from_monster_type(char type);
Item generate_loot_from_rarity_type(char type);
monster_stats create_monster(level Current, char type);
bool return_chance(int chance);
void generate_doors(std::vector<std::pair<int,int>> &doors, level Current);
int generate_gold(char type);
double rarity_value(char rarity);
void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item &item);
Item craft_item(char loot_rarity, char loot_type);
