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

#include "headers/main.h"
#include "headers/classes.h"
#include "headers/mode.h"

void reset_inv_params(No_Delete &perm_config){
    perm_config.default_show_type_type=Type::ALL;
    perm_config.default_show_rarity_type=Rarity::ALL;
    perm_config.default_sort_rarity_method=DEFAULT_SHOW_SELECTION;
    perm_config.only_show_equipped=false;
}

void init_copy(std::vector<Item> &original, std::vector<Item *> &items_copy){
    items_copy.clear();
    for(int i=0; i<original.size(); i++){
        items_copy.push_back(&original[i]);
    }
}

void process_copy(std::vector<Item> &original, std::vector<Item *> &items_copy, No_Delete &perm_config){
    init_copy(original, items_copy);
    if(perm_config.only_show_equipped){ // Only show Equipped
        items_copy.erase(std::remove_if(items_copy.begin(), items_copy.end(), [](const Item *v){return v->is_equipped==false;}), items_copy.end());
    }
    if(perm_config.default_show_rarity_type!=Rarity::ALL){ // Only show certain rarity
        items_copy.erase(std::remove_if(items_copy.begin(), items_copy.end(), [perm_config](const Item *v){return v->rarity!=perm_config.default_show_rarity_type;}), items_copy.end());
    }
    if(perm_config.default_show_type_type!=Type::ALL){ // Only show certain types
        items_copy.erase(std::remove_if(items_copy.begin(), items_copy.end(), [perm_config](const Item *v){return v->type!=perm_config.default_show_type_type;}), items_copy.end());
    }
    if(perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_ASCENDING){ // Sort by rarity ascending
        std::sort(items_copy.begin(), items_copy.end(), sort_ascending);
    }
    if(perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_DESCENDING){ // Sort by rarity descending
        std::sort(items_copy.begin(), items_copy.end(), sort_descending);
    }
    if(original.empty()){
        return;
    }
    if(items_copy.empty()){
        reset_inv_params(perm_config);
        process_copy(original, items_copy, perm_config);
    }
}

void print_item(tcod::Console &main_win, tcod::ContextPtr &context, bool curr_pos_matches_condition, bool curr_config_matches_condition, int line, const std::string &output, bool is_valid=true){
    if(!is_valid){
        tcod::print(main_win, {0, line}, "["+output+"]", GRAY, BLACK);
        return;
    }
    if(curr_pos_matches_condition){
        if(curr_config_matches_condition){
            tcod::print(main_win, {0, line}, "->["+output+"]", BLACK, WHITE);
        }
        else{
            tcod::print(main_win, {0, line}, "["+output+"]", BLACK, WHITE);
        }
    }
    else{
        if(curr_config_matches_condition){
            tcod::print(main_win, {0, line}, "->["+output+"]", WHITE, BLACK);
        }
        else{
            tcod::print(main_win, {0, line}, "["+output+"]", WHITE, BLACK);
        }
    }
}

bool check_if_rarity_valid(const std::vector<Item *> &items_copy, Rarity check){
    for(int i=0; i<items_copy.size(); i++){
        if(items_copy[i]->rarity==check){
            return true;
        }
    }
    return false;
}

bool check_if_type_valid(const std::vector<Item *> &items_copy, Type check){
    for(int i=0; i<items_copy.size(); i++){
        if(items_copy[i]->type==check){
            return true;
        }
    }
    return false;
}

bool check_if_equip_valid(const std::vector<Item *> &items_copy){
    for(int i=0; i<items_copy.size(); i++){
        if(items_copy[i]->is_equipped){
            return true;
        }
    }
    return false;
}

void draw_inventory_modifier_selection(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<Item *> &items_copy, const No_Delete &perm_config, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Inventory Modifier Selection", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "Show only [Rarity]:", WHITE, BLACK);
    print_item(main_win, context, csr_pos==0, perm_config.default_show_rarity_type==Rarity::ALL, 2, "All");
    print_item(main_win, context, csr_pos==1, perm_config.default_show_rarity_type==Rarity::COMMON, 3, "Common", check_if_rarity_valid(items_copy, Rarity::COMMON));
    print_item(main_win, context, csr_pos==2, perm_config.default_show_rarity_type==Rarity::UNCOMMON, 4, "Uncommon", check_if_rarity_valid(items_copy, Rarity::UNCOMMON));
    print_item(main_win, context, csr_pos==3, perm_config.default_show_rarity_type==Rarity::RARE, 5, "Rare", check_if_rarity_valid(items_copy, Rarity::RARE));
    print_item(main_win, context, csr_pos==4, perm_config.default_show_rarity_type==Rarity::EPIC, 6, "Epic", check_if_rarity_valid(items_copy, Rarity::EPIC));
    print_item(main_win, context, csr_pos==5, perm_config.default_show_rarity_type==Rarity::LEGENDARY, 7, "Legendary", check_if_rarity_valid(items_copy, Rarity::LEGENDARY));
    print_item(main_win, context, csr_pos==6, perm_config.default_show_rarity_type==Rarity::ARTIFACT, 8, "Artifact", check_if_rarity_valid(items_copy, Rarity::ARTIFACT));
    tcod::print(main_win, {0, 10}, "Show only [Type]:", WHITE, BLACK);
    print_item(main_win, context, csr_pos==7, perm_config.default_show_type_type==Type::ALL, 11, "All");
    print_item(main_win, context, csr_pos==8, perm_config.default_show_type_type==Type::HELMET, 12, "Helmet", check_if_type_valid(items_copy, Type::HELMET));
    print_item(main_win, context, csr_pos==9, perm_config.default_show_type_type==Type::CHESTPLATE, 13, "Chestplate", check_if_type_valid(items_copy, Type::CHESTPLATE));
    print_item(main_win, context, csr_pos==10, perm_config.default_show_type_type==Type::GREAVES, 14, "Greaves", check_if_type_valid(items_copy, Type::GREAVES));
    print_item(main_win, context, csr_pos==11, perm_config.default_show_type_type==Type::BOOTS, 15, "Boots", check_if_type_valid(items_copy, Type::BOOTS));
    print_item(main_win, context, csr_pos==12, perm_config.default_show_type_type==Type::SHIELD, 16, "Shield", check_if_type_valid(items_copy, Type::SHIELD));
    print_item(main_win, context, csr_pos==13, perm_config.default_show_type_type==Type::WEAPON, 17, "Weapon", check_if_type_valid(items_copy, Type::WEAPON));
    tcod::print(main_win, {0, 19}, "Show only [Equipped]:", WHITE, BLACK);
    print_item(main_win, context, csr_pos==14, !perm_config.only_show_equipped, 20, "All");
    print_item(main_win, context, csr_pos==15, perm_config.only_show_equipped, 21, "Equipped Only", check_if_equip_valid(items_copy));
    tcod::print(main_win, {0, 23}, "Sort by [Rarity]:", WHITE, BLACK);
    print_item(main_win, context, csr_pos==16, perm_config.default_sort_rarity_method==DEFAULT_SHOW_SELECTION, 24, "None");
    print_item(main_win, context, csr_pos==17, perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_ASCENDING, 25, "Ascending");
    print_item(main_win, context, csr_pos==18, perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_DESCENDING, 26, "Descending");
    print_item(main_win, context, csr_pos==19, perm_config.show_current_item_compared_to_equipped, 28, "Show difference between current and equipped item");
    print_item(main_win, context, csr_pos==20, perm_config.keep_changes_persistent, 30, "Keep Changes Persistent");
    print_item(main_win, context, csr_pos==21, false, 32, "Reset To Defaults");
    print_item(main_win, context, csr_pos==22, false, 34, "Done");
    context->present(main_win);
}

void change_config(No_Delete &perm_config, unsigned int csr_pos, const std::vector<Item *> &items_copy){
    switch(csr_pos){
        case 0:
            perm_config.default_show_rarity_type=Rarity::ALL;
            break;
        case 1:
            perm_config.default_show_rarity_type=Rarity::COMMON;
            break;
        case 2:
            perm_config.default_show_rarity_type=Rarity::UNCOMMON;
            break;
        case 3:
            perm_config.default_show_rarity_type=Rarity::RARE;
            break;
        case 4:
            perm_config.default_show_rarity_type=Rarity::EPIC;
            break;
        case 5:
            perm_config.default_show_rarity_type=Rarity::LEGENDARY;
            break;
        case 6:
            perm_config.default_show_rarity_type=Rarity::ARTIFACT;
            break;
        case 7:
            perm_config.default_show_type_type=Type::ALL;
            break;
        case 8:
            perm_config.default_show_type_type=Type::HELMET;
            break;
        case 9:
            perm_config.default_show_type_type=Type::CHESTPLATE;
            break;
        case 10:
            perm_config.default_show_type_type=Type::GREAVES;
            break;
        case 11:
            perm_config.default_show_type_type=Type::BOOTS;
            break;
        case 12:
            perm_config.default_show_type_type=Type::SHIELD;
            break;
        case 13:
            perm_config.default_show_type_type=Type::WEAPON;
            break;
        case 14:
            perm_config.only_show_equipped=false;
            break;
        case 15:
            perm_config.only_show_equipped=true;
            break;
        case 16:
            perm_config.default_sort_rarity_method=DEFAULT_SHOW_SELECTION;
            break;
        case 17:
            perm_config.default_sort_rarity_method=SORT_TYPE_RARITY_ASCENDING;
            break;
        case 18:
            perm_config.default_sort_rarity_method=SORT_TYPE_RARITY_DESCENDING;
            break;
        case 19:
            if(perm_config.show_current_item_compared_to_equipped){
                perm_config.show_current_item_compared_to_equipped=false;
            }
            else{
                perm_config.show_current_item_compared_to_equipped=true;
            }
            break;
        case 20:
            if(perm_config.keep_changes_persistent){
                perm_config.keep_changes_persistent=false;
            }
            else{
                perm_config.keep_changes_persistent=true;
            }
            break;
        default:
            break;
    }
}

bool check_if_csr_move_valid(const unsigned int &csr_pos, const std::vector<Item *> &items_copy){
    switch(csr_pos){
        case 1:
            if(check_if_rarity_valid(items_copy, Rarity::COMMON)){
                return true;
            }
            break;
        case 2:
            if(check_if_rarity_valid(items_copy, Rarity::UNCOMMON)){
                return true;
            }
            break;
        case 3:
            if(check_if_rarity_valid(items_copy, Rarity::RARE)){
                return true;
            }
            break;
        case 4:
            if(check_if_rarity_valid(items_copy, Rarity::EPIC)){
                return true;
            }
            break;
        case 5:
            if(check_if_rarity_valid(items_copy, Rarity::LEGENDARY)){
                return true;
            }
            break;
        case 6:
            if(check_if_rarity_valid(items_copy, Rarity::ARTIFACT)){
                return true;
            }
            break;
        case 8:
            if(check_if_type_valid(items_copy, Type::HELMET)){
                return true;
            }
            break;
        case 9:
            if(check_if_type_valid(items_copy, Type::CHESTPLATE)){
                return true;
            }
            break;
        case 10:
            if(check_if_type_valid(items_copy, Type::GREAVES)){
                return true;
            }
            break;
        case 11:
            if(check_if_type_valid(items_copy, Type::BOOTS)){
                return true;
            }
            break;
        case 12:
            if(check_if_type_valid(items_copy, Type::SHIELD)){
                return true;
            }
            break;
        case 13:
            if(check_if_type_valid(items_copy, Type::WEAPON)){
                return true;
            }
            break;
        case 15:
            if(check_if_equip_valid(items_copy)){
                return true;
            }
            break;
        default:
            return true;
    }
    return false;
}

void repeat_down_until_valid_move(unsigned int &csr_pos, const std::vector<Item *> &items_copy){
    csr_pos++;
    if(!check_if_csr_move_valid(csr_pos, items_copy)){
        repeat_down_until_valid_move(csr_pos, items_copy);
    }
}

void repeat_up_until_valid_move(unsigned int &csr_pos, const std::vector<Item *> &items_copy){
    csr_pos--;
    if(!check_if_csr_move_valid(csr_pos, items_copy)){
        repeat_up_until_valid_move(csr_pos, items_copy);
    }
}

void inventory_modifier_selection(tcod::Console &main_win, tcod::ContextPtr &context, No_Delete &perm_config, std::vector<Item> original_copy, std::vector<Item *> &items_copy){
    unsigned int csr_pos=0;
    int ch;
    while(true){
        draw_inventory_modifier_selection(main_win, context, items_copy, perm_config, csr_pos);
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<22){
            repeat_down_until_valid_move(csr_pos, items_copy);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            repeat_up_until_valid_move(csr_pos, items_copy);
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==21){
                reset_inv_params(perm_config);
                process_copy(original_copy, items_copy, perm_config);
            }
            if(csr_pos==22){
                return;
            }
            change_config(perm_config, csr_pos, items_copy);
        }
        else if(ch=='q'){
            return;
        }
    }
}
