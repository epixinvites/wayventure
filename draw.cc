#include "headers/draw.h"
#include <sstream>
void draw_level(WINDOW *interaction_bar, level Current){
    wclear(interaction_bar);
    std::stringstream ss;
    ss<<'L'<<Current.lvl<<' '<<Current.x<<':'<<Current.y;
    mvwaddstr(interaction_bar, 0, 0, ss.str().c_str());
    wrefresh(interaction_bar);
}
void draw_stats(WINDOW *status_win, Player stats){
    wclear(status_win);
    std::stringstream ss;
    ss<<"HP:"<<stats.cur_hp<<" Attk:"<<stats.attk<<" Def:"<<stats.def<<" Shield:"<<stats.cur_shield<<" CritChn:"<<stats.crit_chance<<" CritDmg:"<<stats.crit_dmg<<" Gold:"<<stats.gold<<" T:"<<stats.steps/10;
    mvwaddstr(status_win, 0, 0, ss.str().c_str());
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
    mvwaddstr(status_win, 1, 0, ss.str().c_str());
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
    mvwaddstr(status_win, 2, 0, ss.str().c_str());
    wrefresh(status_win);
}
void draw_player(WINDOW *main_win, int x, int y){
    wattron(main_win, COLOR_PAIR(1));
    mvwaddch(main_win, y, x, '@');
    wattroff(main_win, COLOR_PAIR(1));
}
void draw_monster(WINDOW *main_win, std::vector<monster> monsters){
    for(int i=0; i<monsters.size(); i++){
        if(monsters[i].type=='b'){ // boss
            wattron(main_win, COLOR_PAIR(3));
            mvwaddch(main_win, monsters[i].y, monsters[i].x, 'X');
            wattroff(main_win, COLOR_PAIR(3));
        }
        else if(monsters[i].type=='x'){ // level boss
            wattron(main_win, COLOR_PAIR(13));
            mvwaddch(main_win, monsters[i].y, monsters[i].x, 'X');
            wattroff(main_win, COLOR_PAIR(13));
        }
        else if(monsters[i].type=='f'){ // final boss
            wattron(main_win, COLOR_PAIR(14));
            mvwaddch(main_win, monsters[i].y, monsters[i].x, 'X');
            wattroff(main_win, COLOR_PAIR(14));
        }
        else{
            wattron(main_win, COLOR_PAIR(2));
            mvwaddch(main_win, monsters[i].y, monsters[i].x, 'x');
            wattroff(main_win, COLOR_PAIR(2));
        }
    }
}
void draw_border(WINDOW *main_win){
    for(int i=0; i<80; i++){ // vertical
        mvwaddch(main_win, 0, i, '#');
        mvwaddch(main_win, 49, i, '#');
    }
    for(int i=1; i<50; i++){ // horizontal
        mvwaddch(main_win, i, 0, '#');
        mvwaddch(main_win, i, 79, '#');
    }
}
void draw_doors(WINDOW *main_win, level Current){
    wattron(main_win, COLOR_PAIR(11));
    if(Current.y>1){
        mvwaddch(main_win, 49, 39, '+');
    }
    if(Current.y<5){
        mvwaddch(main_win, 0, 39, '+');
    }
    if(Current.x>1){
        mvwaddch(main_win, 24, 0, '+');
    }
    if(Current.x<5){
        mvwaddch(main_win, 24, 79, '+');
    }
    wattroff(main_win, COLOR_PAIR(11));
    if(Current.x==1&&Current.y==1&&Current.lvl==1){
        wattron(main_win, COLOR_PAIR(12));
        mvwaddch(main_win, 49, 39, '+'); // Bar
        wattroff(main_win, COLOR_PAIR(12));
    }
    if(Current.x==1&&Current.y==1&&Current.lvl>1){
        mvwaddch(main_win, 24, 39, '<');
    }
    if(Current.x==5&&Current.y==5&&Current.lvl<5){
        mvwaddch(main_win, 24, 39, '>');
    }
}
void redraw_dungeon(WINDOW *main_win, level Current, std::vector<monster> monsters, std::pair<int,int> csr_pos){
    wclear(main_win);
    draw_border(main_win);
    draw_doors(main_win, Current);
    draw_player(main_win, csr_pos.first, csr_pos.second);
    draw_monster(main_win, monsters);
}
void redraw_everything(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, std::pair<int,int> csr_pos, Player User, level Current, std::vector<monster> monsters){
    wclear(main_win);
    wclear(status_win);
    wclear(interaction_bar);
    draw_level(interaction_bar, Current);
    draw_stats(status_win, User);
    draw_player(main_win, csr_pos.first, csr_pos.second);
    draw_monster(main_win, monsters);
    draw_border(main_win);
    draw_doors(main_win, Current);
    wrefresh(main_win);
    wrefresh(status_win);
    wrefresh(interaction_bar);
}

