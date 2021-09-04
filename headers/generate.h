#pragma once
#include <random>
#include "classes.h"
void generate_monsters(std::vector<monster> &monsters, level Current, Csr csr_pos);
Item generate_loot_from_monster_type(char type);
Item generate_loot_from_rarity_type(char type);
monster_stats create_monster(level Current, char type);
bool return_chance(int chance);
void generate_doors(std::vector<std::pair<int,int>> &doors, level Current);
int generate_gold(char type);
double rarity_value(char rarity);
