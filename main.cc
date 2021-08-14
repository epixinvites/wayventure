#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <curses.h>
#include <fstream>
#include <unordered_map>
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include "headers/mode.h"
bool check_surroundings(std::vector<monster> monsters, int x, int y){
    for(int i = 0; i<monsters.size(); i++){
        if(x==monsters[i].x&&y==monsters[i].y){
            return true;
        }
    }
    return false;
}
std::pair<int,char> check_monster_pos(std::vector<monster> monsters, int x, int y){
    for(int i = 0; i<monsters.size(); i++){
        if(x==monsters[i].x&&y==monsters[i].y){
            return {i,monsters[i].type};
        }
    }
    return {-1,'0'};
}
void char_move(WINDOW *main_win, int ch, std::pair<int,int> &csr_pos, std::vector<monster> monsters, Player &User){
    bool require_move = false;
    if((ch=='a'||ch==KEY_LEFT)&&csr_pos.first>1&&!check_surroundings(monsters,csr_pos.first-1,csr_pos.second)){
        mvwaddch(main_win,csr_pos.second,csr_pos.first,' ');
        csr_pos.first--; require_move=true;
    }
    if((ch=='d'||ch==KEY_RIGHT)&&csr_pos.first<78&&!check_surroundings(monsters,csr_pos.first+1,csr_pos.second)){
        mvwaddch(main_win,csr_pos.second,csr_pos.first,' ');
        csr_pos.first++; require_move=true;
    }
    if((ch=='w'||ch==KEY_UP)&&csr_pos.second>1&&!check_surroundings(monsters,csr_pos.first,csr_pos.second-1)){
        mvwaddch(main_win,csr_pos.second,csr_pos.first,' ');
        csr_pos.second--; require_move=true;
    }
    if((ch=='s'||ch==KEY_DOWN)&&csr_pos.second<48&&!check_surroundings(monsters,csr_pos.first,csr_pos.second+1)){
        mvwaddch(main_win,csr_pos.second,csr_pos.first,' ');
        csr_pos.second++; require_move=true;
    }
    if(require_move){
        User.steps++;
        if(User.steps>100000000000000000){
            User.steps/=10;
        }
        draw_player(main_win,csr_pos.first,csr_pos.second);
        wrefresh(main_win);
    }
}
void calculate_damage (Player &User, monster_stats &monster){
    if(User.crit_chance>0&&return_chance(User.crit_chance)){ // Calculate damage that enemy takes
        monster.hp=monster.hp-(User.attk*(1+(User.crit_dmg/100.0)));
    }
    else{
        monster.hp=monster.hp-(User.attk-monster.def);
    }
    if(User.def>=monster.attk){
        return;
    }
    else{
        User.cur_hp-=(monster.attk-User.def);
    }
}
bool player_battle(WINDOW *main_win, WINDOW *status_win, Player &User, level Current, char monster_type){ // WIP return false if lost, true if won. Return to mainmenu if false
    monster_stats monster = create_monster(Current,monster_type);
    while(true){
        wclear(main_win);
        draw_border(main_win);
        std::stringstream user_output;
        std::stringstream enemy_output;
        draw_stats(status_win,User);
        user_output<<User.name<<"~ HP:"<<User.cur_hp<<" Defence:"<<User.def<<" Shield:"<<User.cur_shield<<" Heal:"<<User.inv.heal_amount;
        enemy_output<<"Enemy~ HP:"<<monster.hp<<" Attk:"<<monster.attk<<" Def:"<<monster.def;
        mvwaddstr(main_win,48,1,user_output.str().c_str());
        mvwaddstr(main_win,1,1,enemy_output.str().c_str());
        int ch = wgetch(main_win);
        if(ch=='1'){
            calculate_damage(User,monster);
            if(User.cur_hp<=0){
                return false;
            }
            if(monster.hp<=0){
                return true;
            }
        }
        if(ch=='2'){
            User.cur_shield=User.ori_shield;
            if(User.def>=monster.attk){
                continue;
            }
            if(User.cur_shield>0){ // Calculate damage that player takes
                if(User.cur_shield>=monster.attk){
                    User.cur_shield-=monster.attk;
                }
                else{
                    User.cur_hp=User.cur_hp-(monster.attk-User.def-User.cur_shield);
                    User.cur_shield = 0;
                }
            }
        }
        if(ch=='3'){
            if(User.inv.heal_amount>0&&User.cur_hp!=User.ori_hp){
                User.inv.heal_amount--;
                User.cur_hp=User.ori_hp;
            }
        }
        wrefresh(main_win);
    }
    return false;
}
std::pair<bool,bool> attack_monster(WINDOW *main_win, WINDOW *status_win, std::vector<monster> &monsters, std::pair<int,int> csr_pos, Player &User, level Current){
    std::pair<bool,bool> attack_status {false,false}; // .first = if surroundings have monsters, .second is_alive
    std::pair<int,char> pos;
    if(check_surroundings(monsters,csr_pos.first-1,csr_pos.second)){
        attack_status.first=true;
        pos = check_monster_pos(monsters,csr_pos.first-1,csr_pos.second);
        attack_status.second = player_battle(main_win,status_win,User,Current,pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    if(check_surroundings(monsters,csr_pos.first+1,csr_pos.second)){
        attack_status.first=true;
        pos = check_monster_pos(monsters,csr_pos.first+1,csr_pos.second);
        attack_status.second = player_battle(main_win,status_win,User,Current,pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    if(check_surroundings(monsters,csr_pos.first,csr_pos.second-1)){
        attack_status.first=true;
        pos = check_monster_pos(monsters,csr_pos.first,csr_pos.second-1);
        attack_status.second = player_battle(main_win,status_win,User,Current,pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    if(check_surroundings(monsters,csr_pos.first,csr_pos.second+1)){
        attack_status.first=true;
        pos = check_monster_pos(monsters,csr_pos.first,csr_pos.second+1);
        attack_status.second = player_battle(main_win,status_win,User,Current,pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    return {attack_status};
}
void move_door(std::vector<std::pair<int,int>> &doors, std::vector<monster> &monsters, level &Current, std::pair<int,int> &csr_pos){
    for(int i = 0; i<doors.size(); i++){
        if(doors[i].first==csr_pos.first&&doors[i].second==csr_pos.second-1){
            ++Current.y;
            csr_pos={39,48};
            doors.clear();
            monsters.clear();
            generate_doors(doors,Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first&&doors[i].second==csr_pos.second+1){
            --Current.y;
            csr_pos={39,1};
            doors.clear();
            monsters.clear();
            generate_doors(doors,Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first+1&&doors[i].second==csr_pos.second){
            ++Current.x;
            csr_pos={1,24};
            doors.clear();
            monsters.clear();
            generate_doors(doors,Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first-1&&doors[i].second==csr_pos.second){
            --Current.x;
            csr_pos={78,24};
            doors.clear();
            monsters.clear();
            generate_doors(doors,Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
    }
}
bool move_staircase(level &Current, std::pair<int,int> csr_pos){
    if(Current.lvl<5&&Current.x==5&&Current.y==5){ // go down
        if(csr_pos.first==39&&csr_pos.second==24){
            Current.lvl++;
            Current.reset(0);
            return true;
        }
    }
    if(Current.lvl>1&&Current.x==1&&Current.y==1){ // go down
        if(csr_pos.first==39&&csr_pos.second==24){
            Current.lvl--;
            Current.reset(1);
            return true;
        }
    }
    return false;
}
void end_program(int sig){
    if(sig==0){
        endwin();
        std::cout<<"Nyooooom~"<<std::endl;
        std::ifstream cosmos_ascii("res/cosmos.txt");
        std::string line;
        while(std::getline(cosmos_ascii,line)){
            std::cout<<line<<std::endl;
        }
        return;
    }
    if(sig==1){
        endwin();
        std::cout<<"The Dark Lord laughed: \"Weakling...\""<<std::endl;
        return;
    }
}
void end_program(int sig, std::string error){
    if(sig==-1){
        endwin();
        std::cout<<"Bzzt- Something fatal has occured- Error rep.r..d....!@#$%^&*~__+|:<?"<<std::endl;
        std::cerr<<error<<std::endl;
        return;
    }
}
void init_dungeon(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, std::pair<int,int> &csr_pos, Player &User, level &Current){
    std::vector<monster> monsters;
    std::vector<std::pair<int,int>> doors;
    generate_monsters(monsters, Current, csr_pos);
    generate_doors(doors,Current);
    draw_level(interaction_bar,Current);
    draw_stats(status_win,User);
    while(true){
        redraw_dungeon(main_win,Current,monsters,csr_pos);
        int ch = wgetch(main_win);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==KEY_LEFT||ch==KEY_RIGHT||ch==KEY_DOWN||ch==KEY_UP){
            redraw_dungeon(main_win,Current,monsters,csr_pos);
            char_move(main_win,ch,csr_pos,monsters,User);
            draw_stats(status_win,User);
        }
        if(ch=='z'){
            move_door(doors, monsters, Current, csr_pos);
            redraw_dungeon(main_win,Current,monsters,csr_pos);
            draw_level(interaction_bar,Current);
        }
        if(ch=='x'){
            std::pair <bool,bool> attack_status = attack_monster(main_win,status_win,monsters,csr_pos,User,Current);
            if(attack_status.first&&!attack_status.second){ // If dead return to main menu
                end_program(1);
                return;
            }
            if(attack_status.first&&attack_status.second){ // If win redraw dungeon and move on
                redraw_dungeon(main_win,Current,monsters,csr_pos);
            }
        }
        if(ch=='c'){
            if(move_staircase(Current,csr_pos)){
                generate_doors(doors,Current);
                redraw_dungeon(main_win,Current,monsters,csr_pos);
                draw_level(interaction_bar,Current);
            }
        }
        if(ch=='r'){
            redraw_dungeon(main_win,Current,monsters,csr_pos);
            draw_level(interaction_bar,Current);
            draw_stats(status_win,User);
        }
        if(ch=='i'){
            if(!User.inv.item.empty()){
                inventory_mode(main_win,status_win,interaction_bar,User);
                redraw_everything(main_win,status_win,interaction_bar,csr_pos,User,Current,monsters);
            }
        }
        if(ch=='q'){
            end_program(0);
            return;
        }
    }
}
void init(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, std::pair<int,int> &csr_pos, Player &User, level &Current){
    std::ifstream ascii_wayfarer("res/wayfarer.txt");
    if(!ascii_wayfarer){end_program(-1,"wayfarer.txt not found!"); return;}
    int win_iterator=0;
    std::string line;
    while(std::getline(ascii_wayfarer, line)){
        mvwaddstr(main_win,win_iterator,1,line.c_str());
        win_iterator++;
    }
    mvwaddstr(main_win,10,1,"A simple old school dungeon adventure with endless posibilities...");
    mvwaddstr(main_win,20,15,"Press any key to continue...");
    wrefresh(main_win);
    ascii_wayfarer.close();
    getchar();
    init_dungeon(main_win,status_win,interaction_bar,csr_pos,User,Current);
}
int main(){
    initscr(); cbreak(); noecho(); curs_set(0);
    WINDOW *main_win = newwin(50,80,1,0); // 50 rows, 80 columns
    WINDOW *status_win = newwin(1,100,51,0); // Status bar
    WINDOW *interaction_bar = newwin(1,200,0,0);
    Player User;
    User.init(); // IMPORTANT
    level Current{1,1,1};
    keypad(main_win, TRUE);
    std::pair<int,int> csr_pos{1,1}; // x, y
    if(has_colors()){
        start_color();
        init_pair(1,COLOR_CYAN,COLOR_BLACK); // Player
        init_pair(2,COLOR_RED,COLOR_BLACK); // enemy
        init_pair(3,COLOR_WHITE,COLOR_BLACK); // boss
        init_pair(4,COLOR_YELLOW,COLOR_BLACK); // loot
        init_pair(5,COLOR_WHITE,COLOR_BLACK); // common
        init_pair(6,COLOR_GREEN,COLOR_BLACK); // uncommon
        init_pair(7,COLOR_BLUE,COLOR_BLACK); // rare
        init_pair(8,COLOR_MAGENTA,COLOR_BLACK); // epic
        init_pair(9,COLOR_YELLOW,COLOR_BLACK); // legendary
        init_pair(10,COLOR_RED,COLOR_BLACK); // artifact
        init_pair(11,COLOR_BLACK,COLOR_WHITE); // doors
        init_pair(12,COLOR_WHITE,COLOR_MAGENTA); // special doors
        init_pair(13,COLOR_BLACK,COLOR_YELLOW); // level boss
        init_pair(14,COLOR_WHITE,COLOR_MAGENTA); // final boss
        init_pair(15,COLOR_BLACK,COLOR_WHITE); // common inverted
        init_pair(16,COLOR_BLACK,COLOR_GREEN); // uncommon inverted
        init_pair(17,COLOR_BLACK,COLOR_BLUE); // rare inverted
        init_pair(18,COLOR_BLACK,COLOR_MAGENTA); // epic inverted
        init_pair(19,COLOR_BLACK,COLOR_YELLOW); // legendary inverted
        init_pair(20,COLOR_BLACK,COLOR_RED); // artifact inverted

    }
    init(main_win,status_win,interaction_bar,csr_pos,User,Current);
    std::cin.get();
    return 0;

    /*
     *    init_extended_color(16,372,843,0);
     *    init_extended_pair(15,16,COLOR_BLACK);
     *    wattron(main_win,COLOR_PAIR(15));
     *    mvwaddstr(main_win,0,0,"Test");
     *    wattroff(main_win,COLOR_PAIR(15));
     *    wrefresh(main_win);
     */

    /*
    // Generate loot after defeating enemy
    // Save data to file / Retrieve data from file
    // make install (copy resources to $HOME/.project-cosmos/)
    // Hunger and Auto-health regen
    // Bar
    // Status effects
    // Tutorial level
    */

}
