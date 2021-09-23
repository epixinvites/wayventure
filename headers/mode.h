#pragma once
#include "classes.h"
#include "main.h"
void print_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line);
void inventory_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config);
void bar_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User,NPC &npc, NoDelete &perm_config);
void reforge_repair_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  Player &User, NoDelete &perm_config);
void eat_drink_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User);
void show_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User);
void help_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string mode);
void init_copy(Player &User, std::vector<Item*> &items_copy);
void process_copy(Player &User, std::vector<Item*> &items_copy, const NoDelete &perm_config);
void inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config, const std::vector<Item*> items_copy);
