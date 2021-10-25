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

#include <random>
#include "classes.h"
#include "main.h"

// For readability

void generate_room_monsters(std::vector<Monster> &enemy_data, const Level current, const Csr csr_pos);

Item generate_loot_from_monster_type(Enemy_Type type);

Item generate_loot_from_rarity_type(Rarity type);

Monster_Stats create_monster(Level current, Enemy_Type type);

bool return_chance(int chance);

int generate_random_number(int range_lo, int range_hi);

void generate_doors(std::vector<DoorData> &door_data, const Level current);

void generate_stairs(std::vector<StaircaseData> &staircase_data, const Level current);

int generate_gold(Enemy_Type type);

double rarity_value(Rarity rarity);

double rarity_bonus(Rarity rarity);

void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item *item);

Item craft_item(Rarity loot_rarity, Type loot_type);

Item generate_trade_items(Rarity loot_rarity);
