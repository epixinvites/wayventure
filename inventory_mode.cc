#include <curses.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "headers/classes.h"
#include "headers/mode.h"
#include "headers/draw.h"
#include "headers/generate.h"
void clear_screen(WINDOW *main_win){
    wclear(main_win);
    wrefresh(main_win);
}
void clear_screen(WINDOW *main_win, WINDOW *interaction_bar){
    wclear(main_win);
    wclear(interaction_bar);
    wrefresh(main_win);
    wrefresh(interaction_bar);
}
void draw_base(WINDOW *main_win, WINDOW *interaction_bar, int y, unsigned int size, unsigned int page){
    std::stringstream ss;
    wclear(interaction_bar);
    ss<<"Inventory "<<"("<<page+1<<"/"<<((size-1)/30)+1<<")";
    mvwaddstr(interaction_bar, 0, 0, ss.str().c_str());
    for(int i=0; i<80; i++){
        mvwaddch(main_win, y, i, '-');
    }
    wrefresh(interaction_bar);
    wrefresh(main_win);
}
void print_string_with_color(WINDOW *main_win, std::string ss, int color_pair, int line){
    wattron(main_win, COLOR_PAIR(color_pair));
    mvwaddstr(main_win, line, 0, ss.c_str());
    wattroff(main_win, COLOR_PAIR(color_pair));
}
void print_string_with_color(WINDOW *main_win, std::string ss, int color_pair, int y, int x){
    wattron(main_win, COLOR_PAIR(color_pair));
    mvwaddstr(main_win, y, x, ss.c_str());
    wattroff(main_win, COLOR_PAIR(color_pair));
}
void print_item(WINDOW *main_win, const Item *cur_item, int line){
    std::stringstream ss;
    if(cur_item->is_equipped){
        ss<<"*";
    }
    switch(cur_item->type){
        case 'h':
            ss<<"[Helmet] ";
            break;
        case 'c':
            ss<<"[Chestplate] ";
            break;
        case 'g':
            ss<<"[Greaves] ";
            break;
        case 'b':
            ss<<"[Boots] ";
            break;
        case 's':
            ss<<"[Shield] ";
            break;
        case 'w':
            ss<<"[Weapon] ";
            break;
        default:
            mvwaddstr(main_win, line, 0, "Error");
            return;
    }
    ss<<cur_item->name;
    if(cur_item->calibration>0){
        ss<<" ["<<cur_item->calibration<<"]";
    }
    switch(cur_item->rarity){
        case 'c':
            print_string_with_color(main_win, ss.str(), 5, line);
            break;
        case 'u':
            print_string_with_color(main_win, ss.str(), 6, line);
            break;
        case 'r':
            print_string_with_color(main_win, ss.str(), 7, line);
            break;
        case 'e':
            print_string_with_color(main_win, ss.str(), 8, line);
            break;
        case 'l':
            print_string_with_color(main_win, ss.str(), 9, line);
            break;
        case 'a':
            print_string_with_color(main_win, ss.str(), 10, line);
            break;
        default:
            mvwaddstr(main_win, line, 0, "Error");
            break;
    }
}
void print_bold_item(WINDOW *main_win, const Item *cur_item, int line){
    std::stringstream ss;
    if(cur_item->is_equipped){
        ss<<"*";
    }
    switch(cur_item->type){
        case 'h':
            ss<<"[Helmet] ";
            break;
        case 'c':
            ss<<"[Chestplate] ";
            break;
        case 'g':
            ss<<"[Greaves] ";
            break;
        case 'b':
            ss<<"[Boots] ";
            break;
        case 's':
            ss<<"[Shield] ";
            break;
        case 'w':
            ss<<"[Weapon] ";
            break;
        default:
            mvwaddstr(main_win, line, 0, "Error");
            return;
    }
    ss<<cur_item->name;
    if(cur_item->calibration>0){
        ss<<" ["<<cur_item->calibration<<"]";
    }
    switch(cur_item->rarity){
        case 'c':
            print_string_with_color(main_win, ss.str(), 15, line);
            break;
        case 'u':
            print_string_with_color(main_win, ss.str(), 16, line);
            break;
        case 'r':
            print_string_with_color(main_win, ss.str(), 17, line);
            break;
        case 'e':
            print_string_with_color(main_win, ss.str(), 18, line);
            break;
        case 'l':
            print_string_with_color(main_win, ss.str(), 19, line);
            break;
        case 'a':
            print_string_with_color(main_win, ss.str(), 20, line);
            break;
        default:
            mvwaddstr(main_win, line, 0, "Error");
            break;
    }
}
void print_description(WINDOW *main_win, const Item *cur_item, int line){
    mvwaddstr(main_win, line, 0, "Name: ");
    mvwaddstr(main_win, line+1, 0, "Rarity: ");
    mvwaddstr(main_win, line, 6, cur_item->name.c_str());
    switch(cur_item->rarity){
        case 'c':
            print_string_with_color(main_win, "Common", 5, line+1, 8);
            break;
        case 'u':
            print_string_with_color(main_win, "Uncommon", 6, line+1, 8);
            break;
        case 'r':
            print_string_with_color(main_win, "Rare", 7, line+1, 8);
            break;
        case 'e':
            print_string_with_color(main_win, "Epic", 8, line+1, 8);
            break;
        case 'l':
            print_string_with_color(main_win, "Legendary", 9, line+1, 8);
            break;
        case 'a':
            print_string_with_color(main_win, "Artifact", 10, line+1, 8);
            break;
        default:
            mvwaddstr(main_win, line, 0, "-Error-");
            break;
    }
    line+=2;
    mvwaddstr(main_win, line, 0, "Type: ");
    switch(cur_item->type){
        case 'h':
            mvwaddstr(main_win, line, 6, "Helmet");
            break;
        case 'c':
            mvwaddstr(main_win, line, 6, "Chestplate");
            break;
        case 'g':
            mvwaddstr(main_win, line, 6, "Greaves");
            break;
        case 'b':
            mvwaddstr(main_win, line, 6, "Boots");
            break;
        case 's':
            mvwaddstr(main_win, line, 6, "Shield");
            break;
        case 'w':
            mvwaddstr(main_win, line, 6, "Weapon");
            break;
        default:
            mvwaddstr(main_win, line, 0, "-Error-");
            return;
    }
    line++;
    if(cur_item->is_equipped){
        wattron(main_win, COLOR_PAIR(6));
        mvwaddstr(main_win, line, 0, "Equipped: true");
        wattroff(main_win, COLOR_PAIR(6));
    }
    if(!cur_item->is_equipped){
        wattron(main_win, COLOR_PAIR(10));
        mvwaddstr(main_win, line, 0, "Equipped: false");
        wattroff(main_win, COLOR_PAIR(10));
    }
    line++;
    std::stringstream ss;
    mvwaddstr(main_win,line,0,("Durability: "+std::to_string(cur_item->durability)+"%").c_str());
    line++;
    if(cur_item->uses>0){
        mvwaddstr(main_win, line, 0, ("Uses: "+std::to_string(cur_item->uses)).c_str());
        line++;
    }
    if(cur_item->calibration>0){
        mvwaddstr(main_win, line, 0, ("Calibration Level: "+std::to_string(cur_item->calibration)).c_str());
        line++;
    }
    if(cur_item->enhancement>0){
        mvwaddstr(main_win, line, 0, ("Enhancement: "+std::to_string(cur_item->enhancement)).c_str());
        line++;
    }
    if(cur_item->hp>0){
        mvwaddstr(main_win, line, 0, ("Health: "+std::to_string(cur_item->hp)).c_str());
        line++;
    }
    if(cur_item->attk>0){
        mvwaddstr(main_win, line, 0, ("Damage: "+std::to_string(cur_item->attk)).c_str());
        line++;
    }
    if(cur_item->def>0){
        mvwaddstr(main_win, line, 0, ("Defence: "+std::to_string(cur_item->def)).c_str());
        line++;
    }
    if(cur_item->shield>0){
        mvwaddstr(main_win, line, 0, ("Shield: "+std::to_string(cur_item->shield)).c_str());
        line++;
    }
    if(cur_item->crit_chance>0){
        mvwaddstr(main_win, line, 0, ("Critical Chance: "+std::to_string(cur_item->crit_chance)+"%").c_str());
        line++;
    }
    if(cur_item->crit_dmg>0){
        mvwaddstr(main_win, line, 0, ("Critical Damage: "+std::to_string(cur_item->crit_dmg)+"%").c_str());
        line++;
    }
    wrefresh(main_win);
}
void equip_item(Player &User, unsigned int csr_pos, unsigned int page_num){
    User.inv.item[csr_pos+(page_num*30)].is_equipped=true;
    if(User.inv.item[csr_pos+(page_num*30)].type=='h'){
        if(User.equip.helmet!=nullptr){
            User.uninitialize_gear(User.equip.helmet);
            User.equip.helmet->is_equipped=false;
        }
        User.equip.helmet=&User.inv.item[csr_pos+(page_num*30)];
        User.initialize_gear(User.equip.helmet);
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='c'){
        if(User.equip.chestplate!=nullptr){
            User.uninitialize_gear(User.equip.chestplate);
            User.equip.chestplate->is_equipped=false;
        }
        User.equip.chestplate=&User.inv.item[csr_pos+(page_num*30)];
        User.initialize_gear(User.equip.chestplate);
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='g'){
        if(User.equip.greaves!=nullptr){
            User.uninitialize_gear(User.equip.greaves);
            User.equip.greaves->is_equipped=false;
        }
        User.equip.greaves=&User.inv.item[csr_pos+(page_num*30)];
        User.initialize_gear(User.equip.greaves);
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='b'){
        if(User.equip.boots!=nullptr){
            User.uninitialize_gear(User.equip.boots);
            User.equip.boots->is_equipped=false;
        }
        User.equip.boots=&User.inv.item[csr_pos+(page_num*30)];
        User.initialize_gear(User.equip.boots);
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='s'){
        if(User.equip.shield!=nullptr){
            User.uninitialize_gear(User.equip.shield);
            User.equip.shield->is_equipped=false;
        }
        User.equip.shield=&User.inv.item[csr_pos+(page_num*30)];
        User.initialize_gear(User.equip.shield);
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='w'){
        if(User.equip.weapon!=nullptr){
            User.uninitialize_gear(User.equip.weapon);
            User.equip.weapon->is_equipped=false;
        }
        User.equip.weapon=&User.inv.item[csr_pos+(page_num*30)];
        User.initialize_gear(User.equip.weapon);
    }
}
void unequip_item(Player &User, unsigned int csr_pos, unsigned int page_num){
    if(User.inv.item[csr_pos+(page_num*30)].type=='h'&&User.cur_hp>User.inv.item[csr_pos+(page_num*30)].hp){
        User.uninitialize_gear(User.equip.helmet);
        User.equip.helmet=nullptr;
        User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='c'&&User.cur_hp>User.inv.item[csr_pos+(page_num*30)].hp){
        User.uninitialize_gear(User.equip.chestplate);
        User.equip.chestplate=nullptr;
        User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='g'){
        User.uninitialize_gear(User.equip.greaves);
        User.equip.greaves=nullptr;
        User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='b'){
        User.uninitialize_gear(User.equip.boots);
        User.equip.boots=nullptr;
        User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='s'){
        User.uninitialize_gear(User.equip.shield);
        User.equip.shield=nullptr;
        User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='w'){
        User.uninitialize_gear(User.equip.weapon);
        User.equip.weapon=nullptr;
        User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    }
}
void draw_inventory(WINDOW *main_win, WINDOW *interaction_bar, WINDOW *status_win, const Player &User, unsigned int page_num, unsigned int csr_pos){
    clear_screen(main_win);
    draw_base(main_win, interaction_bar, 34, User.inv.item.size(), page_num);
    draw_stats(status_win, User);
    for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++, iterator++){
        print_item(main_win, &User.inv.item[i], iterator);
    }
    print_description(main_win, &User.inv.item[page_num*30+csr_pos], 35);
    print_bold_item(main_win, &User.inv.item[page_num*30+csr_pos], csr_pos);
    wrefresh(main_win);
    wrefresh(interaction_bar);
}
std::string get_string(WINDOW *main_win, WINDOW *interaction_bar, std::string original){
    std::string input;
    wclear(interaction_bar);
    mvwaddstr(interaction_bar,0,0,"([ESC] to cancel) Rename item to: ");
    wrefresh(interaction_bar);
    curs_set(1);
    char ch;
    while(true){
        ch=wgetch(main_win);
        if(ch==10&&input.length()>0){ // KEY_ENTER
            curs_set(0);
            return input;
        }
        if(ch==7){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        if(ch==27){ // KEY_ESC
            curs_set(0);
            return original;
        }
        else if(input.length()<30&&(isdigit(ch)||isalpha(ch)||isblank(ch))){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss<<"Rename item to: "<<input;
        wclear(interaction_bar);
        mvwaddstr(interaction_bar,0,0,ss.str().c_str());
        wrefresh(interaction_bar);
    }
}
unsigned int get_int(WINDOW *main_win, WINDOW *interaction_bar, std::string dialogue){
    std::string input;
    wclear(interaction_bar);
    mvwaddstr(interaction_bar,0,0,dialogue.c_str());
    wrefresh(interaction_bar);
    curs_set(1);
    char ch;
    while(true){
        ch=wgetch(main_win);
        if(ch==10&&input.length()>0){ // KEY_ENTER
            curs_set(0);
            return std::stoi(input);
        }
        if(ch==7){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        if(ch==27||ch=='q'){ // KEY_ESC
            curs_set(0);
            return 0;
        }
        else if(input.length()<8&&isdigit(ch)){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss<<dialogue<<input;
        wclear(interaction_bar);
        mvwaddstr(interaction_bar,0,0,ss.str().c_str());
        wrefresh(interaction_bar);
    }
}
void print_misc_bold(WINDOW *main_win, Miscellaneous &User, unsigned int csr_pos){
    switch (csr_pos){
        case 0:
            print_string_with_color(main_win, "Ancient Cores: "+std::to_string(User.ancient_core), 20, 0);
            break;
        case 1:
            print_string_with_color(main_win, "Crystallium: "+std::to_string(User.crystallium), 19, 1);
            break;
        case 4:
            print_string_with_color(main_win, "Common: "+std::to_string(User.materials.common), 15, 4);
            break;
        case 5:
            print_string_with_color(main_win, "Uncommon: "+std::to_string(User.materials.uncommon), 16, 5);
            break;
        case 6:
            print_string_with_color(main_win, "Rare: "+std::to_string(User.materials.rare), 17, 6);
            break;
        case 7:
            print_string_with_color(main_win, "Epic: "+std::to_string(User.materials.epic), 18, 7);
            break;
        case 8:
            print_string_with_color(main_win, "Legendary: "+std::to_string(User.materials.legendary), 19, 8);
            break;
        case 9:
            print_string_with_color(main_win, "Artifact: "+std::to_string(User.materials.artifact), 20, 9);
            break;
        case 12:
            print_string_with_color(main_win, ("Helmet: "+std::to_string(User.blueprint.helmet)).c_str(), 15, 12);
            break;
        case 13:
            print_string_with_color(main_win, ("Chestplate: "+std::to_string(User.blueprint.chestplate)).c_str(), 15, 13);
            break;
        case 14:
            print_string_with_color(main_win, ("Greaves: "+std::to_string(User.blueprint.greaves)).c_str(), 15, 14);
            break;
        case 15:
            print_string_with_color(main_win, ("Boots: "+std::to_string(User.blueprint.boots)).c_str(), 15, 15);
            break;
        case 16:
            print_string_with_color(main_win, ("Shield: "+std::to_string(User.blueprint.shield)).c_str(), 15, 16);
            break;
        case 17:
            print_string_with_color(main_win, ("Weapon: "+std::to_string(User.blueprint.weapon)).c_str(), 15, 17);
            break;
        default:
            print_string_with_color(main_win, "Invalid cursor position", 10, csr_pos);
    }
}
bool decrease_amount(unsigned int &original, unsigned int amount){
    if(original>=amount){
        original-=amount;
        return true;
    }
    else{
        return false;
    }
}
bool remove_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount){
    if(amount==0){
        return false;
    }
    switch (csr_pos){
        case 0:
            return decrease_amount(User.ancient_core,amount);
        case 1:
            return decrease_amount(User.crystallium,amount);
        case 4:
            return decrease_amount(User.materials.common,amount);
        case 5:
            return decrease_amount(User.materials.uncommon,amount);
        case 6:
            return decrease_amount(User.materials.rare,amount);
        case 7:
            return decrease_amount(User.materials.epic,amount);
        case 8:
            return decrease_amount(User.materials.legendary,amount);
        case 9:
            return decrease_amount(User.materials.artifact,amount);
        case 12:
            return decrease_amount(User.blueprint.helmet,amount);
        case 13:
            return decrease_amount(User.blueprint.chestplate,amount);
        case 14:
            return decrease_amount(User.blueprint.greaves,amount);
        case 15:
            return decrease_amount(User.blueprint.boots,amount);
        case 16:
            return decrease_amount(User.blueprint.shield,amount);
        case 17:
            return decrease_amount(User.blueprint.weapon,amount);
        default:
            return false;
    }
}
void print_misc_item(WINDOW *main_win, Miscellaneous &User, unsigned int csr_pos){
    wclear(main_win);
    print_string_with_color(main_win, "Ancient Cores: "+std::to_string(User.ancient_core), 10, 0);
    print_string_with_color(main_win, "Crystallium: "+std::to_string(User.crystallium), 9, 1);
    mvwaddstr(main_win,3,0,"Materials:");
    print_string_with_color(main_win, "Common: "+std::to_string(User.materials.common), 5, 4);
    print_string_with_color(main_win, "Uncommon: "+std::to_string(User.materials.uncommon), 6, 5);
    print_string_with_color(main_win, "Rare: "+std::to_string(User.materials.rare), 7, 6);
    print_string_with_color(main_win, "Epic: "+std::to_string(User.materials.epic), 8, 7);
    print_string_with_color(main_win, "Legendary: "+std::to_string(User.materials.legendary), 9, 8);
    print_string_with_color(main_win, "Artifact: "+std::to_string(User.materials.artifact), 10, 9);
    mvwaddstr(main_win,11,0,"Blueprints:");
    mvwaddstr(main_win,12,0,("Helmet: "+std::to_string(User.blueprint.helmet)).c_str());
    mvwaddstr(main_win,13,0,("Chestplate: "+std::to_string(User.blueprint.chestplate)).c_str());
    mvwaddstr(main_win,14,0,("Greaves: "+std::to_string(User.blueprint.greaves)).c_str());
    mvwaddstr(main_win,15,0,("Boots: "+std::to_string(User.blueprint.boots)).c_str());
    mvwaddstr(main_win,16,0,("Shield: "+std::to_string(User.blueprint.shield)).c_str());
    mvwaddstr(main_win,17,0,("Weapon: "+std::to_string(User.blueprint.weapon)).c_str());
    print_misc_bold(main_win, User, csr_pos);
    wrefresh(main_win);
}
void show_misc_items(WINDOW *main_win, WINDOW *interaction_bar, Miscellaneous &User){
    unsigned int csr_pos = 0;
    wclear(interaction_bar);
    mvwaddstr(interaction_bar,0,0,"Miscallaneous items:");
    wrefresh(interaction_bar);
    while(true){
        print_misc_item(main_win, User, csr_pos);
        int ch=wgetch(main_win);
        if((ch==KEY_DOWN||ch=='s')&&csr_pos<17){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"Miscallaneous items:");
            wrefresh(interaction_bar);
            if(csr_pos==1||csr_pos==9){
                csr_pos+=3;
            }
            else{
                csr_pos++;
            }
        }
        if((ch==KEY_UP||ch=='w')&&csr_pos>0){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"Miscallaneous items:");
            wrefresh(interaction_bar);
            if(csr_pos==4||csr_pos==12){
                csr_pos-=3;
            }
            else{
                csr_pos--;
            }
        }
        if(ch=='r'){
            unsigned int quantity = get_int(main_win, interaction_bar,"([ESC] to cancel) Quantity to be trashed: ");
            wclear(interaction_bar);
            if(!remove_misc_item(User, csr_pos, quantity)){
                mvwaddstr(interaction_bar,0,0,"[System] Invalid amount");
            }
            wrefresh(interaction_bar);
        }
        if(ch=='q'||ch=='m'){
            return;
        }
    }
}
void inventory_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, Player &User){
    unsigned int page_num=0;
    unsigned int csr_pos=0;
    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
    while(true){
        int ch=wgetch(main_win);
        if((ch=='s'||ch==KEY_DOWN)&&((csr_pos+page_num*30)<User.inv.item.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if((ch=='w'||ch==KEY_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='a'||ch==KEY_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
            }
        }
        if(ch=='d'||ch==KEY_RIGHT){
            if((page_num+1)*30<User.inv.item.size()){
                page_num++;
                csr_pos=0;
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
            }
        }
        if(ch=='e'){
            if(!User.inv.item[csr_pos+(page_num*30)].is_equipped&&User.inv.item[csr_pos+(page_num*30)].durability>0){ // if there current item is unequipped
                equip_item(User, csr_pos, page_num);
            }
            else if(User.inv.item[csr_pos+(page_num*30)].is_equipped){
                unequip_item(User, csr_pos, page_num);
            }
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='r'){
            std::string input = get_string(main_win, interaction_bar, User.inv.item[csr_pos+(page_num*30)].name);
            User.inv.item[csr_pos+(page_num*30)].name = input;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='m'){
            show_misc_items(main_win, interaction_bar, User.inv.misc);
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='H'){
            help_mode(main_win,interaction_bar,"inventory_mode");
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='R'){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar, 0, 0, "[System] Are you sure you want to delete this item? Press y to continue, any other key to cancel.");
            wrefresh(interaction_bar);
            ch=wgetch(main_win);
            if(ch=='y'){
                unequip_item(User, csr_pos, page_num);
                User.remove_item(csr_pos+(page_num*30));
                if((page_num*30+csr_pos)>=User.inv.item.size()){csr_pos--;}
                if(csr_pos<0){
                    csr_pos=0;
                }
                if(User.inv.item.empty()){
                    return;
                }
            }
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='q'){
            return;
        }
    }
}
bool decrease_materials(char rarity, Miscellaneous &misc, unsigned int amount){
    if(rarity=='c'&&misc.materials.common>=amount){
        misc.materials.common-=amount;
        return true;
    }
    else if(rarity=='u'&&misc.materials.uncommon>=amount){
        misc.materials.uncommon-=amount;
        return true;
    }
    else if(rarity=='r'&&misc.materials.rare>=amount){
        misc.materials.rare-=amount;
        return true;
    }
    else if(rarity=='e'&&misc.materials.epic>=amount){
        misc.materials.epic-=amount;
        return true;
    }
    else if(rarity=='l'&&misc.materials.legendary>=amount){
        misc.materials.legendary-=amount;
        return true;
    }
    else if(rarity=='a'&&misc.materials.artifact>=amount){
        misc.materials.artifact-=amount;
        return true;
    }
    return false;
}
bool enhance_item(Item &item, Miscellaneous &misc, unsigned int amount){
    bool require_enhance = false;
    if(decrease_materials(item.rarity, misc, amount)){
        item.enhancement+=amount;
        item.reinitialize_item();
        return true;
    }
    return false;
}
void reforge_repair_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, Player &User){
    unsigned int page_num=0;
    unsigned int csr_pos=0;
    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
    while(true){
        int ch=wgetch(main_win);
        if((ch=='s'||ch==KEY_DOWN)&&((csr_pos+page_num*30)<User.inv.item.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if((ch=='w'||ch==KEY_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='a'||ch==KEY_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
            }
        }
        if(ch=='d'||ch==KEY_RIGHT){
            if((page_num+1)*30<User.inv.item.size()){
                page_num++;
                csr_pos=0;
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
                wrefresh(main_win);
            }
        }
        if(ch=='m'){
            show_misc_items(main_win, interaction_bar, User.inv.misc);
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='r'){
            wclear(interaction_bar);
            if(User.inv.item[csr_pos+(page_num*30)].durability<100.0){
                const unsigned int cost = (100.0-User.inv.item[csr_pos+(page_num*30)].durability)*rarity_value(User.inv.item[csr_pos+(page_num*30)].rarity)*15;
                std::stringstream ss;
                ss<<"[System] Attempt repair? Cost: "<<cost<< " [y/n]";
                mvwaddstr(interaction_bar,0,0,ss.str().c_str());
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
                if(ch=='y'){
                    if(User.gold>=cost){
                        User.gold-=cost;
                        draw_stats(status_win, User);
                        if(return_chance(User.inv.item[csr_pos+(page_num*30)].durability)){
                            User.inv.item[csr_pos+(page_num*30)].durability=100.0;
                            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
                            wclear(interaction_bar);
                            mvwaddstr(interaction_bar,0,0,"[System] Success: Item Repaired.");
                            wrefresh(interaction_bar);
                        }
                        else{
                            wclear(interaction_bar);
                            mvwaddstr(interaction_bar,0,0,"[System] Error: Repair failed.");
                            wrefresh(interaction_bar);
                        }
                    }
                    else{
                        wclear(interaction_bar);
                        mvwaddstr(interaction_bar,0,0,"[System] Error: Insufficient gold");
                        wrefresh(interaction_bar);
                    }
                }
                else{
                    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
                }
            }
        }
        if(ch=='e'){ // Enhance
            unsigned int amount = get_int(main_win, interaction_bar, "[System] Amount of materials you want to use in this enhancement: ");
            if(amount>0){
                User.uninitialize_stats();
                if(enhance_item(User.inv.item[csr_pos+(page_num*30)], User.inv.misc, amount)){
                    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
                }
                User.initialize_stats();
                draw_stats(status_win, User);
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[System] Failure");
                wrefresh(interaction_bar);
            }
        }
        if(ch=='R'){ // Reforge
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"[System] Reforge item? (This action is irreversible) [y/n]");
            wrefresh(interaction_bar);
            int ch = wgetch(main_win);
            if(ch=='y'){
                if(decrease_materials(User.inv.item[csr_pos+(page_num)*30].rarity, User.inv.misc, 3)){
                    unsigned int amount = get_int(main_win, interaction_bar, "Use Ancient Cores? [0 to not use any]: ");
                    if(User.inv.misc.ancient_core>=amount){
                        User.inv.misc.ancient_core-=amount;
                        User.uninitialize_stats();
                        reforge_item(amount, User.inv.item[csr_pos+(page_num*30)]);
                        User.inv.item[csr_pos+(page_num*30)].reinitialize_item();
                        User.initialize_stats();
                        draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
                        draw_stats(status_win, User);
                    }
                    else{
                        wclear(interaction_bar);
                        mvwaddstr(interaction_bar,0,0,"[System] Insufficient Ancient Cores");
                        wrefresh(interaction_bar);
                    }
                }
                else{
                    wclear(interaction_bar);
                    mvwaddstr(interaction_bar,0,0,"[System] Insufficient materials");
                    wrefresh(interaction_bar);
                }
            }
            else{
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
            }
        }
        // Crafting ('c')
        // Salvage ('S')
        if(ch=='q'){
            return;
        }
    }
}

