#include "headers/draw.h"
#include <sstream>

void clear_and_draw_dialog(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialog){
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0, 0}, dialog, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}

void draw_level(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Level current){
    SDL_wclear_dialog_bar(main_win, context);
    std::stringstream ss;
    ss << 'L' << current.lvl << ' ' << current.x << ':' << current.y;
    tcod::print(*main_win, {0, 0}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
//    context->present(*main_win);
}

void draw_stats(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &user){
    SDL_wclear_stats_bar(main_win, context);
    std::stringstream ss;
    ss << "HP:" << user.cur_hp << " Attk:" << user.attk << " Def:" << user.def << " Shield:" << user.cur_shield << " CritChn:" << user.crit_chance << " CritDmg:" << user.crit_dmg;
    tcod::print(*main_win, {0, 51}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss << "Gold:" << user.gold << " T:" << user.steps/10;
    tcod::print(*main_win, {0, 52}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
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
    tcod::print(*main_win, {0, 53}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
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
    tcod::print(*main_win, {0, 54}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
//    context->present(*main_win);
}

void draw_player(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int x, int y){
    TCOD_console_put_char_ex(main_win.get(), x, y+1, '@', CYAN, BLACK);
}

void draw_monster(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<Monster> &monsters){
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

void draw_border(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context){
    for(int i=0; i<80; i++){ // vertical
        TCOD_console_put_char_ex(main_win.get(), i, 1, '#', WHITE, BLACK);
        TCOD_console_put_char_ex(main_win.get(), i, 50, '#', WHITE, BLACK);
    }
    for(int i=2; i<50; i++){ // horizontal
        TCOD_console_put_char_ex(main_win.get(), 0, i, '#', WHITE, BLACK);
        TCOD_console_put_char_ex(main_win.get(), 79, i, '#', WHITE, BLACK);
    }
}

void draw_doors(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<DoorData> &door_data){
    for(const auto &i:door_data){
        TCOD_console_put_char_ex(main_win.get(), i.x, i.y+1, '+', BLACK, WHITE);
    }
}

void draw_loot_box(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<LootData> &loot_in_room){
    if(!loot_in_room.empty()){
        for(const auto &i:loot_in_room){
            TCOD_console_put_char_ex(main_win.get(), i.room_position.x, i.room_position.y+1, 'o', YELLOW, BLACK);
        }
    }
}

void redraw_main_dungeon(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Csr csr_pos, const Player &user, const Dungeon &dungeon_data, const RoomData *cur_room){
    TCOD_console_clear(main_win.get());
    draw_level(main_win, context, dungeon_data.current);
    draw_stats(main_win, context, user);
    draw_border(main_win, context);
    draw_doors(main_win, context, cur_room->door_data);
    draw_loot_box(main_win, context, cur_room->loot_in_room);
    draw_player(main_win, context, csr_pos.x, csr_pos.y);
    draw_monster(main_win, context, cur_room->enemy_data);
    context->present(*main_win);
}

