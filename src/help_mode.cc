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

#include <string>
#include <fstream>
#include "headers/main.h"

void help_mode(tcod::Console &main_win, tcod::ContextPtr &context, std::string mode){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Help", WHITE, BLACK);
    std::ifstream dungeon_help("src/res/help/"+mode+".txt");
    std::string line;
    int count=0;
    while(std::getline(dungeon_help, line)){
        tcod::print(main_win, {0, count+1}, line, WHITE, BLACK);
        count++;
    }
    context->present(main_win);
    int ch=0;
    while(!(ch>0&&ch<128)){
        ch=SDL_getch(main_win, context);
    }
}
