#include <iostream>
#include <random>
#include <sstream>
#include <curses.h>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <unistd.h>
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include "headers/mode.h"
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
bool check_surroundings(std::vector<monster> monsters, int x, int y){
    for(int i=0; i<monsters.size(); i++){
        if(x==monsters[i].x&&y==monsters[i].y){
            return true;
        }
    }
    return false;
}
std::pair<int,char> check_monster_pos(std::vector<monster> monsters, int x, int y){
    for(int i=0; i<monsters.size(); i++){
        if(x==monsters[i].x&&y==monsters[i].y){
            return{i,monsters[i].type};
        }
    }
    return{-1,'0'};
}
void char_move(WINDOW *main_win, int ch, Csr &csr_pos, std::vector<monster> monsters, Player &User){
    bool require_move=false;
    if((ch=='a'||ch==KEY_LEFT)&&csr_pos.first>1&&!check_surroundings(monsters, csr_pos.first-1, csr_pos.second)){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.first--;
        require_move=true;
    }
    if((ch=='d'||ch==KEY_RIGHT)&&csr_pos.first<78&&!check_surroundings(monsters, csr_pos.first+1, csr_pos.second)){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.first++;
        require_move=true;
    }
    if((ch=='w'||ch==KEY_UP)&&csr_pos.second>1&&!check_surroundings(monsters, csr_pos.first, csr_pos.second-1)){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.second--;
        require_move=true;
    }
    if((ch=='s'||ch==KEY_DOWN)&&csr_pos.second<48&&!check_surroundings(monsters, csr_pos.first, csr_pos.second+1)){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.second++;
        require_move=true;
    }
    if(require_move){
        User.steps++;
        if(User.steps>100000000000000000){
            User.steps/=10;
        }
        if(User.steps%5==0){
            User.hydration--;
        }
        if(User.steps%8==0){
            User.saturation--;
        }
        draw_player(main_win, csr_pos.first, csr_pos.second);
        wrefresh(main_win);
    }
}
void process_gear(Player &User, Item *&processed_item, int damage){
    if(processed_item!=nullptr){
        processed_item->uses++;
        processed_item->durability-=((damage/500.0)*(1+((101-processed_item->durability)/100)))/rarity_value(processed_item->rarity);
        if(processed_item->durability<=0.0){
            processed_item->durability = 0;
            processed_item->is_equipped = false;
            processed_item=nullptr;
        }
        else{
            processed_item->reinitialize_item();
        }
    }
}
std::pair<std::string,std::string> calculate_damage(Player &User, monster_stats &monster){
    std::stringstream first, second;
    if(User.crit_chance>0&&return_chance(User.crit_chance)){ // Calculate damage that enemy takes
        monster.hp=monster.hp-(User.attk*(1+(User.crit_dmg/100.0)));
        first<<"Critical Hit: "<<"You dealt "<<User.attk*(1+(User.crit_dmg/100.0))<<" damage";
    }
    else{
        monster.hp=monster.hp-(User.attk-monster.def);
        first<<"You dealt "<<User.attk-monster.def<<" damage";
    }
    if(User.def<monster.attk){ // Calculate damage I take
        if(User.cur_shield>monster.attk){
            User.cur_shield-=monster.attk;
            second<<"You blocked "<<monster.attk<<" damage";
        }
        else if(User.cur_shield<monster.attk&&User.cur_shield>0){
            User.cur_hp-=(monster.attk-User.cur_shield);
            second<<"You received "<<monster.attk-User.cur_shield<<" damage";
            User.cur_shield = 0;
        }
        else{
            User.cur_hp-=(monster.attk-User.def);
            second<<"You received "<<monster.attk-User.def<<" damage";
        }
    }
    else{
        second<<"You received no damage";
    }
    User.uninitialize_stats();
    process_gear(User, User.equip.helmet, monster.attk);
    process_gear(User, User.equip.chestplate, monster.attk);
    process_gear(User, User.equip.greaves, monster.attk);
    process_gear(User, User.equip.boots, monster.attk);
    process_gear(User, User.equip.shield, monster.attk);
    process_gear(User, User.equip.weapon, monster.attk);
    User.initialize_stats();
    return {first.str(),second.str()};
}
bool player_battle(WINDOW *main_win, WINDOW *status_win, Player &User, level Current, char monster_type){
    monster_stats monster=create_monster(Current, monster_type);
    std::pair<std::string,std::string> log;
    while(true){
        wclear(main_win);
        draw_border(main_win);
        std::stringstream user_output;
        std::stringstream enemy_output;
        draw_stats(status_win, User);
        user_output<<User.name<<"~ HP:"<<User.cur_hp<<" Defence:"<<User.def<<" Shield:"<<User.cur_shield<<" Heal:"<<User.inv.heal_amount;
        enemy_output<<"Enemy~ HP:"<<monster.hp<<" Attk:"<<monster.attk<<" Def:"<<monster.def;
        mvwaddstr(main_win, 48, 1, user_output.str().c_str());
        mvwaddstr(main_win, 1, 1, enemy_output.str().c_str());
        mvwaddstr(main_win,24,10,log.first.c_str());
        mvwaddstr(main_win,25,10,log.second.c_str());
        int ch=wgetch(main_win);
        if(ch=='1'){
            log=calculate_damage(User, monster);
            if(User.cur_hp<=0){
                return false;
            }
            if(monster.hp<=0){
                User.gold+=generate_gold(monster_type);
                Item loot=generate_loot_from_monster_type(monster_type);
                wclear(main_win);
                mvwaddstr(main_win, 0, 0, "Press any key to keep and [r] to trash");
                print_description(main_win, &loot, 1);
                draw_stats(status_win, User);
                int ch=wgetch(main_win);
                if(ch=='r'){
                    return true;
                }
                User.add_item(loot);
                return true;
            }
        }
        if(ch=='2'){
            if(User.inv.heal_amount>0&&User.cur_hp!=User.ori_hp){
                User.inv.heal_amount--;
                User.cur_hp=User.ori_hp;
            }
        }
        wrefresh(main_win);
    }
    return false;
}
std::pair<bool,bool> attack_monster(WINDOW *main_win, WINDOW *status_win, std::vector<monster> &monsters, Csr csr_pos, Player &User, level Current){
    std::pair<bool,bool> attack_status{false,false}; // .first = if surroundings have monsters, .second is_alive
    std::pair<int,char> pos;
    if(check_surroundings(monsters, csr_pos.first-1, csr_pos.second)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first-1, csr_pos.second);
        attack_status.second=player_battle(main_win, status_win, User, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    if(check_surroundings(monsters, csr_pos.first+1, csr_pos.second)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first+1, csr_pos.second);
        attack_status.second=player_battle(main_win, status_win, User, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    if(check_surroundings(monsters, csr_pos.first, csr_pos.second-1)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first, csr_pos.second-1);
        attack_status.second=player_battle(main_win, status_win, User, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    if(check_surroundings(monsters, csr_pos.first, csr_pos.second+1)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first, csr_pos.second+1);
        attack_status.second=player_battle(main_win, status_win, User, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    return{attack_status};
}
void move_door(std::vector<std::pair<int,int>> &doors, std::vector<monster> &monsters, level &Current, Csr &csr_pos){
    for(int i=0; i<doors.size(); i++){
        if(doors[i].first==csr_pos.first&&doors[i].second==csr_pos.second-1){
            ++Current.y;
            csr_pos={39,48};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first&&doors[i].second==csr_pos.second+1){
            --Current.y;
            csr_pos={39,1};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first+1&&doors[i].second==csr_pos.second){
            ++Current.x;
            csr_pos={1,24};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first-1&&doors[i].second==csr_pos.second){
            --Current.x;
            csr_pos={78,24};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
    }
}
bool move_staircase(level &Current, Csr csr_pos){
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
void print_food(WINDOW *main_win, Player &User){
    wclear(main_win);
    std::stringstream ss;
    ss<<"[1] Bread (30 Saturation Points): "<<User.inv.food.bread;
    mvwaddstr(main_win,0,0,ss.str().c_str());
    ss.str(std::string());
    ss<<"[2] Waffle (50 Saturation Points): "<<User.inv.food.waffle;
    mvwaddstr(main_win,1,0,ss.str().c_str());
    ss.str(std::string());
    ss<<"[3] Energy Bar (100 Saturation Points): "<<User.inv.food.energy_bar;
    mvwaddstr(main_win,2,0,ss.str().c_str());
    ss.str(std::string());
    ss<<"[4] Water (50 Hydration Points): "<<User.inv.water.water;
    mvwaddstr(main_win,3,0,ss.str().c_str());
    ss.str(std::string());
    ss<<"[5] Sparkling Water (100 Hydration Points): "<<User.inv.water.sparkling_juice;
    mvwaddstr(main_win,4,0,ss.str().c_str());
    ss.str(std::string());
    wrefresh(main_win);
}
void eat_drink_mode(WINDOW *main_win, WINDOW *status_win, Player &User){
    print_food(main_win, User);
    while(true){
        draw_stats(status_win, User);
        print_food(main_win, User);
        wrefresh(status_win);
        int ch=wgetch(main_win);
        if(ch=='1'&&User.inv.food.bread>0){
            User.eat(&User.inv.food.bread);
        }
        if(ch=='2'&&User.inv.food.waffle>0){
            User.eat(&User.inv.food.waffle);
        }
        if(ch=='3'&&User.inv.food.energy_bar>0){
            User.eat(&User.inv.food.energy_bar);
        }
        if(ch=='4'&&User.inv.water.water>0){
            User.drink(&User.inv.water.water);
        }
        if(ch=='5'&&User.inv.water.sparkling_juice>0){
            User.drink(&User.inv.water.sparkling_juice);
        }
        if(ch=='q'){
            return;
        }
    }
    draw_stats(status_win, User);
}
void end_program(int sig){
    endwin();
    if(sig==0){
        std::cout<<"Nya~"<<std::endl;
        std::ifstream cosmos_ascii("res/cosmos.txt");
        std::string line;
        while(std::getline(cosmos_ascii, line)){
            std::cout<<line<<std::endl;
        }
        return;
    }
    if(sig==1){
        std::ifstream tombstone_file("res/tombstone.txt");
        std::string line;
        while(std::getline(tombstone_file,line)){
            std::cout<<line<<std::endl;
        }
        return;
    }
    if(sig==2){
        std::cout<<"*Grumbling* You died of hunger... You regret being on a diet while fighting monsters. (Seriously, dieting exccesively is bad for your health)"<<std::endl;
        return;
    }
    if(sig==3){
        std::cout<<"*Cough* *Cough* You died of thirst... Next time, remember to join the HydroHomies. (Remember to drink water bro)"<<std::endl;
    }
}
void end_program(int sig, std::string error){
    endwin();
    if(sig==-1){
        std::cout<<"Bzzt- Something fatal has occurred- Error rep.r..d....!@#$%^&*~__+|:<? *Developer cries in the background*"<<std::endl;
        std::cerr<<error<<std::endl;
        return;
    }
}
bool is_empty(std::ifstream& pFile){
    return pFile.peek() == std::ifstream::traits_type::eof();
}
void init_data(Player &User, level &Current, Csr &csr_pos, std::vector<monster> &monsters, NPC &npc){
    std::ifstream ifile("save/user.save",std::ios::binary);
    if(!is_empty(ifile)){
        cereal::PortableBinaryInputArchive retrieve(ifile);
        retrieve(User,Current,csr_pos,monsters,npc);
        // Insert data corruption checks
        std::filesystem::remove("save/user.save.1");
        std::filesystem::copy("save/user.save","save/user.save.1");
    }
}
void save_data(Player User, level Current, Csr csr_pos, std::vector<monster> monsters, NPC npc){
    std::ofstream ofile("save/user.save",std::ios::trunc|std::ios::binary);
    cereal::PortableBinaryOutputArchive archive(ofile);
    User.uninitialize_stats();
    archive(User,Current,csr_pos,monsters,npc);
}
void drop_items_on_death(Player &User, Csr &csr_pos, level &current){
    User=Player();
    csr_pos={1,1};
    current={1,1,1};
}
void init_dungeon(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, Csr &csr_pos, Player &User, level &Current, std::vector<monster> &monsters, NPC &npc){
    std::vector<std::pair<int,int>> doors;
    if(monsters.empty()){
        generate_monsters(monsters, Current, csr_pos);
    }
    generate_doors(doors, Current);
    draw_level(interaction_bar, Current);
    draw_stats(status_win, User);
    while(true){
        redraw_everything(main_win, status_win, interaction_bar, csr_pos, User, Current, monsters);
        int ch=wgetch(main_win);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==KEY_LEFT||ch==KEY_RIGHT||ch==KEY_DOWN||ch==KEY_UP){
            char_move(main_win, ch, csr_pos, monsters, User);
            redraw_dungeon(main_win, Current, monsters, csr_pos);
            draw_stats(status_win, User);
        }
        if(ch=='z'){
            move_door(doors, monsters, Current, csr_pos);
            redraw_everything(main_win, status_win, interaction_bar, csr_pos, User, Current, monsters);
        }
        if(ch=='x'){
            std::pair<bool,bool> attack_status=attack_monster(main_win, status_win, monsters, csr_pos, User, Current);
            if(attack_status.first&&!attack_status.second){ // If dead return to main menu
                end_program(1);
                drop_items_on_death(User,csr_pos,Current);
                save_data(User, Current, csr_pos, monsters, npc);
                return;
            }
            if(attack_status.first&&attack_status.second){ // If win redraw dungeon and move on
                User.cur_shield=User.ori_shield;
                redraw_dungeon(main_win, Current, monsters, csr_pos);
            }
        }
        if(ch=='c'){
            if(move_staircase(Current, csr_pos)){
                generate_doors(doors, Current);
                redraw_everything(main_win, status_win, interaction_bar, csr_pos, User, Current, monsters);
            }
            else if(Current.lvl==1&&Current.x==1&&Current.y==1&&csr_pos.first==1&&csr_pos.second==48){
                bar_mode(main_win, status_win, interaction_bar, User, npc);
            }
        }
        if(ch=='r'){
            redraw_everything(main_win, status_win, interaction_bar, csr_pos, User, Current, monsters);
        }
        if(ch=='i'){
            if(!User.inv.item.empty()){
                inventory_mode(main_win, status_win, interaction_bar, User);
                redraw_everything(main_win, status_win, interaction_bar, csr_pos, User, Current, monsters);
            }
        }
        if(ch=='e'){
            eat_drink_mode(main_win, status_win, User);
        }
        if(ch=='.'){
            if(User.cur_hp<User.ori_hp){
                User.saturation-=1;
                User.cur_hp+=(User.ori_hp*0.015);
            }
            if(User.cur_hp>User.ori_hp){
                User.cur_hp=User.ori_hp;
            }
        }
        if(ch=='S'){
            save_data(User, Current, csr_pos, monsters, npc);
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"Data saved successfully!");
            wrefresh(interaction_bar);
            sleep(1);
        }
        if(ch=='H'){
            help_mode(main_win, interaction_bar, "dungeon_mode");
        }
        if(ch=='q'){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"Do you really wish to quit? [y] to quit, any other key to abort.");
            wrefresh(interaction_bar);
            ch=wgetch(main_win);
            if(ch=='y'){
                end_program(0);
                save_data(User, Current, csr_pos, monsters, npc);
                return;
            }
        }
        if(User.saturation<=0){
            end_program(2);
            drop_items_on_death(User,csr_pos,Current);
            save_data(User, Current, csr_pos, monsters, npc);
            return;
        }
        if(User.hydration<=0){
            end_program(3);
            drop_items_on_death(User,csr_pos,Current);
            save_data(User, Current, csr_pos, monsters, npc);
            return;
        }
    }
}
void init(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, Csr &csr_pos, Player &User, level &Current, NPC &npc){
    std::vector<monster> monsters;
    init_data(User, Current, csr_pos, monsters, npc);
    User.init();
    std::ifstream ascii_wayfarer("res/wayfarer.txt");
    if(!ascii_wayfarer){
        end_program(-1, "wayfarer.txt not found!");
        return;
    }
    int win_iterator=0;
    std::string line;
    while(std::getline(ascii_wayfarer, line)){
        mvwaddstr(main_win, win_iterator, 1, line.c_str());
        win_iterator++;
    }
    mvwaddstr(main_win, 10, 1, "A simple old school dungeon adventure with endless posibilities...");
    mvwaddstr(main_win, 20, 15, "Press any key to continue...");
    wrefresh(main_win);
    ascii_wayfarer.close();
    getchar();
    init_dungeon(main_win, status_win, interaction_bar, csr_pos, User, Current, monsters, npc);
}
int main(int argc, char *argv[]){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    WINDOW *main_win=newwin(50, 80, 1, 0); // 50 rows, 80 columns
    WINDOW *status_win=newwin(3, 100, 51, 0); // Status bar
    WINDOW *interaction_bar=newwin(1, 200, 0, 0);
    Player User;
    level Current{1,1,1};
    NPC npc;
    keypad(main_win, TRUE);
    Csr csr_pos{1,1}; // x, y
    if(has_colors()){
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK); // Player
        init_pair(2, COLOR_RED, COLOR_BLACK); // enemy
        init_pair(3, COLOR_WHITE, COLOR_BLACK); // boss
        init_pair(4, COLOR_YELLOW, COLOR_BLACK); // loot
        init_pair(5, COLOR_WHITE, COLOR_BLACK); // common
        init_pair(6, COLOR_GREEN, COLOR_BLACK); // uncommon & grass
        init_pair(7, COLOR_BLUE, COLOR_BLACK); // rare & water
        init_pair(8, COLOR_MAGENTA, COLOR_BLACK); // epic
        init_pair(9, COLOR_YELLOW, COLOR_BLACK); // legendary
        init_pair(10, COLOR_RED, COLOR_BLACK); // artifact
        init_pair(11, COLOR_BLACK, COLOR_WHITE); // doors
        init_pair(12, COLOR_MAGENTA, COLOR_BLACK); // special doors & NPCs
        init_pair(13, COLOR_BLACK, COLOR_YELLOW); // level boss
        init_pair(14, COLOR_WHITE, COLOR_MAGENTA); // final boss
        init_pair(15, COLOR_BLACK, COLOR_WHITE); // common inverted
        init_pair(16, COLOR_BLACK, COLOR_GREEN); // uncommon inverted
        init_pair(17, COLOR_BLACK, COLOR_BLUE); // rare inverted
        init_pair(18, COLOR_BLACK, COLOR_MAGENTA); // epic inverted
        init_pair(19, COLOR_BLACK, COLOR_YELLOW); // legendary inverted
        init_pair(20, COLOR_BLACK, COLOR_RED); // artifact inverted
        init(main_win, status_win, interaction_bar, csr_pos, User, Current, npc);
    }
    else{
        endwin();
        throw std::runtime_error("This program requires colors to function properly.");
    }
    return 0;

    /*
     *    init_extended_color(16,372,843,0);
     *    init_extended_pair(15,16,COLOR_BLACK);
     *    wattron(main_win,COLOR_PAIR(15));
     *    mvwaddstr(main_win,0,0,"Test");
     *    wattroff(main_win,COLOR_PAIR(15));
     *    wrefresh(main_win);
     */
}
