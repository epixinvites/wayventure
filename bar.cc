#include <curses.h>
#include <fstream>
#include "headers/mode.h"
#include "headers/classes.h"
#include "headers/draw.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
struct Bartender{
    int relation = 50;
};
struct Farmer{
    int relation = 50;
};
struct Bank{
    unsigned int saved_gold = 0;
    unsigned int total_gold = 0;
    double interest = 0.01;
    unsigned long long last_applied = 0;
};
class Chest{
    std::vector<Item> storage;
};
class Merchant{
    int relation = 50;
    std::vector<Item> store;
};
struct NPC{
    Bartender bartender;
    Farmer farmer;
    Bank bank;
    Chest chest;
    Merchant gear_merchant;
    Merchant mysterious_trader;
};
void char_move(int ch, WINDOW *main_win, Csr &csr_pos, const std::vector<std::string> &pub_layout){
    if((ch=='a'||ch==KEY_LEFT)&&csr_pos.first>1&&pub_layout[csr_pos.second][csr_pos.first-1]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first,' ');
        csr_pos.first--;
        draw_player(main_win, csr_pos.first, csr_pos.second);
        wrefresh(main_win);
    }
    if((ch=='d'||ch==KEY_RIGHT)&&csr_pos.first<78&&pub_layout[csr_pos.second][csr_pos.first+1]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.first++;
        draw_player(main_win, csr_pos.first, csr_pos.second);
        wrefresh(main_win);
    }
    if((ch=='w'||ch==KEY_UP)&&csr_pos.second>1&&pub_layout[csr_pos.second-1][csr_pos.first]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.second--;
        draw_player(main_win, csr_pos.first, csr_pos.second);
        wrefresh(main_win);
    }
    if((ch=='s'||ch==KEY_DOWN)&&csr_pos.second<48&&pub_layout[csr_pos.second+1][csr_pos.first]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first,' ');
        csr_pos.second++;
        draw_player(main_win, csr_pos.first, csr_pos.second);
        wrefresh(main_win);
    }
}
void bar_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar,Player &User){
    Csr csr_pos{78,1};
    std::ifstream pub_layout_file("res/bar_layout.txt");
    std::vector<std::string> pub_layout; // {50,80}
    for(int i = 0; i<50; i++){
        std::string line; std::getline(pub_layout_file,line);
        pub_layout.push_back(line);
    }
    wclear(main_win);
    wclear(interaction_bar);
    for(int i = 0; i<pub_layout.size(); i++){
        for(int j = 0; j<pub_layout[i].size(); j++){
            if(pub_layout[i][j]=='.'){
                wattron(main_win,COLOR_PAIR(6));
                mvwaddch(main_win,i,j,pub_layout[i][j]);
                wattroff(main_win,COLOR_PAIR(6));
            }
            else if(pub_layout[i][j]=='~'){
                wattron(main_win,COLOR_PAIR(7));
                mvwaddch(main_win,i,j,pub_layout[i][j]);
                wattroff(main_win,COLOR_PAIR(7));
            }
            else if(pub_layout[i][j]=='{'||pub_layout[i][j]=='}'){
                wattron(main_win,COLOR_PAIR(10));
                mvwaddch(main_win,i,j,pub_layout[i][j]);
                wattroff(main_win,COLOR_PAIR(10));
            }
            else{
                mvwaddch(main_win,i,j,pub_layout[i][j]);
            }
        }
    }
    mvwaddstr(interaction_bar,0,0,"Pub");
    draw_player(main_win, csr_pos.first, csr_pos.second);
    wrefresh(interaction_bar);
    wrefresh(main_win);
    int ch;
    while(true){
        ch=wgetch(main_win);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==KEY_LEFT||ch==KEY_RIGHT||ch==KEY_DOWN||ch==KEY_UP){
            char_move(ch, main_win, csr_pos, pub_layout);
        }
        if(ch=='x'){

        }
    }
}
