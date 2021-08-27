#pragma once
#include <random>
#include "classes.h"
Item generate_loot(char type);
void generate_monsters(std::vector<monster> &monsters, level Current, Csr csr_pos);
monster_stats create_monster(level Current, char type);
bool return_chance(int chance);
void generate_doors(std::vector<std::pair<int,int>> &doors, level Current);
int generate_gold(char type);
