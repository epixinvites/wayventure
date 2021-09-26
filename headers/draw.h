#pragma once
#include <sstream>
#include "main.h"
#include "classes.h"
void draw_level(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, level Current);
void draw_stats(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player User);
void draw_player(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int x, int y);
void draw_monster(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::vector<monster> monsters);
void draw_border(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context);
void draw_doors(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, level Current);
void draw_inventory(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item*> items_copy, unsigned int page_num, unsigned int csr_pos, bool is_blacksmith_mode=false, bool is_inventory_modifier_mode = false);
void redraw_dungeon(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, level Current, std::vector<monster> monsters, Csr csr_pos);
void redraw_everything(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Csr csr_pos, Player User, level Current, std::vector<monster> monsters);
