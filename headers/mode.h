#pragma once
void print_description(WINDOW *main_win, const Item *cur_item, int line);
void inventory_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar,Player &User);
