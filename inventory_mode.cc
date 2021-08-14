#include <curses.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "headers/classes.h"
#include "headers/mode.h"
#include "headers/draw.h"
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
    mvwaddstr(interaction_bar,0,0,ss.str().c_str());
    for(int i = 0; i<80; i++){
        mvwaddch(main_win,y,i,'-');
    }
    wrefresh(interaction_bar);
    wrefresh(main_win);
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
            mvwaddstr(main_win,line,0,"Error");
            return;
    }
    ss<<cur_item->name;
    if(cur_item->calibration>0){
        ss<<" ["<<cur_item->calibration<<"]";
    }
    switch (cur_item->rarity){
        case 'c':
            wattron(main_win,COLOR_PAIR(5));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(5));
            break;
        case 'u':
            wattron(main_win,COLOR_PAIR(6));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(6));
            break;
        case 'r':
            wattron(main_win,COLOR_PAIR(7));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(7));
            break;
        case 'e':
            wattron(main_win,COLOR_PAIR(8));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(8));
            break;
        case 'l':
            wattron(main_win,COLOR_PAIR(9));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(9));
            break;
        case 'a':
            wattron(main_win,COLOR_PAIR(10));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(10));
            break;
        default:
            mvwaddstr(main_win,line,0,"Error");
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
            mvwaddstr(main_win,line,0,"Error");
            return;
    }
    ss<<cur_item->name;
    if(cur_item->calibration>0){
        ss<<" ["<<cur_item->calibration<<"]";
    }
    switch (cur_item->rarity){
        case 'c':
            wattron(main_win,COLOR_PAIR(15));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(15));
            break;
        case 'u':
            wattron(main_win,COLOR_PAIR(16));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(16));
            break;
        case 'r':
            wattron(main_win,COLOR_PAIR(17));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(17));
            break;
        case 'e':
            wattron(main_win,COLOR_PAIR(18));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(18));
            break;
        case 'l':
            wattron(main_win,COLOR_PAIR(19));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(19));
            break;
        case 'a':
            wattron(main_win,COLOR_PAIR(20));
            mvwaddstr(main_win,line,0,ss.str().c_str());
            wattroff(main_win,COLOR_PAIR(20));
            break;
        default:
            mvwaddstr(main_win,line,0,"Error");
            break;
    }
}
void print_description(WINDOW *main_win, const Item *cur_item, int line){
    mvwaddstr(main_win,line,0,"Name: ");
    mvwaddstr(main_win,line+1,0,"Rarity: ");
    switch (cur_item->rarity){
        case 'c':
            mvwaddstr(main_win,line,6,cur_item->name.c_str());
            wattron(main_win,COLOR_PAIR(5));
            mvwaddstr(main_win,line+1,8,"Common");
            wattroff(main_win,COLOR_PAIR(5));
            break;
        case 'u':
            mvwaddstr(main_win,line,6,cur_item->name.c_str());
            wattron(main_win,COLOR_PAIR(6));
            mvwaddstr(main_win,line+1,8,"Uncommon");
            wattroff(main_win,COLOR_PAIR(6));
            break;
        case 'r':
            mvwaddstr(main_win,line,6,cur_item->name.c_str());
            wattron(main_win,COLOR_PAIR(7));
            mvwaddstr(main_win,line+1,8,"Rare");
            wattroff(main_win,COLOR_PAIR(7));
            break;
        case 'e':
            mvwaddstr(main_win,line,6,cur_item->name.c_str());
            wattron(main_win,COLOR_PAIR(8));
            mvwaddstr(main_win,line+1,8,"Epic");
            wattroff(main_win,COLOR_PAIR(8));
            break;
        case 'l':
            mvwaddstr(main_win,line,6,cur_item->name.c_str());
            wattron(main_win,COLOR_PAIR(9));
            mvwaddstr(main_win,line+1,8,"Legendary");
            wattroff(main_win,COLOR_PAIR(9));
            break;
        case 'a':
            mvwaddstr(main_win,line,6,cur_item->name.c_str());
            wattron(main_win,COLOR_PAIR(10));
            mvwaddstr(main_win,line+1,8,"Artifact");
            wattroff(main_win,COLOR_PAIR(10));
            break;
        default:
            mvwaddstr(main_win,line,0,"-Error-");
            break;
    }
    line+=2;
    mvwaddstr(main_win,line,0,"Type: ");
    switch(cur_item->type){
        case 'h':
            mvwaddstr(main_win,line,6,"Helmet");
            break;
        case 'c':
            mvwaddstr(main_win,line,6,"Chestplate");
            break;
        case 'g':
            mvwaddstr(main_win,line,6,"Greaves");
            break;
        case 'b':
            mvwaddstr(main_win,line,6,"Boots");
            break;
        case 's':
            mvwaddstr(main_win,line,6,"Shield");
            break;
        case 'w':
            mvwaddstr(main_win,line,6,"Weapon");
            break;
        default:
            mvwaddstr(main_win,line,0,"-Error-");
            return;
    }
    line++;
    if(cur_item->is_equipped){
        wattron(main_win,COLOR_PAIR(6));
        mvwaddstr(main_win,line,0,"Equipped: true");
        wattroff(main_win,COLOR_PAIR(6));
    }
    if(!cur_item->is_equipped){
        wattron(main_win,COLOR_PAIR(10));
        mvwaddstr(main_win,line,0,"Equipped: false");
        wattroff(main_win,COLOR_PAIR(10));
    }
    line++;
    std::stringstream ss;
    if(cur_item->uses>0){
        ss<<"Uses: "<<cur_item->uses;
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->calibration>0){
        ss<<"Calibration Level: "<<cur_item->calibration;
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->hp>0){
        ss<<"Health: "<<cur_item->hp;
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->attk>0){
        ss<<"Damage: "<<cur_item->attk;
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->def>0){
        ss<<"Defence: "<<cur_item->def;
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->shield>0){
        ss<<"Shield: "<<cur_item->shield;
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->crit_chance>0){
        ss<<"Critical Chance: "<<cur_item->crit_chance<<"%";
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
        line++;
    }
    if(cur_item->crit_dmg>0){
        ss<<"Critical Damage: "<<cur_item->crit_dmg<<"%";
        mvwaddstr(main_win,line,0,ss.str().c_str());
        ss.str(std::string());
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
    User.inv.item[csr_pos+(page_num*30)].is_equipped=false;
    if(User.inv.item[csr_pos+(page_num*30)].type=='h'){
        User.uninitialize_gear(User.equip.helmet);
        User.equip.helmet=nullptr;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='c'){
        User.uninitialize_gear(User.equip.chestplate);
        User.equip.chestplate=nullptr;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='g'){
        User.uninitialize_gear(User.equip.greaves);
        User.equip.greaves=nullptr;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='b'){
        User.uninitialize_gear(User.equip.boots);
        User.equip.boots=nullptr;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='s'){
        User.uninitialize_gear(User.equip.shield);
        User.equip.shield=nullptr;
    }
    if(User.inv.item[csr_pos+(page_num*30)].type=='w'){
        User.uninitialize_gear(User.equip.weapon);
        User.equip.weapon=nullptr;
    }
}
void inventory_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar, Player &User){
    clear_screen(main_win,interaction_bar);
    unsigned int page_num = 0;
    unsigned int csr_pos = 0;
    for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
        print_item(main_win,&User.inv.item[i],iterator);
    }
    wrefresh(main_win);
    draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
    print_bold_item(main_win,&User.inv.item[page_num*30],0);
    print_description(main_win,&User.inv.item[page_num*30],35);
    while(true){
        int ch = wgetch(main_win);
        if(ch=='s'&&((csr_pos+page_num*30)<User.inv.item.size()-1&&csr_pos<29)){
            csr_pos++;
            clear_screen(main_win);
            draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
            for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
                print_item(main_win,&User.inv.item[i],iterator);
            }
            print_description(main_win,&User.inv.item[page_num*30+csr_pos],35);
            print_bold_item(main_win,&User.inv.item[page_num*30+csr_pos],csr_pos);

        }
        if(ch=='w'&&csr_pos>0){
            csr_pos--;
            clear_screen(main_win);
            draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
            for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
                print_item(main_win,&User.inv.item[i],iterator);
            }
            print_description(main_win,&User.inv.item[page_num*30+csr_pos],35);
            print_bold_item(main_win,&User.inv.item[page_num*30+csr_pos],csr_pos);

        }
        if(ch=='a'){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                clear_screen(main_win);
                draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
                for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
                    print_item(main_win,&User.inv.item[i],iterator);
                }
                print_description(main_win,&User.inv.item[page_num*30],35);
                print_bold_item(main_win,&User.inv.item[page_num*30],0);
            }
        }
        if(ch=='d'){
            if((page_num+1)*30<User.inv.item.size()){
                page_num++;
                csr_pos=0;
                clear_screen(main_win);
                draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
                for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
                    print_item(main_win,&User.inv.item[i],iterator);
                }
                print_description(main_win,&User.inv.item[page_num*30],35);
                print_bold_item(main_win,&User.inv.item[page_num*30],0);
                wrefresh(main_win);
            }
        }
        if(ch=='e'){
            if(!User.inv.item[csr_pos+(page_num*30)].is_equipped){ // if there current item is unequipped
                equip_item(User,csr_pos,page_num);
            }
            else if(User.inv.item[csr_pos+(page_num*30)].is_equipped){
                unequip_item(User,csr_pos,page_num);
            }
            clear_screen(main_win);
            draw_stats(status_win,User);
            draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
            for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
                print_item(main_win,&User.inv.item[i],iterator);
            }
            print_description(main_win,&User.inv.item[page_num*30+csr_pos],35);
            print_bold_item(main_win,&User.inv.item[page_num*30+csr_pos],csr_pos);
        }
        if(ch=='r'){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"[System] Are you sure you want to delete this item? Press y to continue, any other key to cancel.");
            wrefresh(interaction_bar);
            ch = wgetch(main_win);
            if(ch=='y'){
                unequip_item(User,csr_pos,page_num);
                User.inv.item.erase(User.inv.item.begin()+(csr_pos+(page_num*30)));
            }
            clear_screen(main_win);
            draw_stats(status_win,User);
            draw_base(main_win,interaction_bar,34,User.inv.item.size(),page_num);
            for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++,iterator++){
                print_item(main_win,&User.inv.item[i],iterator);
            }
            print_description(main_win,&User.inv.item[page_num*30+csr_pos],35);
            print_bold_item(main_win,&User.inv.item[page_num*30+csr_pos],csr_pos);
        }
        if(ch=='q'){
            return;
        }
    }
}
// type: 'h' helmet, 'c' chestplate, 'g' greaves, 'b' boots, 's' shield, 'w' weapon
// rarity: 'c' common, 'u' uncommon, 'r' rare, 'e' epic, 'l' legendary, 'a' artifact
// Item(std::string name, char type, char rarity, bool is_equipped, int hp, int attk, int def, int shield, int crit_chance, int crit_dmg)

