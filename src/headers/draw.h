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

#include <sstream>
#include "main.h"
#include "classes.h"

void draw_level(tcod::Console &main_win, tcod::ContextPtr &context, Level current);

void draw_stats(tcod::Console &main_win, tcod::ContextPtr &context, const Player &user);

void draw_player(tcod::Console &main_win, tcod::ContextPtr &context, int x, int y);

void draw_monster(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<Monster> &monsters);

void draw_border(tcod::Console &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context);

void draw_doors(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<DoorData> &door_data, const std::vector<StaircaseData> &stair_data);

void draw_inventory(tcod::Console &main_win, tcod::ContextPtr &context, const Player &user, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, bool is_blacksmith_mode=false, bool is_inventory_modifier_mode=false);

void draw_loot_box(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<LootData> &loot_in_room);

void redraw_main_dungeon(tcod::Console &main_win, tcod::ContextPtr &context, Csr csr_pos, const Player &user, const Dungeon &dungeon_data, const RoomData *cur_room);

