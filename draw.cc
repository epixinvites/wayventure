#include "headers/draw.h"
#include <sstream>
void clear_and_draw_dialog(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string dialog){
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0,0}, dialog, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}
void draw_level(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, level Current){
    SDL_wclear_dialog_bar(main_win, context);
    std::stringstream ss;
    ss<<'L'<<Current.lvl<<' '<<Current.x<<':'<<Current.y;
    tcod::print(*main_win, {0,0}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}
void draw_stats(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player stats){
    SDL_wclear_stats_bar(main_win, context);
    std::stringstream ss;
    ss<<"HP:"<<stats.cur_hp<<" Attk:"<<stats.attk<<" Def:"<<stats.def<<" Shield:"<<stats.cur_shield<<" CritChn:"<<stats.crit_chance<<" CritDmg:"<<stats.crit_dmg<<" Gold:"<<stats.gold<<" T:"<<stats.steps/10;
    tcod::print(*main_win, {0,51}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss<<"Saturation ("<<stats.saturation<<"/100)"<<" [";
    for(int i = 1; i<=50; i++){
        if(i*2<=stats.saturation){
            ss<<"#";
        }
        else{
            ss<<"-";
        }
    }
    ss<<"]";
    tcod::print(*main_win, {0,52}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss<<"Hydration ("<<stats.hydration<<"/100)"<<" [";
    for(int i = 1; i<=50; i++){
        if(i*2<=stats.hydration){
            ss<<"#";
        }
        else{
            ss<<"-";
        }
    }
    ss<<"]";
    tcod::print(*main_win, {0,53}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}
void draw_player(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int x, int y){
    TCOD_console_put_char_ex(main_win.get(), x, y+1, '@', CYAN, BLACK);
}
void draw_monster(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::vector<monster> monsters){
    for(int i=0; i<monsters.size(); i++){
        if(monsters[i].type=='b'){ // boss
            TCOD_console_put_char_ex(main_win.get(), monsters[i].x, monsters[i].y+1, 'X', WHITE, BLACK);
        }
        else if(monsters[i].type=='x'){ // level boss
            TCOD_console_put_char_ex(main_win.get(), monsters[i].x, monsters[i].y+1, 'X', BLACK, YELLOW);
        }
        else if(monsters[i].type=='f'){ // final boss
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
void draw_doors(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, level Current){
    if(Current.y>1){
        TCOD_console_put_char_ex(main_win.get(), 39, 50, '+', BLACK, WHITE);
    }
    if(Current.y<5){
        TCOD_console_put_char_ex(main_win.get(), 39, 1, '+', BLACK, WHITE);
    }
    if(Current.x>1){
        TCOD_console_put_char_ex(main_win.get(), 0, 25, '+', BLACK, WHITE);
    }
    if(Current.x<5){
        TCOD_console_put_char_ex(main_win.get(), 79, 25, '+', BLACK, WHITE);
    }
    if(Current.x==1&&Current.y==1&&Current.lvl==1){
        TCOD_console_put_char_ex(main_win.get(), 1, 49, '<', WHITE, BLACK);
    }
    if(Current.x==1&&Current.y==1&&Current.lvl>1){
        TCOD_console_put_char_ex(main_win.get(), 39, 25, '<', WHITE, BLACK);
    }
    if(Current.x==5&&Current.y==5&&Current.lvl<5){
        TCOD_console_put_char_ex(main_win.get(), 39, 25, '>', WHITE, BLACK);
    }
}

void redraw_dungeon(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, level Current, std::vector<monster> monsters, Csr csr_pos){
    TCOD_console_clear(main_win.get());
    draw_border(main_win, context);
    draw_doors(main_win, context, Current);
    draw_player(main_win, context, csr_pos.first, csr_pos.second);
    draw_monster(main_win, context, monsters);
    context->present(*main_win);
}
void redraw_everything(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Csr csr_pos, Player User, level Current, std::vector<monster> monsters, bool is_draw_level = true){
    TCOD_console_clear(main_win.get());
    if(is_draw_level){
        draw_level(main_win, context, Current);
    }
    draw_stats(main_win, context, User);
    draw_border(main_win, context);
    draw_doors(main_win, context, Current);
    draw_player(main_win, context, csr_pos.first, csr_pos.second);
    draw_monster(main_win, context, monsters);
    context->present(*main_win);
}

