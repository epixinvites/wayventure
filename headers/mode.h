#pragma once
#include "classes.h"
void print_description(WINDOW *main_win, const Item *cur_item, int line);
void inventory_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar,Player &User);
void bar_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar,Player &User,NPC &npc);
void reforge_repair_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, Player &User);
void eat_drink_mode(WINDOW *main_win, WINDOW *status_win, Player &User);
void show_misc_items(WINDOW *main_win, WINDOW *interaction_bar, Miscellaneous &User);
void help_mode(WINDOW *main_win, WINDOW *interaction_bar, std::string mode);
