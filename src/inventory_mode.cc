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

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include "headers/classes.h"
#include "headers/mode.h"
#include "headers/draw.h"
#include "headers/generate.h"

void draw_base(tcod::Console &main_win, tcod::ContextPtr &context, int y, unsigned int size, unsigned int page, bool is_blacksmith_mode, bool is_inventory_modifier_mode){
    std::stringstream ss;
    SDL_wclear_dialog_bar(main_win, context);
    if(size==0){
        ss << "[System] Inventory Empty";
    }
    else if(is_blacksmith_mode){
        ss << "[Blacksmith] How can I help you today? " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")";
    }
    else if(is_inventory_modifier_mode){
        ss << "Inventory " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")" << " [Inventory Modifier Mode]";
    }
    else{
        ss << "Inventory " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")";
    }
    tcod::print(main_win, {0, 0}, ss.str(), WHITE, BLACK);
    for(int i=0; i<80; i++){
        TCOD_console_put_char_ex(main_win.get(), i, y+1, '-', WHITE, BLACK);
    }
//    context->present(main_win);
}

void print_bold_with_condition(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &output, const TCOD_ColorRGB &foreground, int line, bool is_bold){
    if(is_bold){
        tcod::print(main_win, {0, line}, output, BLACK, foreground);
    }
    else{
        tcod::print(main_win, {0, line}, output, foreground, BLACK);
    }
}

void print_bold_with_condition_ex(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &output, const TCOD_ColorRGB &foreground, const TCOD_ColorRGB &background, int x, int y, TCOD_alignment_t alignment, bool is_bold){
    if(is_bold){
        tcod::print(main_win, {x, y}, output, background, foreground, alignment, TCOD_BKGND_SET);
    }
    else{
        tcod::print(main_win, {x, y}, output, foreground, background, alignment, TCOD_BKGND_SET);
    }
}

void print_item(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, bool is_selected){
    std::stringstream ss;
    if(cur_item->is_equipped){
        ss << "*";
    }
    switch(cur_item->type){
        case Type::HELMET:
            ss << "[Helmet] ";
            break;
        case Type::CHESTPLATE:
            ss << "[Chestplate] ";
            break;
        case Type::GREAVES:
            ss << "[Greaves] ";
            break;
        case Type::BOOTS:
            ss << "[Boots] ";
            break;
        case Type::SHIELD:
            ss << "[Shield] ";
            break;
        case Type::WEAPON:
            ss << "[Weapon] ";
            break;
        default:
            tcod::print(main_win, {0, line+1}, "Error", WHITE, BLACK);
            return;
    }
    ss << cur_item->name;
    if(cur_item->calibration>0){
        ss << " [" << cur_item->calibration << "]";
    }
    switch(cur_item->rarity){
        case Rarity::COMMON:
            print_bold_with_condition(main_win, context, ss.str(), WHITE, line+1, is_selected);
            break;
        case Rarity::UNCOMMON:
            print_bold_with_condition(main_win, context, ss.str(), GREEN, line+1, is_selected);
            break;
        case Rarity::RARE:
            print_bold_with_condition(main_win, context, ss.str(), BLUE, line+1, is_selected);
            break;
        case Rarity::EPIC:
            print_bold_with_condition(main_win, context, ss.str(), PURPLE, line+1, is_selected);
            break;
        case Rarity::LEGENDARY:
            print_bold_with_condition(main_win, context, ss.str(), YELLOW, line+1, is_selected);
            break;
        case Rarity::ARTIFACT:
            print_bold_with_condition(main_win, context, ss.str(), DARK_RED, line+1, is_selected);
            break;
        default:
            break;
    }
}

void print_description(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line){
    tcod::print(main_win, {0, line+1}, "Name: ", WHITE, BLACK);
    tcod::print(main_win, {6, line+1}, cur_item->name, WHITE, BLACK);
    tcod::print(main_win, {0, line+2}, "Rarity: ", WHITE, BLACK);
    switch(cur_item->rarity){
        case Rarity::COMMON:
            tcod::print(main_win, {8, line+2}, "Common", WHITE, BLACK);
            break;
        case Rarity::UNCOMMON:
            tcod::print(main_win, {8, line+2}, "Uncommon", GREEN, BLACK);
            break;
        case Rarity::RARE:
            tcod::print(main_win, {8, line+2}, "Rare", BLUE, BLACK);
            break;
        case Rarity::EPIC:
            tcod::print(main_win, {8, line+2}, "Epic", PURPLE, BLACK);
            break;
        case Rarity::LEGENDARY:
            tcod::print(main_win, {8, line+2}, "Legendary", YELLOW, BLACK);
            break;
        case Rarity::ARTIFACT:
            tcod::print(main_win, {8, line+2}, "Artifact", DARK_RED, BLACK);
            break;
        default:
            tcod::print(main_win, {8, line+2}, "-ERROR-", WHITE, BLACK);
            break;
    }
    line+=2;
    tcod::print(main_win, {0, line+1}, "Type: ", WHITE, BLACK);
    switch(cur_item->type){
        case Type::HELMET:
            tcod::print(main_win, {6, line+1}, "Helmet", WHITE, BLACK);
            break;
        case Type::CHESTPLATE:
            tcod::print(main_win, {6, line+1}, "Chestplate", WHITE, BLACK);
            break;
        case Type::GREAVES:
            tcod::print(main_win, {6, line+1}, "Greaves", WHITE, BLACK);
            break;
        case Type::BOOTS:
            tcod::print(main_win, {6, line+1}, "Boots", WHITE, BLACK);
            break;
        case Type::SHIELD:
            tcod::print(main_win, {6, line+1}, "Shield", WHITE, BLACK);
            break;
        case Type::WEAPON:
            tcod::print(main_win, {6, line+1}, "Weapon", WHITE, BLACK);
            break;
        default:
            tcod::print(main_win, {6, line+1}, "-ERROR-", WHITE, BLACK);
            return;
    }
    line++;
    tcod::print(main_win, {0, line+1}, "Equipped: ", WHITE, BLACK);
    if(cur_item->is_equipped){
        tcod::print(main_win, {10, line+1}, "true", GREEN, BLACK);
    }
    if(!cur_item->is_equipped){
        tcod::print(main_win, {10, line+1}, "false", LIGHT_RED, BLACK);
    }
    line++;
    std::stringstream ss;
    tcod::print(main_win, {0, line+1}, ("Durability: "+std::to_string(cur_item->durability)+"%"), WHITE, BLACK);
    line++;
    if(cur_item->uses>0){
        tcod::print(main_win, {0, line+1}, ("Uses: "+std::to_string(cur_item->uses)), WHITE, BLACK);
        line++;
    }
    if(cur_item->calibration>0){
        tcod::print(main_win, {0, line+1}, ("Calibration Level: "+std::to_string(cur_item->calibration)), WHITE, BLACK);
        line++;
    }
    if(cur_item->enhancement>0){
        tcod::print(main_win, {0, line+1}, ("Enhancement: "+std::to_string(cur_item->enhancement)), WHITE, BLACK);
        line++;
    }
    if(cur_item->hp>0){
        tcod::print(main_win, {0, line+1}, ("Health: "+std::to_string(cur_item->hp)), WHITE, BLACK);
        line++;
    }
    if(cur_item->attk>0){
        tcod::print(main_win, {0, line+1}, ("Damage: "+std::to_string(cur_item->attk)), WHITE, BLACK);
        line++;
    }
    if(cur_item->def>0){
        tcod::print(main_win, {0, line+1}, ("Defence: "+std::to_string(cur_item->def)), WHITE, BLACK);
        line++;
    }
    if(cur_item->shield>0){
        tcod::print(main_win, {0, line+1}, ("Shield: "+std::to_string(cur_item->shield)), WHITE, BLACK);
        line++;
    }
    if(cur_item->crit_chance>0){
        tcod::print(main_win, {0, line+1}, ("Critical Chance: "+std::to_string(cur_item->crit_chance)+"%"), WHITE, BLACK);
        line++;
    }
    if(cur_item->crit_dmg>0){
        tcod::print(main_win, {0, line+1}, ("Critical Damage: "+std::to_string(cur_item->crit_dmg)+"%"), WHITE, BLACK);
        line++;
    }
    context->present(main_win);
}

void equip_item(Player &user, Item *cur_item){
    cur_item->is_equipped=true;
    if(cur_item->type==Type::HELMET){
        if(user.equip.helmet!=nullptr){
            user.uninitialize_gear(user.equip.helmet);
            user.equip.helmet->is_equipped=false;
        }
        user.equip.helmet=cur_item;
        user.initialize_gear(user.equip.helmet);
    }
    if(cur_item->type==Type::CHESTPLATE){
        if(user.equip.chestplate!=nullptr){
            user.uninitialize_gear(user.equip.chestplate);
            user.equip.chestplate->is_equipped=false;
        }
        user.equip.chestplate=cur_item;
        user.initialize_gear(user.equip.chestplate);
    }
    if(cur_item->type==Type::GREAVES){
        if(user.equip.greaves!=nullptr){
            user.uninitialize_gear(user.equip.greaves);
            user.equip.greaves->is_equipped=false;
        }
        user.equip.greaves=cur_item;
        user.initialize_gear(user.equip.greaves);
    }
    if(cur_item->type==Type::BOOTS){
        if(user.equip.boots!=nullptr){
            user.uninitialize_gear(user.equip.boots);
            user.equip.boots->is_equipped=false;
        }
        user.equip.boots=cur_item;
        user.initialize_gear(user.equip.boots);
    }
    if(cur_item->type==Type::SHIELD){
        if(user.equip.shield!=nullptr){
            user.uninitialize_gear(user.equip.shield);
            user.equip.shield->is_equipped=false;
        }
        user.equip.shield=cur_item;
        user.initialize_gear(user.equip.shield);
    }
    if(cur_item->type==Type::WEAPON){
        if(user.equip.weapon!=nullptr){
            user.uninitialize_gear(user.equip.weapon);
            user.equip.weapon->is_equipped=false;
        }
        user.equip.weapon=cur_item;
        user.initialize_gear(user.equip.weapon);
    }
}

bool unequip_item(Player &user, Item *cur_item){
    if(!cur_item->is_equipped){
        return true;
    }
    if(user.cur_hp<=cur_item->hp){
        return false;
    }
    if(cur_item->type==Type::HELMET&&user.cur_hp>cur_item->hp){
        user.uninitialize_gear(user.equip.helmet);
        user.equip.helmet=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::CHESTPLATE&&user.cur_hp>cur_item->hp){
        user.uninitialize_gear(user.equip.chestplate);
        user.equip.chestplate=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::GREAVES){
        user.uninitialize_gear(user.equip.greaves);
        user.equip.greaves=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::BOOTS){
        user.uninitialize_gear(user.equip.boots);
        user.equip.boots=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::SHIELD){
        user.uninitialize_gear(user.equip.shield);
        user.equip.shield=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::WEAPON){
        user.uninitialize_gear(user.equip.weapon);
        user.equip.weapon=nullptr;
        cur_item->is_equipped=false;
    }
    return true;
}

void draw_inventory(tcod::Console &main_win, tcod::ContextPtr &context, const Player &user, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, bool is_blacksmith_mode, bool is_inventory_modifier_mode){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, user);
    if(!items_copy.empty()){
        draw_base(main_win, context, 34, items_copy.size(), page_num, is_blacksmith_mode, is_inventory_modifier_mode);
        for(long long int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
            print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
        }
        print_description(main_win, context, items_copy[page_num*30+csr_pos], 35);
    }
    else{
        draw_base(main_win, context, 34, 0, page_num, is_blacksmith_mode, is_inventory_modifier_mode);
        context->present(main_win);
    }
}

bool decrease_amount(unsigned long long int &original, unsigned int amount){
    if(original>=amount){
        original-=amount;
        return true;
    }
    else{
        return false;
    }
}

bool remove_misc_item(Miscellaneous &user, unsigned int csr_pos, unsigned int amount){
    if(amount==0){
        return false;
    }
    switch(csr_pos){
        case 0:
            return decrease_amount(user.cores.ancient_core, amount);
        case 1:
            return decrease_amount(user.cores.mysterious_shard, amount);
        case 2:
            return decrease_amount(user.cores.crystallium, amount);
        case 3:
            return decrease_amount(user.cores.crystal_core, amount);
        case 4:
            return decrease_amount(user.materials.common, amount);
        case 5:
            return decrease_amount(user.materials.uncommon, amount);
        case 6:
            return decrease_amount(user.materials.rare, amount);
        case 7:
            return decrease_amount(user.materials.epic, amount);
        case 8:
            return decrease_amount(user.materials.legendary, amount);
        case 9:
            return decrease_amount(user.materials.artifact, amount);
        case 10:
            return decrease_amount(user.blueprint.helmet, amount);
        case 11:
            return decrease_amount(user.blueprint.chestplate, amount);
        case 12:
            return decrease_amount(user.blueprint.greaves, amount);
        case 13:
            return decrease_amount(user.blueprint.boots, amount);
        case 14:
            return decrease_amount(user.blueprint.shield, amount);
        case 15:
            return decrease_amount(user.blueprint.weapon, amount);
        case 16:
            return decrease_amount(user.heal_amount, amount);
        default:
            return false;
    }
}

void add_misc_item(Miscellaneous &user, unsigned int csr_pos, unsigned int amount){
    switch(csr_pos){
        case 0:
            user.cores.ancient_core+=amount;
            break;
        case 1:
            user.cores.mysterious_shard+=amount;
            break;
        case 2:
            user.cores.crystallium+=amount;
            break;
        case 3:
            user.cores.crystal_core+=amount;
            break;
        case 4:
            user.materials.common+=amount;
            break;
        case 5:
            user.materials.uncommon+=amount;
            break;
        case 6:
            user.materials.rare+=amount;
            break;
        case 7:
            user.materials.epic+=amount;
            break;
        case 8:
            user.materials.legendary+=amount;
            break;
        case 9:
            user.materials.artifact+=amount;
            break;
        case 10:
            user.blueprint.helmet+=amount;
            break;
        case 11:
            user.blueprint.chestplate+=amount;
            break;
        case 12:
            user.blueprint.greaves+=amount;
            break;
        case 13:
            user.blueprint.boots+=amount;
            break;
        case 14:
            user.blueprint.shield+=amount;
            break;
        case 15:
            user.blueprint.weapon+=amount;
            break;
        case 16:
            user.heal_amount+=amount;
            break;
        default:
            break;
    }
}

void print_misc_item(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    tcod::print(main_win, {0, 1}, "Cores:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "Ancient Cores: "+std::to_string(user.cores.ancient_core), DARK_RED, 2, csr_pos==0);
    print_bold_with_condition(main_win, context, "Mysterious Shard: "+std::to_string(user.cores.mysterious_shard), DARK_RED, 3, csr_pos==1);
    print_bold_with_condition(main_win, context, "Crystallium: "+std::to_string(user.cores.crystallium), YELLOW, 4, csr_pos==2);
    print_bold_with_condition(main_win, context, "Crystal Cores: "+std::to_string(user.cores.crystal_core), PURPLE, 5, csr_pos==3);
    tcod::print(main_win, {0, 7}, "Materials:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "Common: "+std::to_string(user.materials.common), WHITE, 8, csr_pos==4);
    print_bold_with_condition(main_win, context, "Uncommon: "+std::to_string(user.materials.uncommon), GREEN, 9, csr_pos==5);
    print_bold_with_condition(main_win, context, "Rare: "+std::to_string(user.materials.rare), BLUE, 10, csr_pos==6);
    print_bold_with_condition(main_win, context, "Epic: "+std::to_string(user.materials.epic), PURPLE, 11, csr_pos==7);
    print_bold_with_condition(main_win, context, "Legendary: "+std::to_string(user.materials.legendary), YELLOW, 12, csr_pos==8);
    print_bold_with_condition(main_win, context, "Artifact: "+std::to_string(user.materials.artifact), DARK_RED, 13, csr_pos==9);
    tcod::print(main_win, {0, 15}, "Blueprints:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "Helmet: "+std::to_string(user.blueprint.helmet), WHITE, 16, csr_pos==10);
    print_bold_with_condition(main_win, context, "Chestplate: "+std::to_string(user.blueprint.chestplate), WHITE, 17, csr_pos==11);
    print_bold_with_condition(main_win, context, "Greaves: "+std::to_string(user.blueprint.greaves), WHITE, 18, csr_pos==12);
    print_bold_with_condition(main_win, context, "Boots: "+std::to_string(user.blueprint.boots), WHITE, 19, csr_pos==13);
    print_bold_with_condition(main_win, context, "Shield: "+std::to_string(user.blueprint.shield), WHITE, 20, csr_pos==14);
    print_bold_with_condition(main_win, context, "Weapon: "+std::to_string(user.blueprint.weapon), WHITE, 21, csr_pos==15);
    tcod::print(main_win, {0, 23}, "Others:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "First-Aid Kits: "+std::to_string(user.heal_amount), WHITE, 24, csr_pos==16);
    context->present(main_win);
}

void show_misc_items(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user){
    unsigned int csr_pos=0;
    int ch;
    clear_and_draw_dialog(main_win, context, "Miscallaneous items:");
    while(true){
        print_misc_item(main_win, context, user, csr_pos);
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<16){
            clear_and_draw_dialog(main_win, context, "Miscallaneous items:");
            csr_pos++;
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "Miscallaneous items:");
            csr_pos--;
        }
        else if(ch=='r'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be trashed: ");
            if(!remove_misc_item(user, csr_pos, quantity)){
                clear_and_draw_dialog(main_win, context, "[System] Invalid amount");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
        }
        else if(ch=='q'||ch=='m'){
            return;
        }
    }
}

bool decrease_materials(Rarity rarity, Miscellaneous &misc, unsigned int amount){
    if(rarity==Rarity::COMMON&&misc.materials.common>=amount){
        misc.materials.common-=amount;
        return true;
    }
    else if(rarity==Rarity::UNCOMMON&&misc.materials.uncommon>=amount){
        misc.materials.uncommon-=amount;
        return true;
    }
    else if(rarity==Rarity::RARE&&misc.materials.rare>=amount){
        misc.materials.rare-=amount;
        return true;
    }
    else if(rarity==Rarity::EPIC&&misc.materials.epic>=amount){
        misc.materials.epic-=amount;
        return true;
    }
    else if(rarity==Rarity::LEGENDARY&&misc.materials.legendary>=amount){
        misc.materials.legendary-=amount;
        return true;
    }
    else if(rarity==Rarity::ARTIFACT&&misc.materials.artifact>=amount){
        misc.materials.artifact-=amount;
        return true;
    }
    return false;
}

bool enhance_item(Item *cur_item, Miscellaneous &misc, unsigned int amount){
    bool require_enhance=false;
    if(decrease_materials(cur_item->rarity, misc, amount)){
        cur_item->enhancement+=amount;
        cur_item->reinitialize_item();
        return true;
    }
    return false;
}

void draw_crafting_selection(tcod::Console &main_win, tcod::ContextPtr &context, const Miscellaneous &misc, unsigned int csr_pos, int blueprint_selection, int material_selection){
    SDL_wclear_main_win(main_win, context);
    tcod::print(main_win, {0, 1}, "Select a blueprint:", WHITE, BLACK);
    if(csr_pos==1||blueprint_selection==1){
        tcod::print(main_win, {0, 2}, "Helmet ["+std::to_string(misc.blueprint.helmet)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 2}, "Helmet ["+std::to_string(misc.blueprint.helmet)+"]", WHITE, BLACK);
    }
    if(csr_pos==2||blueprint_selection==2){
        tcod::print(main_win, {0, 3}, "Chestplate ["+std::to_string(misc.blueprint.chestplate)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 3}, "Chestplate ["+std::to_string(misc.blueprint.chestplate)+"]", WHITE, BLACK);
    }
    if(csr_pos==3||blueprint_selection==3){
        tcod::print(main_win, {0, 4}, "Greaves ["+std::to_string(misc.blueprint.greaves)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 4}, "Greaves ["+std::to_string(misc.blueprint.greaves)+"]", WHITE, BLACK);
    }
    if(csr_pos==4||blueprint_selection==4){
        tcod::print(main_win, {0, 5}, "Boots ["+std::to_string(misc.blueprint.boots)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 5}, "Boots ["+std::to_string(misc.blueprint.boots)+"]", WHITE, BLACK);
    }
    if(csr_pos==5||blueprint_selection==5){
        tcod::print(main_win, {0, 6}, "Shield ["+std::to_string(misc.blueprint.shield)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 6}, "Shield ["+std::to_string(misc.blueprint.shield)+"]", WHITE, BLACK);
    }
    if(csr_pos==6||blueprint_selection==6){
        tcod::print(main_win, {0, 7}, "Weapon ["+std::to_string(misc.blueprint.weapon)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 7}, "Weapon ["+std::to_string(misc.blueprint.weapon)+"]", WHITE, BLACK);
    }
    tcod::print(main_win, {0, 9}, "Select the material:", WHITE, BLACK);
    if(csr_pos==9||material_selection==9){
        tcod::print(main_win, {0, 10}, "Common ["+std::to_string(misc.materials.common)+"]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 10}, "Common ["+std::to_string(misc.materials.common)+"]", WHITE, BLACK);
    }
    if(csr_pos==10||material_selection==10){
        tcod::print(main_win, {0, 11}, "Uncommon ["+std::to_string(misc.materials.uncommon)+"]", BLACK, GREEN);
    }
    else{
        tcod::print(main_win, {0, 11}, "Uncommon ["+std::to_string(misc.materials.uncommon)+"]", GREEN, BLACK);
    }
    if(csr_pos==11||material_selection==11){
        tcod::print(main_win, {0, 12}, "Rare ["+std::to_string(misc.materials.rare)+"]", BLACK, BLUE);
    }
    else{
        tcod::print(main_win, {0, 12}, "Rare ["+std::to_string(misc.materials.rare)+"]", BLUE, BLACK);
    }
    if(csr_pos==12||material_selection==12){
        tcod::print(main_win, {0, 13}, "Epic ["+std::to_string(misc.materials.epic)+"]", BLACK, PURPLE);
    }
    else{
        tcod::print(main_win, {0, 13}, "Epic ["+std::to_string(misc.materials.epic)+"]", PURPLE, BLACK);
    }
    if(csr_pos==13||material_selection==13){
        tcod::print(main_win, {0, 14}, "Legendary ["+std::to_string(misc.materials.legendary)+"]", BLACK, YELLOW);
    }
    else{
        tcod::print(main_win, {0, 14}, "Legendary ["+std::to_string(misc.materials.legendary)+"]", YELLOW, BLACK);
    }
    if(csr_pos==14||material_selection==14){
        tcod::print(main_win, {0, 15}, "Artifact ["+std::to_string(misc.materials.artifact)+"]", BLACK, DARK_RED);
    }
    else{
        tcod::print(main_win, {0, 15}, "Artifact ["+std::to_string(misc.materials.artifact)+"]", DARK_RED, BLACK);
    }
    if(csr_pos==16){
        tcod::print(main_win, {0, 17}, "[Done]", BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, 17}, "[Done]", WHITE, BLACK);
    }
    clear_and_draw_dialog(main_win, context, "[Blacksmith] What do you want to craft today?");
}

bool check_if_sufficient_materials(Miscellaneous &misc, int material_selection, int required_material, Rarity &rarity){
    switch(material_selection){
        case 9: // common
            if(misc.materials.common>=required_material){
                misc.materials.common-=required_material;
                rarity=Rarity::COMMON;
                return true;
            }
            else{
                return false;
            }
            break;
        case 10: // uncommon
            if(misc.materials.uncommon>=required_material){
                misc.materials.uncommon-=required_material;
                rarity=Rarity::UNCOMMON;
                return true;
            }
            else{
                return false;
            }
            break;
        case 11: // rare
            if(misc.materials.rare>=required_material){
                misc.materials.rare-=required_material;
                rarity=Rarity::RARE;
                return true;
            }
            else{
                return false;
            }
            break;
        case 12: // epic
            if(misc.materials.epic>=required_material){
                misc.materials.epic-=required_material;
                rarity=Rarity::EPIC;
                return true;
            }
            else{
                return false;
            }
            break;
        case 13: // legendary
            if(misc.materials.legendary>=required_material){
                misc.materials.legendary-=required_material;
                rarity=Rarity::LEGENDARY;
                return true;
            }
            else{
                return false;
            }
            break;
        case 14: // artifact
            if(misc.materials.artifact>=required_material){
                misc.materials.artifact-=required_material;
                rarity=Rarity::ARTIFACT;
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

bool check_if_craft_valid(int blueprint_selection, int material_selection, Miscellaneous &misc, Type &gear_type, Rarity &rarity){
    switch(blueprint_selection){
        case 1: // helmet
            if(misc.blueprint.helmet>0){
                if(check_if_sufficient_materials(misc, material_selection, 3, rarity)){
                    misc.blueprint.helmet--;
                    gear_type=Type::HELMET;
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
                    gear_type=Type::CHESTPLATE;
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
                    gear_type=Type::GREAVES;
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
                    gear_type=Type::BOOTS;
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
                    gear_type=Type::SHIELD;
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
                    gear_type=Type::WEAPON;
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

void crafting_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user){
    unsigned int csr_pos=1;
    int blueprint_selection=0, material_selection=0;
    int ch;
    while(true){
        draw_crafting_selection(main_win, context, user.inv.misc, csr_pos, blueprint_selection, material_selection);
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
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>1){
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
        else if(ch==SDLK_TAB){ // KEY_TAB
            if(csr_pos<7){
                csr_pos+=8;
            }
            else if(csr_pos<15){
                csr_pos-=8;
            }
        }
        else if(ch==SDLK_RETURN){ // KEY_ENTER
            if(csr_pos<7){
                blueprint_selection=csr_pos;
            }
            else if(csr_pos<15){
                material_selection=csr_pos;
            }
            if(csr_pos==16&&blueprint_selection!=0&&material_selection!=0){
                Type gear_type;
                Rarity rarity;
                if(check_if_craft_valid(blueprint_selection, material_selection, user.inv.misc, gear_type, rarity)){
                    user.add_item(craft_item(rarity, gear_type));
                    return;
                }
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void salvage_item(Player &user, Item *item){
    switch(item->rarity){
        case Rarity::COMMON:
            user.inv.misc.materials.common+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::UNCOMMON:
            user.inv.misc.materials.uncommon+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::RARE:
            user.inv.misc.materials.rare+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::EPIC:
            user.inv.misc.materials.epic+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::LEGENDARY:
            user.inv.misc.materials.legendary+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::ARTIFACT:
            user.inv.misc.materials.artifact+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        default:
            break;
    }
    user.uninitialize_stats();
    user.remove_item(item);
    user.initialize_stats();
}

void inventory_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config){
    bool is_modifier_called = false;
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    init_copy(user.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(user.inv.item, items_copy, perm_config);
    }
    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if((ch=='a'||ch==SDLK_LEFT)&&page_num>0){
            page_num--;
            csr_pos=0;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if((ch=='d'||ch==SDLK_RIGHT)&&((page_num+1)*30<items_copy.size())){
            page_num++;
            csr_pos=0;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='e'){
            if(!items_copy[csr_pos+(page_num*30)]->is_equipped&&items_copy[csr_pos+(page_num*30)]->durability>0){ // if the current item is unequipped
                equip_item(user, items_copy[csr_pos+(page_num*30)]);
                draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
            }
            else if(items_copy[csr_pos+(page_num*30)]->is_equipped){
                unequip_item(user, items_copy[csr_pos+(page_num*30)]);
                draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
            }
        }
        else if(ch=='r'){
            items_copy[csr_pos+(page_num*30)]->name=get_string(main_win, context, "([ESC] to cancel) Rename item to: ", items_copy[csr_pos+(page_num*30)]->name);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='m'){
            show_misc_items(main_win, context, user.inv.misc);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='H'){
            help_mode(main_win, context, "inventory_mode");
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='R'){
            if(SDL_getch_y_or_n(main_win, context, "[System] Delete item? (You can salvage instead at the blacksmith) [y/n]")){
                if(unequip_item(user, items_copy[csr_pos+(page_num*30)])){
                    user.remove_item(items_copy[csr_pos+(page_num*30)]);
                    if(user.inv.item.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    if(is_modifier_called){
                        process_copy(user.inv.item, items_copy, perm_config);
                    }
                    else{
                        init_copy(user.inv.item, items_copy);
                    }
                }
            }
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='-'){ // Inventory Modifier
            inventory_modifier_selection(main_win, context, perm_config, user.inv.item, items_copy);
            process_copy(user.inv.item, items_copy, perm_config);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
    }
}

void reforge_repair_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config){
    bool is_modifier_called = false;
    std::vector<Item *> items_copy;
    init_copy(user.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(user.inv.item, items_copy, perm_config);
    }
    bool safety_mode=true;
    unsigned int page_num=0;
    int csr_pos=0;
    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if((ch=='a'||ch==SDLK_LEFT)&&(page_num>0)){
            page_num--;
            csr_pos=0;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if((ch=='d'||ch==SDLK_RIGHT)&&((page_num+1)*30<items_copy.size())){
            page_num++;
            csr_pos=0;
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='m'){
            show_misc_items(main_win, context, user.inv.misc);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
        }
        else if(ch=='-'){ // Inventory Modifier
            inventory_modifier_selection(main_win, context, perm_config, user.inv.item, items_copy);
            process_copy(user.inv.item, items_copy, perm_config);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='M'){ // Multiselect salvage (std::queue)
            if(!safety_mode){
                safety_mode=true;
                clear_and_draw_dialog(main_win, context, "Safety Mode ON");
            }
            else{
                safety_mode=false;
                clear_and_draw_dialog(main_win, context, "Safety Mode OFF [Salvaging will now not require confirmation]");
            }
        }
        else if(ch=='r'){ // Repair
            if(items_copy[csr_pos+(page_num*30)]->durability<100.0){
                const unsigned int cost=(100.0-items_copy[csr_pos+(page_num*30)]->durability)*rarity_value(items_copy[csr_pos+(page_num*30)]->rarity)*15;
                const unsigned int material_cost=2*(1+((100-items_copy[csr_pos+(page_num*30)]->durability)/100));
                std::stringstream ss;
                ss << "[System] Attempt repair? Gold:" << cost << " Material:" << material_cost << " [y/n]";
                if(SDL_getch_y_or_n(main_win, context, ss.str())){
                    if(user.gold>=cost&&decrease_materials(items_copy[csr_pos+(page_num*30)]->rarity, user.inv.misc, material_cost)){
                        user.gold-=cost;
                        draw_stats(main_win, context, user);
                        if(return_chance(items_copy[csr_pos+(page_num*30)]->durability*2)){
                            items_copy[csr_pos+(page_num*30)]->durability=100.0;
                            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                            clear_and_draw_dialog(main_win, context, "[System] Success: Item Repaired.");
                        }
                        else{
                            clear_and_draw_dialog(main_win, context, "[System] Error: Repair failed.");
                        }
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Insufficient gold/materials");
                    }
                }
                else{
                    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                }
            }
        }
        else if(ch=='e'){ // Enhance
            unsigned int amount=get_int(main_win, context, "[System] Amount of materials you want to use in this enhancement: ");
            if(amount>0){
                user.uninitialize_stats();
                if(enhance_item(items_copy[csr_pos+(page_num*30)], user.inv.misc, amount)){
                    user.initialize_stats();
                    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                    clear_and_draw_dialog(main_win, context, "[System] Success");
                }
                else{
                    user.initialize_stats();
                    clear_and_draw_dialog(main_win, context, "[System] Failure");
                }
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Failure");
            }
        }
        else if(ch=='R'){ // Reforge
            if(SDL_getch_y_or_n(main_win, context, "[System] Reforge item? (This action is irreversible) [y/n]")){
                unsigned int gold_cost=rarity_value(items_copy[csr_pos+(page_num*30)]->rarity)*200;
                unsigned int material_cost=2*(1+((100-items_copy[csr_pos+(page_num*30)]->durability)/100.0));
                std::stringstream ss;
                ss << "Proceed? Cost of reforge: " << gold_cost << " Gold, " << material_cost << " Materials [y/n]";
                if(SDL_getch_y_or_n(main_win, context, ss.str())&&decrease_materials(items_copy[csr_pos+(page_num*30)]->rarity, user.inv.misc, material_cost)&&user.gold>=gold_cost){
                    user.gold-=gold_cost;
                    bool use_crystallium=false, use_ancient_core=false;
                    unsigned int crystallium_amount=get_int(main_win, context, "Use Crystallium? [0 to not use any]: ");
                    unsigned int ancient_core_amount=0;
                    if(crystallium_amount==0){
                        ancient_core_amount=get_int(main_win, context, "Use Ancient Cores? [0 to not use any]: ");
                        if(ancient_core_amount>0){
                            use_ancient_core=true;
                        }
                    }
                    else if(crystallium_amount>0){
                        use_crystallium=true;
                    }
                    if(user.inv.misc.cores.ancient_core>=ancient_core_amount&&user.inv.misc.cores.crystallium>=crystallium_amount){
                        user.inv.misc.cores.ancient_core-=ancient_core_amount;
                        user.inv.misc.cores.crystallium-=crystallium_amount;
                        user.uninitialize_stats();
                        reforge_item(ancient_core_amount, crystallium_amount, items_copy[csr_pos+(page_num*30)]);
                        items_copy[csr_pos+(page_num*30)]->reinitialize_item();
                        user.initialize_stats();
                        draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                        draw_stats(main_win, context, user);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Insufficient materials");
                    }
                }
                else{
                    clear_and_draw_dialog(main_win, context, "[System] Aborted");
                }
            }
            else{
                draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
            }
        }
        else if(ch=='c'){
            crafting_mode(main_win, context, user);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
        }
        else if(ch=='S'){
            if(safety_mode){
                if(SDL_getch_y_or_n(main_win, context, "Salvage item? (This action is irreversible) [y/n]")&&unequip_item(user, items_copy[csr_pos+(page_num*30)])){
                    salvage_item(user, items_copy[csr_pos+(page_num*30)]);
                    if(user.inv.item.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    if(is_modifier_called){
                        process_copy(user.inv.item, items_copy, perm_config);
                    }
                    else{
                        init_copy(user.inv.item, items_copy);
                    }
                    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                }
                else{
                    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                }
            }
            else{
                if(unequip_item(user, items_copy[csr_pos+(page_num*30)])){
                    salvage_item(user, items_copy[csr_pos+(page_num*30)]);
                    if(user.inv.item.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    if(is_modifier_called){
                        process_copy(user.inv.item, items_copy, perm_config);
                    }
                    else{
                        init_copy(user.inv.item, items_copy);
                    }
                    draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
                    clear_and_draw_dialog(main_win, context, "[System] Salvage success");
                }
                else{
                    clear_and_draw_dialog(main_win, context, "[System] Salvage Failure");
                }
            }
        }
        else if(ch=='H'){
            help_mode(main_win, context, "blacksmith_mode");
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos, true);
        }
    }
}

