#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "headers/classes.h"
#include "headers/mode.h"
#include "headers/draw.h"
#include "headers/generate.h"
void draw_base(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int y, unsigned int size, unsigned int page, bool is_blacksmith_mode){
    std::stringstream ss;
    SDL_wclear_dialog_bar(main_win, context);
    if(!is_blacksmith_mode){
        ss<<"Inventory "<<"("<<page+1<<"/"<<((size-1)/30)+1<<")";
    }
    else{
        ss<<"[Blacksmith] How can I help you today? "<<"("<<page+1<<"/"<<((size-1)/30)+1<<")";
    }
    tcod::print(*main_win, {0,0}, ss.str(), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    for(int i=0; i<80; i++){
        TCOD_console_put_char(main_win.get(), i, y+1, '-', TCOD_BKGND_SET);
    }
    context->present(*main_win);
}
void print_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line){
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
            tcod::print(*main_win, {0,line+1}, "Error", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    ss<<cur_item->name;
    if(cur_item->calibration>0){
        ss<<" ["<<cur_item->calibration<<"]";
    }
    switch(cur_item->rarity){
        case 'c':
            tcod::print(*main_win, {0,line+1}, ss.str(), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'u':
            tcod::print(*main_win, {0,line+1}, ss.str(), &GREEN, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'r':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLUE, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'e':
            tcod::print(*main_win, {0,line+1}, ss.str(), &PURPLE, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'l':
            tcod::print(*main_win, {0,line+1}, ss.str(), &YELLOW, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'a':
            tcod::print(*main_win, {0,line+1}, ss.str(), &DARK_RED, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {0,line+1}, "Error", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
    }
}
void print_bold_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line){
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
            tcod::print(*main_win, {0,line+1}, "Error", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    ss<<cur_item->name;
    if(cur_item->calibration>0){
        ss<<" ["<<cur_item->calibration<<"]";
    }
    switch(cur_item->rarity){
        case 'c':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'u':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLACK, &GREEN, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'r':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLACK, &BLUE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'e':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLACK, &PURPLE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'l':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLACK, &YELLOW, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'a':
            tcod::print(*main_win, {0,line+1}, ss.str(), &BLACK, &DARK_RED, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {0,line+1}, "Error", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
    }
}
void print_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line){
    tcod::print(*main_win, {0,line+1}, "Name: ", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {6,line+1}, cur_item->name, nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,line+2}, "Rarity: ", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->rarity){
        case 'c':
            tcod::print(*main_win, {8,line+2}, "Common", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'u':
            tcod::print(*main_win, {8,line+2}, "Uncommon", &GREEN, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'r':
            tcod::print(*main_win, {8,line+2}, "Rare", &BLUE, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'e':
            tcod::print(*main_win, {8,line+2}, "Epic", &PURPLE, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'l':
            tcod::print(*main_win, {8,line+2}, "Legendary", &YELLOW, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'a':
            tcod::print(*main_win, {8,line+2}, "Artifact", &DARK_RED, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {8,line+2}, "-ERROR-", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
    }
    line+=2;
    tcod::print(*main_win, {0,line+1}, "Type: ", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->type){
        case 'h':
            tcod::print(*main_win, {6,line+1}, "Helmet", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'c':
            tcod::print(*main_win, {6,line+1}, "Chestplate", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'g':
            tcod::print(*main_win, {6,line+1}, "Greaves", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'b':
            tcod::print(*main_win, {6,line+1}, "Boots", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 's':
            tcod::print(*main_win, {6,line+1}, "Shield", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 'w':
            tcod::print(*main_win, {6,line+1}, "Weapon", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {6,line+1}, "-ERROR-", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    line++;
    tcod::print(*main_win, {0,line+1}, "Equipped: ", nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    if(cur_item->is_equipped){
        tcod::print(*main_win, {10,line+1}, "true", &GREEN, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(!cur_item->is_equipped){
        tcod::print(*main_win, {10,line+1}, "false", &LIGHT_RED, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    }
    line++;
    std::stringstream ss;
    tcod::print(*main_win, {0,line+1}, ("Durability: "+std::to_string(cur_item->durability)+"%"), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    if(cur_item->uses>0){
        tcod::print(*main_win, {0,line+1}, ("Uses: "+std::to_string(cur_item->uses)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->calibration>0){
        tcod::print(*main_win, {0,line+1}, ("Calibration Level: "+std::to_string(cur_item->calibration)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->enhancement>0){
        tcod::print(*main_win, {0,line+1}, ("Enhancement: "+std::to_string(cur_item->enhancement)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->hp>0){
        tcod::print(*main_win, {0,line+1}, ("Health: "+std::to_string(cur_item->hp)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->attk>0){
        tcod::print(*main_win, {0,line+1}, ("Damage: "+std::to_string(cur_item->attk)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->def>0){
        tcod::print(*main_win, {0,line+1}, ("Defence: "+std::to_string(cur_item->def)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->shield>0){
        tcod::print(*main_win, {0,line+1}, ("Shield: "+std::to_string(cur_item->shield)), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_chance>0){
        tcod::print(*main_win, {0,line+1}, ("Critical Chance: "+std::to_string(cur_item->crit_chance)+"%"), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_dmg>0){
        tcod::print(*main_win, {0,line+1}, ("Critical Damage: "+std::to_string(cur_item->crit_dmg)+"%"), nullptr, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    context->present(*main_win);
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
void draw_inventory(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  const Player &User, unsigned int page_num, unsigned int csr_pos, bool is_blacksmith_mode=false){
    SDL_wclear_main_win(main_win, context);
    draw_base(main_win, context, 34, User.inv.item.size(), page_num, is_blacksmith_mode);
    draw_stats(main_win, context, User);
    for(int i=page_num*30, iterator=0; i<User.inv.item.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, &User.inv.item[i], iterator);
    }
    print_description(main_win, context, &User.inv.item[page_num*30+csr_pos], 35);
    print_bold_item(main_win, context, &User.inv.item[page_num*30+csr_pos], csr_pos);
    context->present(*main_win);
}
std::string get_string(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string original){
    std::string input;
    clear_and_draw_dialog(main_win, context, "([ESC] to cancel) Rename item to: ");
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_RETURN&&input.length()>0){ // KEY_ENTER
            return input;
        }
        if(ch==SDLK_BACKSPACE){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        if(ch==SDLK_ESCAPE){ // KEY_ESC
            return original;
        }
        else if(input.length()<30&&(isdigit(ch)||isalpha(ch)||isblank(ch))){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss<<"Rename item to: "<<input;
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}
unsigned int get_int(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string dialogue){
    std::string input;
    clear_and_draw_dialog(main_win, context, dialogue);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_RETURN&&input.length()>0){ // KEY_ENTER
            return std::stoi(input);
        }
        if(ch==SDLK_BACKSPACE){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        if(ch==SDLK_ESCAPE||ch=='q'){ // KEY_ESC
            return 0;
        }
        else if(input.length()<8&&isdigit(ch)){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss<<dialogue<<input;
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}
void print_misc_bold(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, unsigned int csr_pos){
    switch(csr_pos){
        case 0:
            tcod::print(*main_win, {0,1}, "Ancient Cores: "+std::to_string(User.ancient_core), &BLACK, &DARK_RED, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 1:
            tcod::print(*main_win, {0,2}, "Crystallium: "+std::to_string(User.crystallium), &BLACK, &YELLOW, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 2:
            tcod::print(*main_win, {0,3}, "Crystal Cores: "+std::to_string(User.crystal_cores), &BLACK, &PURPLE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 5:
            tcod::print(*main_win, {0,6}, "Common: "+std::to_string(User.materials.common), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 6:
            tcod::print(*main_win, {0,7}, "Uncommon: "+std::to_string(User.materials.uncommon), &BLACK, &GREEN, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 7:
            tcod::print(*main_win, {0,8}, "Rare: "+std::to_string(User.materials.rare), &BLACK, &BLUE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 8:
            tcod::print(*main_win, {0,9}, "Epic: "+std::to_string(User.materials.epic), &BLACK, &PURPLE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 9:
            tcod::print(*main_win, {0,10}, "Legendary: "+std::to_string(User.materials.legendary), &BLACK, &YELLOW, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 10:
            tcod::print(*main_win, {0,11}, "Artifact: "+std::to_string(User.materials.artifact), &BLACK, &DARK_RED, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 13:
            tcod::print(*main_win, {0,14}, "Helmet: "+std::to_string(User.blueprint.helmet), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 14:
            tcod::print(*main_win, {0,15}, "Chestplate: "+std::to_string(User.blueprint.chestplate), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 15:
            tcod::print(*main_win, {0,16}, "Greaves: "+std::to_string(User.blueprint.greaves), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 16:
            tcod::print(*main_win, {0,17}, "Boots: "+std::to_string(User.blueprint.boots), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 17:
            tcod::print(*main_win, {0,18}, "Shield: "+std::to_string(User.blueprint.shield), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case 18:
            tcod::print(*main_win, {0,19}, "Weapon: "+std::to_string(User.blueprint.weapon), &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {0,1}, "Invalid cursor position", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
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
    switch(csr_pos){
        case 0:
            return decrease_amount(User.ancient_core, amount);
        case 1:
            return decrease_amount(User.crystallium, amount);
        case 4:
            return decrease_amount(User.materials.common, amount);
        case 5:
            return decrease_amount(User.materials.uncommon, amount);
        case 6:
            return decrease_amount(User.materials.rare, amount);
        case 7:
            return decrease_amount(User.materials.epic, amount);
        case 8:
            return decrease_amount(User.materials.legendary, amount);
        case 9:
            return decrease_amount(User.materials.artifact, amount);
        case 12:
            return decrease_amount(User.blueprint.helmet, amount);
        case 13:
            return decrease_amount(User.blueprint.chestplate, amount);
        case 14:
            return decrease_amount(User.blueprint.greaves, amount);
        case 15:
            return decrease_amount(User.blueprint.boots, amount);
        case 16:
            return decrease_amount(User.blueprint.shield, amount);
        case 17:
            return decrease_amount(User.blueprint.weapon, amount);
        default:
            return false;
    }
}
void print_misc_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    tcod::print(*main_win, {0,1}, "Ancient Cores: "+std::to_string(User.ancient_core), &DARK_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,2}, "Crystallium: "+std::to_string(User.crystallium), &YELLOW, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,3}, "Crystal Cores: "+std::to_string(User.crystal_cores), &PURPLE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,5}, "Materials:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,6}, "Common: "+std::to_string(User.materials.common), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,7}, "Uncommon: "+std::to_string(User.materials.uncommon), &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,8}, "Rare: "+std::to_string(User.materials.rare), &BLUE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,9}, "Epic: "+std::to_string(User.materials.epic), &PURPLE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,10}, "Legendary: "+std::to_string(User.materials.legendary), &YELLOW, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,11}, "Artifact: "+std::to_string(User.materials.artifact), &DARK_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,13}, "Blueprints:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    mvwaddstr(main_win, 13, 0, ("Helmet: "+std::to_string(User.blueprint.helmet)).c_str());
    mvwaddstr(main_win, 14, 0, ("Chestplate: "+std::to_string(User.blueprint.chestplate)).c_str());
    mvwaddstr(main_win, 15, 0, ("Greaves: "+std::to_string(User.blueprint.greaves)).c_str());
    mvwaddstr(main_win, 16, 0, ("Boots: "+std::to_string(User.blueprint.boots)).c_str());
    mvwaddstr(main_win, 17, 0, ("Shield: "+std::to_string(User.blueprint.shield)).c_str());
    mvwaddstr(main_win, 18, 0, ("Weapon: "+std::to_string(User.blueprint.weapon)).c_str());
    print_misc_bold(main_win, User, csr_pos);
    wrefresh(main_win);
}
void show_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, WINDOW *interaction_bar, Miscellaneous &User){
    unsigned int csr_pos=0;
    wclear(interaction_bar);
    mvwaddstr(interaction_bar, 0, 0, "Miscallaneous items:");
    wrefresh(interaction_bar);
    while(true){
        print_misc_item(main_win, User, csr_pos);
        int ch=wgetch(main_win);
        if((ch==KEY_DOWN||ch=='s')&&csr_pos<17){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar, 0, 0, "Miscallaneous items:");
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
            mvwaddstr(interaction_bar, 0, 0, "Miscallaneous items:");
            wrefresh(interaction_bar);
            if(csr_pos==4||csr_pos==12){
                csr_pos-=3;
            }
            else{
                csr_pos--;
            }
        }
        if(ch=='r'){
            unsigned int quantity=get_int(main_win, interaction_bar, "([ESC] to cancel) Quantity to be trashed: ");
            wclear(interaction_bar);
            if(!remove_misc_item(User, csr_pos, quantity)){
                mvwaddstr(interaction_bar, 0, 0, "[System] Invalid amount");
            }
            wrefresh(interaction_bar);
        }
        if(ch=='q'||ch=='m'){
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
    bool require_enhance=false;
    if(decrease_materials(item.rarity, misc, amount)){
        item.enhancement+=amount;
        item.reinitialize_item();
        return true;
    }
    return false;
}
void draw_crafting_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Miscellaneous &misc, unsigned int csr_pos, int blueprint_selection, int material_selection){
    wclear(main_win);
    wclear(interaction_bar);
    mvwaddstr(interaction_bar, 0, 0, "[Blacksmith] What do you want to craft today?");
    mvwaddstr(main_win, 0, 0, "Select a blueprint:");
    if(csr_pos==1||blueprint_selection==1){
        print_string_with_color(main_win, ("Helmet ["+std::to_string(misc.blueprint.helmet)+"]"), 15, 1);
    }
    else{
        print_string_with_color(main_win, ("Helmet ["+std::to_string(misc.blueprint.helmet)+"]"), 5, 1);
    }
    if(csr_pos==2||blueprint_selection==2){
        print_string_with_color(main_win, ("Chestplate ["+std::to_string(misc.blueprint.chestplate)+"]"), 15, 2);
    }
    else{
        print_string_with_color(main_win, ("Chestplate ["+std::to_string(misc.blueprint.chestplate)+"]"), 5, 2);
    }
    if(csr_pos==3||blueprint_selection==3){
        print_string_with_color(main_win, ("Greaves ["+std::to_string(misc.blueprint.greaves)+"]"), 15, 3);
    }
    else{
        print_string_with_color(main_win, ("Greaves ["+std::to_string(misc.blueprint.greaves)+"]"), 5, 3);
    }
    if(csr_pos==4||blueprint_selection==4){
        print_string_with_color(main_win, ("Boots ["+std::to_string(misc.blueprint.boots)+"]"), 15, 4);
    }
    else{
        print_string_with_color(main_win, ("Boots ["+std::to_string(misc.blueprint.boots)+"]"), 5, 4);
    }
    if(csr_pos==5||blueprint_selection==5){
        print_string_with_color(main_win, ("Shield ["+std::to_string(misc.blueprint.shield)+"]"), 15, 5);
    }
    else{
        print_string_with_color(main_win, ("Shield ["+std::to_string(misc.blueprint.shield)+"]"), 5, 5);
    }
    if(csr_pos==6||blueprint_selection==6){
        print_string_with_color(main_win, ("Weapon ["+std::to_string(misc.blueprint.weapon)+"]"), 15, 6);
    }
    else{
        print_string_with_color(main_win, ("Weapon ["+std::to_string(misc.blueprint.weapon)+"]"), 5, 6);
    }
    mvwaddstr(main_win, 8, 0, "Select the material:");
    if(csr_pos==9||material_selection==9){
        print_string_with_color(main_win, ("Common ["+std::to_string(misc.materials.common)+"]"), 15, 9);
    }
    else{
        print_string_with_color(main_win, ("Common ["+std::to_string(misc.materials.common)+"]"), 5, 9);
    }
    if(csr_pos==10||material_selection==10){
        print_string_with_color(main_win, ("Uncommon ["+std::to_string(misc.materials.uncommon)+"]"), 16, 10);
    }
    else{
        print_string_with_color(main_win, ("Uncommon ["+std::to_string(misc.materials.uncommon)+"]"), 6, 10);
    }
    if(csr_pos==11||material_selection==11){
        print_string_with_color(main_win, ("Rare ["+std::to_string(misc.materials.rare)+"]"), 17, 11);
    }
    else{
        print_string_with_color(main_win, ("Rare ["+std::to_string(misc.materials.rare)+"]"), 7, 11);
    }
    if(csr_pos==12||material_selection==12){
        print_string_with_color(main_win, ("Epic ["+std::to_string(misc.materials.epic)+"]"), 18, 12);
    }
    else{
        print_string_with_color(main_win, ("Epic ["+std::to_string(misc.materials.epic)+"]"), 8, 12);
    }
    if(csr_pos==13||material_selection==13){
        print_string_with_color(main_win, ("Legendary ["+std::to_string(misc.materials.legendary)+"]"), 19, 13);
    }
    else{
        print_string_with_color(main_win, ("Legendary ["+std::to_string(misc.materials.legendary)+"]"), 9, 13);
    }
    if(csr_pos==14||material_selection==14){
        print_string_with_color(main_win, ("Artifact ["+std::to_string(misc.materials.artifact)+"]"), 20, 14);
    }
    else{
        print_string_with_color(main_win, ("Artifact ["+std::to_string(misc.materials.artifact)+"]"), 10, 14);
    }
    if(csr_pos==16){
        print_string_with_color(main_win, "[Done]", 15, 16);
    }
    else{
        print_string_with_color(main_win, "[Done]", 5, 16);
    }
    wrefresh(main_win);
    wrefresh(interaction_bar);
}
bool check_if_sufficient_materials(Miscellaneous &misc, int material_selection, int required_material, char &rarity){
    switch(material_selection){
        case 9: // common
            if(misc.materials.common>=required_material){
                misc.materials.common-=required_material;
                rarity=RARITY_COMMON;
                return true;
            }
            else{
                return false;
            }
            break;
        case 10: // uncommon
            if(misc.materials.uncommon>=required_material){
                misc.materials.uncommon-=required_material;
                rarity=RARITY_UNCOMMON;
                return true;
            }
            else{
                return false;
            }
            break;
        case 11: // rare
            if(misc.materials.rare>=required_material){
                misc.materials.rare-=required_material;
                rarity=RARITY_RARE;
                return true;
            }
            else{
                return false;
            }
            break;
        case 12: // epic
            if(misc.materials.epic>=required_material){
                misc.materials.epic-=required_material;
                rarity=RARITY_EPIC;
                return true;
            }
            else{
                return false;
            }
            break;
        case 13: // legendary
            if(misc.materials.legendary>=required_material){
                misc.materials.legendary-=required_material;
                rarity=RARITY_LEGENDARY;
                return true;
            }
            else{
                return false;
            }
            break;
        case 14: // artifact
            if(misc.materials.artifact>=required_material){
                misc.materials.artifact-=required_material;
                rarity=RARITY_ARTIFACT;
                return true;
            }
            else{
                return false;
            }
            break;
        default:
            return false;
    }
}
bool check_if_craft_valid(int blueprint_selection, int material_selection, Miscellaneous &misc, char &gear_type, char &rarity){
    switch(blueprint_selection){
        case 1: // helmet
            if(misc.blueprint.helmet>0){
                if(check_if_sufficient_materials(misc, material_selection, 3, rarity)){
                    misc.blueprint.helmet--;
                    gear_type=TYPE_HELMET;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 2: // chestplate
            if(misc.blueprint.chestplate>0){
                if(check_if_sufficient_materials(misc, material_selection, 5, rarity)){
                    misc.blueprint.chestplate--;
                    gear_type=TYPE_CHESTPLATE;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 3: // greaves
            if(misc.blueprint.greaves>0){
                if(check_if_sufficient_materials(misc, material_selection, 3, rarity)){
                    misc.blueprint.greaves--;
                    gear_type=TYPE_GREAVES;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 4: // boots
            if(misc.blueprint.boots>0){
                if(check_if_sufficient_materials(misc, material_selection, 2, rarity)){
                    misc.blueprint.boots--;
                    gear_type=TYPE_BOOTS;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 5: // shield
            if(misc.blueprint.shield>0){
                if(check_if_sufficient_materials(misc, material_selection, 4, rarity)){
                    misc.blueprint.shield--;
                    gear_type=TYPE_SHIELD;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 6: // weapon
            if(misc.blueprint.weapon>0){
                if(check_if_sufficient_materials(misc, material_selection, 6, rarity)){
                    misc.blueprint.weapon--;
                    gear_type=TYPE_WEAPON;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        default:
            return false;
    }
    return false;
}
void crafting_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User){
    unsigned int csr_pos=1;
    int blueprint_selection=0, material_selection=0;
    int ch;
    while(true){
        draw_crafting_selection(main_win, context, User.inv.misc, csr_pos, blueprint_selection, material_selection);
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<15){
            if(csr_pos==6){
                csr_pos=9;
            }
            else if(csr_pos==14){
                csr_pos=16;
            }
            else{
                csr_pos++;
            }
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>1){
            if(csr_pos==9){
                csr_pos=6;
            }
            else if(csr_pos==16){
                csr_pos=14;
            }
            else{
                csr_pos--;
            }
        }
        if(ch==SDLK_TAB){ // KEY_TAB
            if(csr_pos<7){
                csr_pos+=8;
            }
            else if(csr_pos<15){
                csr_pos-=8;
            }
        }
        if(ch==SDLK_RETURN){ // KEY_ENTER
            if(csr_pos<7){
                blueprint_selection=csr_pos;
            }
            else if(csr_pos<15){
                material_selection=csr_pos;
            }
            if(csr_pos==16&&blueprint_selection!=0&&material_selection!=0){
                char gear_type, rarity;
                if(check_if_craft_valid(blueprint_selection, material_selection, User.inv.misc, gear_type, rarity)){
                    User.add_item(craft_item(rarity, gear_type));
                    return;
                }
            }
        }
        if(ch=='q'){
            return;
        }
    }
}
void salvage_item(Player &User, long long int pos){
    switch(User.inv.item[pos].rarity){
        case RARITY_COMMON:
            User.inv.misc.materials.common+=(User.inv.item[pos].enhancement/2)+2;
            break;
        case RARITY_UNCOMMON:
            User.inv.misc.materials.uncommon+=(User.inv.item[pos].enhancement/2)+2;
            break;
        case RARITY_RARE:
            User.inv.misc.materials.rare+=(User.inv.item[pos].enhancement/2)+2;
            break;
        case RARITY_EPIC:
            User.inv.misc.materials.epic+=(User.inv.item[pos].enhancement/2)+2;
            break;
        case RARITY_LEGENDARY:
            User.inv.misc.materials.legendary+=(User.inv.item[pos].enhancement/2)+2;
            break;
        case RARITY_ARTIFACT:
            User.inv.misc.materials.artifact+=(User.inv.item[pos].enhancement/2)+2;
            break;
        default:
            break;
    }
    User.uninitialize_stats();
    User.remove_item(pos);
    User.initialize_stats();
}
void inventory_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  Player &User){
    unsigned int page_num=0;
    int csr_pos=0;
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
            std::string input=get_string(main_win, interaction_bar, User.inv.item[csr_pos+(page_num*30)].name);
            User.inv.item[csr_pos+(page_num*30)].name=input;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='m'){
            show_misc_items(main_win, interaction_bar, User.inv.misc);
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos);
        }
        if(ch=='H'){
            help_mode(main_win, interaction_bar, "inventory_mode");
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
                if((page_num*30+csr_pos)>=User.inv.item.size()){
                    csr_pos--;
                }
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
void reforge_repair_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  Player &User){
    unsigned int page_num=0;
    unsigned int csr_pos=0;
    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
    while(true){
        int ch=wgetch(main_win);
        if((ch=='s'||ch==KEY_DOWN)&&((csr_pos+page_num*30)<User.inv.item.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
        }
        if((ch=='w'||ch==KEY_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
        }
        if(ch=='a'||ch==KEY_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
            }
        }
        if(ch=='d'||ch==KEY_RIGHT){
            if((page_num+1)*30<User.inv.item.size()){
                page_num++;
                csr_pos=0;
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
                wrefresh(main_win);
            }
        }
        if(ch=='m'){
            show_misc_items(main_win, interaction_bar, User.inv.misc);
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
        }
        if(ch=='r'){
            wclear(interaction_bar);
            if(User.inv.item[csr_pos+(page_num*30)].durability<100.0){
                const unsigned int cost=(100.0-User.inv.item[csr_pos+(page_num*30)].durability)*rarity_value(User.inv.item[csr_pos+(page_num*30)].rarity)*15;
                const unsigned int material_cost=2*(1+((100-User.inv.item[csr_pos+(page_num*30)].durability)/100));
                std::stringstream ss;
                ss<<"[System] Attempt repair? Gold:"<<cost<<" Material:"<<material_cost<<" [y/n]";
                mvwaddstr(interaction_bar, 0, 0, ss.str().c_str());
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
                if(ch=='y'){
                    if(User.gold>=cost&&decrease_materials(User.inv.item[csr_pos+(page_num*30)].rarity, User.inv.misc, material_cost)){
                        User.gold-=cost;
                        draw_stats(status_win, User);
                        if(return_chance(User.inv.item[csr_pos+(page_num*30)].durability*2)){
                            User.inv.item[csr_pos+(page_num*30)].durability=100.0;
                            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
                            wclear(interaction_bar);
                            mvwaddstr(interaction_bar, 0, 0, "[System] Success: Item Repaired.");
                            wrefresh(interaction_bar);
                        }
                        else{
                            wclear(interaction_bar);
                            mvwaddstr(interaction_bar, 0, 0, "[System] Error: Repair failed.");
                            wrefresh(interaction_bar);
                        }
                    }
                    else{
                        wclear(interaction_bar);
                        mvwaddstr(interaction_bar, 0, 0, "[System] Error: Insufficient gold/materials");
                        wrefresh(interaction_bar);
                    }
                }
                else{
                    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
                }
            }
        }
        if(ch=='e'){ // Enhance
            unsigned int amount=get_int(main_win, interaction_bar, "[System] Amount of materials you want to use in this enhancement: ");
            if(amount>0){
                User.uninitialize_stats();
                if(enhance_item(User.inv.item[csr_pos+(page_num*30)], User.inv.misc, amount)){
                    draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
                }
                User.initialize_stats();
                draw_stats(status_win, User);
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar, 0, 0, "[System] Failure");
                wrefresh(interaction_bar);
            }
        }
        if(ch=='R'){ // Reforge
            wclear(interaction_bar);
            mvwaddstr(interaction_bar, 0, 0, "[System] Reforge item? (This action is irreversible) [y/n]");
            wrefresh(interaction_bar);
            int ch=wgetch(main_win);
            if(ch=='y'){
                unsigned int gold_cost=rarity_value(User.inv.item[csr_pos+(page_num)*30].rarity)*200;
                unsigned int material_cost=2*(1+((100-User.inv.item[csr_pos+(page_num*30)].durability)/100.0));
                wclear(interaction_bar);
                mvwaddstr(interaction_bar, 0, 0, ("Proceed? Cost of reforge: "+std::to_string(gold_cost)+" Gold, "+std::to_string(material_cost)+" Materials [y/n]").c_str());
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
                if(decrease_materials(User.inv.item[csr_pos+(page_num)*30].rarity, User.inv.misc, material_cost)&&User.gold>=gold_cost&&ch=='y'){
                    User.gold-=gold_cost;
                    bool use_crystallium=false, use_ancient_core=false;
                    unsigned int crystallium_amount=get_int(main_win, interaction_bar, "Use Crystallium? [0 to not use any]: ");
                    unsigned int ancient_core_amount=0;
                    if(crystallium_amount==0){
                        ancient_core_amount=get_int(main_win, interaction_bar, "Use Ancient Cores? [0 to not use any]: ");
                        if(ancient_core_amount>0){
                            use_ancient_core=true;
                        }
                    }
                    else if(crystallium_amount>0){
                        use_crystallium=true;
                    }
                    if(User.inv.misc.ancient_core>=ancient_core_amount&&User.inv.misc.crystallium>=crystallium_amount){
                        User.inv.misc.ancient_core-=ancient_core_amount;
                        User.inv.misc.crystallium-=crystallium_amount;
                        User.uninitialize_stats();
                        reforge_item(ancient_core_amount, crystallium_amount, User.inv.item[csr_pos+(page_num*30)]);
                        User.inv.item[csr_pos+(page_num*30)].reinitialize_item();
                        User.initialize_stats();
                        draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
                        draw_stats(status_win, User);
                    }
                    else{
                        wclear(interaction_bar);
                        mvwaddstr(interaction_bar, 0, 0, "[System] Insufficient materials");
                        wrefresh(interaction_bar);
                    }
                }
                else{
                    wclear(interaction_bar);
                    mvwaddstr(interaction_bar, 0, 0, "[System] Aborted");
                    wrefresh(interaction_bar);
                }
            }
            else{
                draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
            }
        }
        if(ch=='c'){
            crafting_mode(main_win, interaction_bar, User);
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
        }
        if(ch=='S'){
            wclear(interaction_bar);
            mvwaddstr(interaction_bar,0,0,"Salvage item? (This action is irreversible) [y/n]");
            wrefresh(interaction_bar);
            ch = wgetch(main_win);
            if(ch=='y'){
                salvage_item(User, csr_pos+(page_num*30));
                csr_pos--;
                if(csr_pos<0){
                    csr_pos=0;
                }
            }
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
        }
        if(ch=='H'){
            help_mode(main_win, interaction_bar, "blacksmith_mode");
            draw_inventory(main_win, interaction_bar, status_win, User, page_num, csr_pos, true);
        }
        if(ch=='q'){
            return;
        }
    }
}

