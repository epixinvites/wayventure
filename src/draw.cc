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

#include "headers/draw.h"
#include <sstream>

void clear_and_draw_dialog(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialog){
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, dialog, WHITE, BLACK);
    context->present(main_win);
}

void draw_level(tcod::Console &main_win, tcod::ContextPtr &context, Level current){
    SDL_wclear_dialog_bar(main_win, context);
    std::stringstream ss;
    ss << 'L' << current.lvl << ' ' << current.x << ':' << current.y;
    tcod::print(main_win, {0, 0}, ss.str(), WHITE, BLACK);
//    context->present(main_win);
}

void draw_stats(tcod::Console &main_win, tcod::ContextPtr &context, const Player &user){
    SDL_wclear_stats_bar(main_win, context);
    std::stringstream ss;
    ss << "HP:" << user.cur_hp << " Attk:" << user.attk << " Def:" << user.def << " Shield:" << user.cur_shield << " CritChn:" << user.crit_chance << " CritDmg:" << user.crit_dmg;
    tcod::print(main_win, {0, 51}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "Gold:" << user.gold << " T:" << user.steps/10;
    tcod::print(main_win, {0, 52}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "Saturation (" << user.saturation << "/100)" << " [";
    for(int i=1; i<=50; i++){
        if(i*2<=user.saturation){
            ss << "#";
        }
        else{
            ss << "-";
        }
    }
    ss << "]";
    tcod::print(main_win, {0, 53}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "Hydration (" << user.hydration << "/100)" << " [";
    for(int i=1; i<=50; i++){
        if(i*2<=user.hydration){
            ss << "#";
        }
        else{
            ss << "-";
        }
    }
    ss << "]";
    tcod::print(main_win, {0, 54}, ss.str(), WHITE, BLACK);
//    context->present(main_win);
}

void draw_player(tcod::Console &main_win, tcod::ContextPtr &context, int x, int y){
    TCOD_console_put_char_ex(main_win.get(), x, y+1, '@', CYAN, BLACK);
}

void draw_monster(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<Monster> &monsters){
    for(int i=0; i<monsters.size(); i++){
        if(monsters[i].type==Enemy_Type::ROOM_BOSS){ // boss
            TCOD_console_put_char_ex(main_win.get(), monsters[i].x, monsters[i].y+1, 'X', WHITE, BLACK);
        }
        else if(monsters[i].type==Enemy_Type::LEVEL_BOSS){ // level boss
            TCOD_console_put_char_ex(main_win.get(), monsters[i].x, monsters[i].y+1, 'X', BLACK, YELLOW);
        }
        else if(monsters[i].type==Enemy_Type::FINAL_BOSS){ // final boss
            TCOD_console_put_char_ex(main_win.get(), monsters[i].x, monsters[i].y+1, 'X', BLACK, PURPLE);
        }
        else{
            TCOD_console_put_char_ex(main_win.get(), monsters[i].x, monsters[i].y+1, 'x', LIGHT_RED, BLACK);
        }
    }
}

void draw_border(tcod::Console &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context){
    for(int i=0; i<80; i++){ // vertical
        TCOD_console_put_char_ex(main_win.get(), i, 1, '#', WHITE, BLACK);
        TCOD_console_put_char_ex(main_win.get(), i, 50, '#', WHITE, BLACK);
    }
    for(int i=2; i<50; i++){ // horizontal
        TCOD_console_put_char_ex(main_win.get(), 0, i, '#', WHITE, BLACK);
        TCOD_console_put_char_ex(main_win.get(), 79, i, '#', WHITE, BLACK);
    }
}

void draw_doors(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<DoorData> &door_data, const std::vector<StaircaseData> &stair_data){
    for(const auto &i:door_data){
        TCOD_console_put_char_ex(main_win.get(), i.x, i.y+1, '+', BLACK, WHITE);
    }
    if(!stair_data.empty()){
        for(const auto &i:stair_data){
            if(i.behaviour==StaircaseData::Behaviour::UP){
                TCOD_console_put_char_ex(main_win.get(), i.x, i.y+1, '<', WHITE, BLACK);
            }
            else if(i.behaviour==StaircaseData::Behaviour::DOWN){
                TCOD_console_put_char_ex(main_win.get(), i.x, i.y+1, '>', WHITE, BLACK);
            }
        }
    }
}

void draw_loot_box(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<LootData> &loot_in_room){
    if(!loot_in_room.empty()){
        for(const auto &i:loot_in_room){
            TCOD_console_put_char_ex(main_win.get(), i.room_position.x, i.room_position.y+1, 'o', YELLOW, BLACK);
        }
    }
}

void redraw_main_dungeon(tcod::Console &main_win, tcod::ContextPtr &context, Csr csr_pos, const Player &user, const Dungeon &dungeon_data, const RoomData *cur_room){
    main_win.clear({0x20, WHITE, BLACK});
    draw_level(main_win, context, dungeon_data.current);
    draw_stats(main_win, context, user);
    draw_border(main_win, context);
    draw_doors(main_win, context, cur_room->door_data, cur_room->staircase_data);
    draw_loot_box(main_win, context, cur_room->loot_in_room);
    draw_player(main_win, context, csr_pos.x, csr_pos.y);
    draw_monster(main_win, context, cur_room->enemy_data);
    context->present(main_win);
}

