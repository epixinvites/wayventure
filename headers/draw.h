#pragma once
#include <curses.h>
#include <sstream>
#include "classes.h"
void draw_level(WINDOW *interaction_bar, level Current);
void draw_stats(WINDOW *status_win, Player stats);
void draw_player(WINDOW *main_win, int x, int y);
void draw_monster(WINDOW *main_win, std::vector<monster> monsters);
void draw_border(WINDOW *main_win);
void draw_doors(WINDOW *main_win, level Current);
void redraw_dungeon(WINDOW *main_win,level Current,std::vector<monster> monsters, std::pair<int,int> csr_pos);
void redraw_everything(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, std::pair<int,int> csr_pos, Player User, level Current,std::vector<monster> monsters);
