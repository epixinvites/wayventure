#pragma once

#include <sstream>
#include "main.h"
#include "classes.h"

void draw_level(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Level current);

void draw_stats(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &user);

void draw_player(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int x, int y);

void draw_monster(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<Monster> &monsters);

void draw_border(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context);

void draw_doors(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<DoorData> &door_data);

void draw_inventory(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &user, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, bool is_blacksmith_mode=false, bool is_inventory_modifier_mode=false);

void draw_loot_box(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<LootData> &loot_in_room);

void redraw_main_dungeon(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Csr csr_pos, const Player &user, const Dungeon &dungeon_data, const RoomData *cur_room);

