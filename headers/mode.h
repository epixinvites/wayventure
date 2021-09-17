#pragma once
#include "classes.h"
#include "main.h"
void print_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line);
void inventory_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, const NoDelete &perm_config);
void bar_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User,NPC &npc, const NoDelete &perm_config);
void reforge_repair_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  Player &User, const NoDelete &perm_config);
void eat_drink_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User);
void show_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User);
void help_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string mode);
