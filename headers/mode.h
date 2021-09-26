#pragma once
#include "classes.h"
#include "main.h"
void print_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line);
void inventory_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config);
void bar_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User,NPC &npc, NoDelete &perm_config);
void reforge_repair_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  Player &User, NoDelete &perm_config);
void eat_drink_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User);
bool decrease_amount(unsigned long long int &original, unsigned int amount);
bool remove_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount);
void add_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount);
void print_misc_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, unsigned int csr_pos);
void show_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User);
bool unequip_item(Player &User, Item *cur_item);
void help_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string mode);
void init_copy(std::vector<Item> &original, std::vector<Item*> &items_copy);
void process_copy(std::vector<Item> &original, std::vector<Item*> &items_copy, NoDelete &perm_config);
void inventory_storage(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config, Chest &chest);
void inventory_retrieve(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config, Chest &chest);
void inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config, std::vector<Item> original_copy, std::vector<Item*> items_copy);
void store_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User);
void retrieve_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User);
