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

#include <map>
#include "classes.h"
#include "main.h"

struct Sort_Ascending{
    std::unordered_map<Rarity, int> priority{{Rarity::ARTIFACT,  0},
                                             {Rarity::LEGENDARY, 1},
                                             {Rarity::EPIC,      2},
                                             {Rarity::RARE,      3},
                                             {Rarity::UNCOMMON,  4},
                                             {Rarity::COMMON,    5}};

    bool operator()(const Item *i, const Item *j){
        int priority_i=priority[i->rarity];
        int priority_j=priority[j->rarity];
        return priority_i<priority_j;
    }

    bool operator()(const std::pair<Item, bool> &i, const std::pair<Item, bool> &j){
        int priority_i=priority[i.first.rarity];
        int priority_j=priority[j.first.rarity];
        return priority_i<priority_j;
    }
} const sort_ascending;

struct Sort_Descending{
    std::unordered_map<Rarity, int> priority{{Rarity::ARTIFACT,  0},
                                             {Rarity::LEGENDARY, 1},
                                             {Rarity::EPIC,      2},
                                             {Rarity::RARE,      3},
                                             {Rarity::UNCOMMON,  4},
                                             {Rarity::COMMON,    5}};

    bool operator()(const Item *i, const Item *j){
        int priority_i=priority[i->rarity];
        int priority_j=priority[j->rarity];
        return priority_i>priority_j;
    }

    bool operator()(const std::pair<Item, bool> &i, const std::pair<Item, bool> &j){
        int priority_i=priority[i.first.rarity];
        int priority_j=priority[j.first.rarity];
        return priority_i>priority_j;
    }
} const sort_descending;

static const std::map<std::string, unsigned long long int> trader_prices{
        // Prices in Gold
        {"crystal_core",         2500},
        {"common_material",      150},
        {"uncommon_material",    300},
        {"rare_material",        600},
        {"epic_material",        1000},
        {"legendary_material",   10000},
        // Prices in Crystal Cores
        {"helmet_blueprint",     3},
        {"chestplate_blueprint", 4},
        {"greaves_blueprint",    5},
        {"boots_blueprint",      2},
        {"shield_blueprint",     3},
        {"weapon_blueprint",     5},
        {"first_aid_kit",        10}};

void draw_base(tcod::Console &main_win, tcod::ContextPtr &context, int y, unsigned int size, unsigned int page, bool is_blacksmith_mode, bool is_inventory_modifier_mode);

void print_item(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, bool is_selected);

void print_description(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line);

void inventory_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config);

void town_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Dungeon &dungeon_data, No_Delete &perm_config, Thread_Flags &thread_flags);

void reforge_repair_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config);

void eat_drink_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user);

bool decrease_amount(unsigned long long int &original, unsigned int amount);

bool remove_misc_item(Miscellaneous &user, unsigned int csr_pos, unsigned int amount);

void add_misc_item(Miscellaneous &user, unsigned int csr_pos, unsigned int amount);

void print_misc_item(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user, unsigned int csr_pos);

void show_misc_items(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user);

bool unequip_item(Player &user, Item *cur_item);

void help_mode(tcod::Console &main_win, tcod::ContextPtr &context, std::string mode);

void init_copy(std::vector<Item> &original, std::vector<Item *> &items_copy);

void process_copy(std::vector<Item> &original, std::vector<Item *> &items_copy, No_Delete &perm_config);

void inventory_storage(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config, Chest &chest);

void inventory_retrieve(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config, Chest &chest);

void inventory_modifier_selection(tcod::Console &main_win, tcod::ContextPtr &context, No_Delete &perm_config, std::vector<Item> original_copy, std::vector<Item *> &items_copy);

void show_merchant_items(tcod::Console &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &user);

void sell_items_to_merchant(tcod::Console &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &user, No_Delete &perm_config);

void store_misc_items(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user, Chest &chest);

void retrieve_misc_items(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user, Chest &chest);

int calculate_price(const Item &cur, double relationship, bool player_sell=false);

void trader_misc_menu(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &gear_merchant);

void mysterious_trader_items(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &mysterious_merchant);
