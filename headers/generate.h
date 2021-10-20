#pragma once

#include <random>
#include "classes.h"
#include "main.h"

// For readability

void generate_monsters(std::vector<Monster> &monsters, Level Current, Csr csr_pos);

Item generate_loot_from_monster_type(Dungeon type);

Item generate_loot_from_rarity_type(Rarity type);

Monster_Stats create_monster(Level Current, Dungeon type);

bool return_chance(int chance);

int generate_random_number(int range_lo, int range_hi);

void generate_doors(std::vector<std::pair<int, int>> &doors, Level Current);

int generate_gold(Dungeon type);

double rarity_value(Rarity rarity);

double rarity_bonus(Rarity rarity);

void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item *item);

Item craft_item(Rarity loot_rarity, Type loot_type);

Item generate_trade_items(Rarity loot_rarity);
