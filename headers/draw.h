#pragma once
#include <sstream>
#include "main.h"
#include "classes.h"
void draw_level(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, level Current);
void draw_stats(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, Player stats);
void draw_player(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, int x, int y);
void draw_monster(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, std::vector<monster> monsters);
void draw_border(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context);
void draw_doors(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, level Current);
void redraw_dungeon(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, level Current, std::vector<monster> monsters, Csr csr_pos);
void redraw_everything(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context,  Csr csr_pos, Player User, level Current, std::vector<monster> monsters);
