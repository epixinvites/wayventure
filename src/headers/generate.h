#pragma once

#include <random>
#include "classes.h"
#include "main.h"

// For readability

Monster generate_room_monsters(std::vector<Monster> &enemy_data, const Level current, const Csr csr_pos={0,0});

Item generate_loot_from_monster_type(Enemy_Type type);

Item generate_loot_from_rarity_type(Rarity type);

Monster_Stats create_monster(Level current, Enemy_Type type);

bool return_chance(int chance);

int generate_random_number(int range_lo, int range_hi);

void generate_doors(std::vector<std::pair<int, int>> &doors, Level current);

int generate_gold(Enemy_Type type);

double rarity_value(Rarity rarity);

double rarity_bonus(Rarity rarity);

void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item *item);

Item craft_item(Rarity loot_rarity, Type loot_type);

Item generate_trade_items(Rarity loot_rarity);
