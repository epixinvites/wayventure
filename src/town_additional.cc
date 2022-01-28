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

#include <iomanip>
#include "headers/main.h"
#include "headers/mode.h"
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"

int calculate_price(const Item &cur, double relationship, bool player_sell){
    int price=100*rarity_value(cur.rarity)*rarity_value(cur.rarity);
    if(cur.def>0&&cur.type!=Type::GREAVES){
        price*=1.5+(cur.def/100.0);
    }
    else{
        price*=1.5+(cur.def/500.0);
    }
    if(cur.crit_dmg>0){
        price*=1.5+(cur.crit_dmg/25.0);
    }
    if(cur.crit_chance>0){
        price*=1.5+(cur.crit_chance/25.0);
    }
    switch(cur.type){
        case Type::HELMET:
            price*=cur.hp/(40*rarity_value(cur.rarity))*2;
            break;
        case Type::CHESTPLATE:
            price*=cur.hp/(100*rarity_value(cur.rarity))*2;
            break;
        case Type::GREAVES:
            price*=cur.def/(50*rarity_value(cur.rarity))*2;
            break;
        case Type::BOOTS:
            price*=cur.def/(30*rarity_value(cur.rarity))*2;
            break;
        case Type::SHIELD:
            price*=cur.shield/(100*rarity_value(cur.rarity))*2;
            break;
        case Type::WEAPON:
            price*=cur.attk/(200*rarity_value(cur.rarity))*4;
            break;
        default:
            break;
    }
    price*=1+((100-relationship)/100);
    if(player_sell){
        return (price*0.8)*(cur.durability/100.0);
    }
    return price;
}

void print_item_description_1(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int &line){
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
}

void print_item_description_2(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int &line){
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
}

void print_trader_item_description(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price, bool is_locked){
    if(is_locked){
        tcod::print(main_win, {0, line+1}, "[Locked]", WHITE, BLACK);
        line++;
    }
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(main_win, {0, line+1}, "Price: "+std::to_string(price), WHITE, BLACK);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(main_win);
}

void print_mysterious_trader_item_description(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, double quality_points){
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(main_win, {0, line+1}, "Quality Points: "+std::to_string(quality_points), WHITE, BLACK);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(main_win);
}

void print_sell_item_description(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price){
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(main_win, {0, line+1}, "Equipped: ", WHITE, BLACK);
    if(cur_item->is_equipped){
        tcod::print(main_win, {10, line+1}, "true", GREEN, BLACK);
    }
    if(!cur_item->is_equipped){
        tcod::print(main_win, {10, line+1}, "false", LIGHT_RED, BLACK);
    }
    line++;
    tcod::print(main_win, {0, line+1}, "Price: "+std::to_string(price), WHITE, BLACK);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(main_win);
}

void draw_trader_items(tcod::Console &main_win, tcod::ContextPtr &context, const Player &user, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, int price, bool is_locked=false){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, user);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(long long int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_trader_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price, is_locked);
}

void draw_mysterious_trader_items(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<Item *> &items_copy, unsigned int csr_pos, double quality_points){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_base(main_win, context, 34, items_copy.size(), 0, false, false);
    for(int i=0, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_mysterious_trader_item_description(main_win, context, items_copy[csr_pos], 35, quality_points);
}

void draw_sell_items(tcod::Console &main_win, tcod::ContextPtr &context, const Player &user, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, int price){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, user);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(long long int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_sell_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price);
}

void store_misc_items(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user, Chest &chest){
    unsigned int csr_pos=0;
    clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
    int ch;
    while(true){
        print_misc_item(main_win, context, user, csr_pos);
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<16){
            clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
            csr_pos++;
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
            csr_pos--;
        }
        else if(ch=='S'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be stored: ");
            if(remove_misc_item(user, csr_pos, quantity)){
                add_misc_item(chest.misc_storage, csr_pos, quantity);
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Invalid amount");
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void retrieve_misc_items(tcod::Console &main_win, tcod::ContextPtr &context, Miscellaneous &user, Chest &chest){
    unsigned int csr_pos=0;
    clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
    int ch;
    while(true){
        print_misc_item(main_win, context, chest.misc_storage, csr_pos);
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<16){
            clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
            csr_pos++;
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
            csr_pos--;
        }
        else if(ch=='R'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be retrieved: ");
            if(remove_misc_item(chest.misc_storage, csr_pos, quantity)){
                add_misc_item(user, csr_pos, quantity);
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Invalid amount");
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void inventory_storage(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config, Chest &chest){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_called=false;
    init_copy(user.inv.item, items_copy);
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
        else if(ch=='S'){
            if(unequip_item(user, items_copy[csr_pos+page_num*30])){
                chest.gear_storage.insert(chest.gear_storage.begin(), *items_copy[csr_pos+page_num*30]);
                user.remove_item(items_copy[csr_pos+page_num*30]);
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
                draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Error Storing Item");
            }
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='-'){ // Inventory Modifier
            is_modifier_called=true;
            inventory_modifier_selection(main_win, context, perm_config, user.inv.item, items_copy);
            process_copy(user.inv.item, items_copy, perm_config);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
    }
}

void inventory_retrieve(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, No_Delete &perm_config, Chest &chest){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_called=false;
    init_copy(chest.gear_storage, items_copy);
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
        else if(ch=='R'){
            user.add_item(*items_copy[csr_pos+page_num*30]);
            chest.gear_storage.erase(chest.gear_storage.begin()+(csr_pos+page_num*30));
            if(chest.gear_storage.empty()){
                return;
            }
            else if(csr_pos>0){
                csr_pos--;
            }
            else if(csr_pos==0&&page_num>0){
                page_num--;
            }
            if(is_modifier_called){
                process_copy(chest.gear_storage, items_copy, perm_config);
            }
            else{
                init_copy(chest.gear_storage, items_copy);
            }
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='-'){ // Inventory Modifier
            is_modifier_called=true;
            inventory_modifier_selection(main_win, context, perm_config, chest.gear_storage, items_copy);
            process_copy(chest.gear_storage, items_copy, perm_config);
            draw_inventory(main_win, context, user, items_copy, page_num, csr_pos);
        }
    }
}

void show_merchant_items(tcod::Console &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &user){
    unsigned int page_num=0;
    int csr_pos=0;
    std::vector<Item> original_copy;
    for(const auto &i: gear_merchant.store){
        original_copy.push_back(i.first);
    }
    std::vector<Item *> items_copy;
    init_copy(original_copy, items_copy);
    int ch;
    draw_trader_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation));
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            tcod::print(main_win, {0, 39}, empty_line, WHITE, BLACK);
            draw_trader_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            tcod::print(main_win, {0, 39}, empty_line, WHITE, BLACK);
            draw_trader_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
        }
        else if(ch=='L'){
            if(!gear_merchant.store[csr_pos+page_num*30].second){
                gear_merchant.store[csr_pos+page_num*30].second=true;
                draw_trader_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                clear_and_draw_dialog(main_win, context, "Item Locked!");
            }
            else{
                gear_merchant.store[csr_pos+page_num*30].second=false;
                draw_trader_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                clear_and_draw_dialog(main_win, context, "Item Unlocked!");
            }
        }
        else if(ch=='B'){
            if(SDL_getch_y_or_n(main_win, context, "Confirm purchase of item? [y/n]")){
                unsigned int amount=calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation);
                if(amount<=user.gold){
                    user.gold-=amount;
                    draw_stats(main_win, context, user);
                    user.add_item(original_copy[csr_pos+page_num*30]);
                    gear_merchant.store.erase(gear_merchant.store.begin()+(csr_pos+page_num*30));
                    if(gear_merchant.store.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    original_copy.clear();
                    for(const auto &i: gear_merchant.store){
                        original_copy.push_back(i.first);
                    }
                    init_copy(original_copy, items_copy);
                    draw_trader_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                    clear_and_draw_dialog(main_win, context, "[System] Item added to inventory");
                }
                else{
                    clear_and_draw_dialog(main_win, context, "[System] Insufficient gold");
                }
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Transaction aborted");
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void sell_items_to_merchant(tcod::Console &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &user, No_Delete &perm_config){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_active=false;
    init_copy(user.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(user.inv.item, items_copy, perm_config);
    }
    int ch;
    draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(user.inv.item[csr_pos+page_num*30], gear_merchant.relation, true));
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        else if((ch=='a'||ch==SDLK_LEFT)&&page_num>0){
            page_num--;
            csr_pos=0;
            draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        else if((ch=='d'||ch==SDLK_RIGHT)&&((page_num+1)*30<items_copy.size())){
            page_num++;
            csr_pos=0;
            draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        else if(ch=='S'&&items_copy[csr_pos+page_num*30]->rarity!=Rarity::ARTIFACT){
            if(SDL_getch_y_or_n(main_win, context, "Sell item? [y/n]")&&unequip_item(user, items_copy[csr_pos+page_num*30])){
                user.gold+=calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true);
                user.remove_item(items_copy[csr_pos+page_num*30]);
                if(user.inv.item.empty()){
                    return;
                }
                else if(csr_pos>0){
                    csr_pos--;
                }
                else if(csr_pos==0&&page_num>0){
                    page_num--;
                }
                if(is_modifier_active){
                    process_copy(user.inv.item, items_copy, perm_config);
                }
                else{
                    init_copy(user.inv.item, items_copy);
                }
                draw_stats(main_win, context, user);
            }
            draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='-'){ // Inventory Modifier
            is_modifier_active=true;
            inventory_modifier_selection(main_win, context, perm_config, user.inv.item, items_copy);
            process_copy(user.inv.item, items_copy, perm_config);
            draw_sell_items(main_win, context, user, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
    }
}

void print_trader_misc_menu(tcod::Console &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    // Buy
    tcod::print(main_win, {0, 1}, "Buy Materials/Blueprints:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "[Common] "+std::to_string(trader_prices.at("common_material"))+" Gold each", WHITE, 2, csr_pos==0);
    print_bold_with_condition(main_win, context, "[Uncommon] "+std::to_string(trader_prices.at("uncommon_material"))+" Gold each", GREEN, 3, csr_pos==1);
    print_bold_with_condition(main_win, context, "[Rare] "+std::to_string(trader_prices.at("rare_material"))+" Gold each", BLUE, 4, csr_pos==2);
    print_bold_with_condition(main_win, context, "[Epic] "+std::to_string(trader_prices.at("epic_material"))+" Gold each", PURPLE, 5, csr_pos==3);
    print_bold_with_condition(main_win, context, "[Legendary] "+std::to_string(trader_prices.at("legendary_material"))+" Gold each", YELLOW, 6, csr_pos==4);
    tcod::print(main_win, {0, 8}, "Blueprints:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "[Helmet] "+std::to_string(trader_prices.at("helmet_blueprint"))+" Crystal Cores each", WHITE, 9, csr_pos==5);
    print_bold_with_condition(main_win, context, "[Chestplate] "+std::to_string(trader_prices.at("chestplate_blueprint"))+" Crystal Cores each", WHITE, 10, csr_pos==6);
    print_bold_with_condition(main_win, context, "[Greaves] "+std::to_string(trader_prices.at("greaves_blueprint"))+" Crystal Cores each", WHITE, 11, csr_pos==7);
    print_bold_with_condition(main_win, context, "[Boots] "+std::to_string(trader_prices.at("boots_blueprint"))+" Crystal Cores each", WHITE, 12, csr_pos==8);
    print_bold_with_condition(main_win, context, "[Shield] "+std::to_string(trader_prices.at("shield_blueprint"))+" Crystal Cores each", WHITE, 13, csr_pos==9);
    print_bold_with_condition(main_win, context, "[Weapon] "+std::to_string(trader_prices.at("weapon_blueprint"))+" Crystal Cores each", WHITE, 14, csr_pos==10);
    tcod::print(main_win, {0, 16}, "Others:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "[First-Aid Kits] "+std::to_string(trader_prices.at("first_aid_kit"))+" Crystal Cores each", PURPLE, 17, csr_pos==11);
    // Sell
    tcod::print(main_win, {0, 21}, "--------------------------------------------------------------------------------", WHITE, BLACK);
    tcod::print(main_win, {0, 25}, "Sell Cores/Materials:", WHITE, BLACK);
    tcod::print(main_win, {0, 26}, "Cores:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "[Crystal Cores] "+std::to_string((unsigned long long int) (trader_prices.at("crystal_core")*0.8))+" Gold each", BLUE, 27, csr_pos==12);
    tcod::print(main_win, {0, 29}, "Materials:", WHITE, BLACK);
    print_bold_with_condition(main_win, context, "[Common] "+std::to_string((unsigned long long int) (trader_prices.at("common_material")*0.8))+" Gold each", WHITE, 30, csr_pos==13);
    print_bold_with_condition(main_win, context, "[Uncommon] "+std::to_string((unsigned long long int) (trader_prices.at("uncommon_material")*0.8))+" Gold each", GREEN, 31, csr_pos==14);
    print_bold_with_condition(main_win, context, "[Rare] "+std::to_string((unsigned long long int) (trader_prices.at("rare_material")*0.8))+" Gold each", BLUE, 32, csr_pos==15);
    print_bold_with_condition(main_win, context, "[Epic] "+std::to_string((unsigned long long int) (trader_prices.at("epic_material")*0.8))+" Gold each", PURPLE, 33, csr_pos==16);
    print_bold_with_condition(main_win, context, "[Legendary] "+std::to_string((unsigned long long int) (trader_prices.at("legendary_material")*0.8))+" Gold each", YELLOW, 34, csr_pos==17);
    clear_and_draw_dialog(main_win, context, "Material Shop:");
}

bool trader_check_if_payment_valid(const std::string &type, unsigned long long int &payment, unsigned long long int amount){
    if(payment>=(trader_prices.at(type)*amount)){
        payment-=(trader_prices.at(type)*amount);
        return true;
    }
    else{
        return false;
    }
}

bool trader_process_payment(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &gear_merchant, unsigned int csr_pos){
    unsigned long long int amount=0;
    if(csr_pos<12){
        amount=get_ullint(main_win, context, "Please enter the amount of items you wish to purchase: ");
    }
    else if(csr_pos<18){
        amount=get_ullint(main_win, context, "Please enter the amount of items you wish to sell: ");
    }
    if(amount<=0){
        return false;
    }
    switch(csr_pos){
        case 0:
            if(trader_check_if_payment_valid("common_material", user.gold, amount)){
                user.inv.misc.materials.common+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 1:
            if(trader_check_if_payment_valid("uncommon_material", user.gold, amount)){
                user.inv.misc.materials.uncommon+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 2:
            if(trader_check_if_payment_valid("rare_material", user.gold, amount)){
                user.inv.misc.materials.rare+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 3:
            if(trader_check_if_payment_valid("epic_material", user.gold, amount)){
                user.inv.misc.materials.epic+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 4:
            if(trader_check_if_payment_valid("legendary_material", user.gold, amount)){
                user.inv.misc.materials.legendary+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 5:
            if(trader_check_if_payment_valid("helmet_blueprint", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.blueprint.helmet+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 6:
            if(trader_check_if_payment_valid("chestplate_blueprint", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.blueprint.chestplate+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 7:
            if(trader_check_if_payment_valid("greaves_blueprint", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.blueprint.greaves+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 8:
            if(trader_check_if_payment_valid("boots_blueprint", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.blueprint.boots+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 9:
            if(trader_check_if_payment_valid("shield_blueprint", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.blueprint.shield+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 10:
            if(trader_check_if_payment_valid("weapon_blueprint", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.blueprint.weapon+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 11:
            if(trader_check_if_payment_valid("first_aid_kit", user.inv.misc.cores.crystal_core, amount)){
                user.inv.misc.heal_amount+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 12:
            if(user.inv.misc.cores.crystal_core>=amount){
                user.inv.misc.cores.crystal_core-=amount;
                user.gold+=(trader_prices.at("crystal_core")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 13:
            if(user.inv.misc.materials.common>=amount){
                user.inv.misc.materials.common-=amount;
                user.gold+=(trader_prices.at("common_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 14:
            if(user.inv.misc.materials.uncommon>=amount){
                user.inv.misc.materials.uncommon-=amount;
                user.gold+=(trader_prices.at("uncommon_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 15:
            if(user.inv.misc.materials.rare>=amount){
                user.inv.misc.materials.rare-=amount;
                user.gold+=(trader_prices.at("rare_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 16:
            if(user.inv.misc.materials.epic>=amount){
                user.inv.misc.materials.epic-=amount;
                user.gold+=(trader_prices.at("epic_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 17:
            if(user.inv.misc.materials.legendary>=amount){
                user.inv.misc.materials.legendary-=amount;
                user.gold+=(trader_prices.at("legendary_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        default:
            break;
    }
    return false;
}

void trader_misc_menu(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &gear_merchant){
    unsigned int csr_pos=0;
    int ch;
    print_trader_misc_menu(main_win, context, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<17){
            csr_pos++;
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='m'){
            show_misc_items(main_win, context, user.inv.misc);
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        else if(ch==SDLK_RETURN){
            if(trader_process_payment(main_win, context, user, gear_merchant, csr_pos)){
                print_trader_misc_menu(main_win, context, csr_pos);
                draw_stats(main_win, context, user);
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Failure");
            }
        }
    }
}

double calculate_quality_points(const Item &cur){
    double quality_points=std::pow(rarity_value(cur.rarity), 4);
    quality_points+=cur.def/10.0;
    quality_points+=cur.crit_chance/5.0;
    quality_points+=cur.crit_dmg/5.0;
    switch(cur.type){
        case Type::HELMET:
            quality_points*=cur.hp/(40*rarity_value(cur.rarity));
            break;
        case Type::CHESTPLATE:
            quality_points*=cur.hp/(100*rarity_value(cur.rarity));
            break;
        case Type::GREAVES:
            quality_points*=cur.def/(50*rarity_value(cur.rarity));
            break;
        case Type::BOOTS:
            quality_points*=cur.def/(30*rarity_value(cur.rarity));
            break;
        case Type::SHIELD:
            quality_points*=cur.shield/(100*rarity_value(cur.rarity));
            break;
        case Type::WEAPON:
            quality_points*=cur.attk/(200*rarity_value(cur.rarity));
            break;
        default:
            break;
    }
    return quality_points;
}

void print_inventory_with_quality_points_description(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line){
    print_item_description_1(main_win, context, cur_item, line);
    double quality_points=calculate_quality_points(*cur_item);
    tcod::print(main_win, {0, line+1}, "Equipped: ", WHITE, BLACK);
    if(cur_item->is_equipped){
        tcod::print(main_win, {10, line+1}, "true", GREEN, BLACK);
    }
    if(!cur_item->is_equipped){
        tcod::print(main_win, {10, line+1}, "false", LIGHT_RED, BLACK);
    }
    line++;
    tcod::print(main_win, {0, line+1}, "Quality Points: "+std::to_string(quality_points), WHITE, BLACK);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(main_win);
}

void print_item_with_selection(tcod::Console &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, bool is_highlighted, bool is_selected){
    std::stringstream ss;
    if(is_selected){
        ss << "->";
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
    print_bold_with_condition(main_win, context, ss.str(), YELLOW, line+1, is_highlighted);
}

void draw_base_with_quality_points(tcod::Console &main_win, tcod::ContextPtr &context, int y, unsigned int size, unsigned int page, const int &target_quality, const int &total_quality){
    std::stringstream ss;
    SDL_wclear_dialog_bar(main_win, context);
    ss << "Inventory " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")" << " Selected Total Quality: [" << total_quality << "/" << target_quality << "]";
    tcod::print(main_win, {0, 0}, ss.str(), WHITE, BLACK);
    for(int i=0; i<80; i++){
        TCOD_console_put_char_ex(main_win.get(), i, y+1, '-', WHITE, BLACK);
    }
//    context->present(main_win);
}

void print_inventory_with_quality_points(tcod::Console &main_win, tcod::ContextPtr &context, unsigned int csr_pos, unsigned int page_num, const Player &user, const std::vector<std::pair<Item *, bool>> &items_copy, const int &target_quality, const int &total_quality){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, user);
    draw_base_with_quality_points(main_win, context, 34, items_copy.size(), page_num, target_quality, total_quality);
    for(long long int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item_with_selection(main_win, context, items_copy[i].first, iterator, csr_pos==iterator, items_copy[page_num*30+iterator].second);
    }
    print_inventory_with_quality_points_description(main_win, context, items_copy[page_num*30+csr_pos].first, 35);
}

int select_inventory_for_exchange(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, const Item *target){
    std::vector<unsigned long long int> selected_for_trade;
    std::vector<std::pair<Item *, bool>> selected_items;
    unsigned int page_num=0;
    int csr_pos=0;
    int total_quality=0;
    for(int i=0; i<user.inv.item.size(); i++){
        if(user.inv.item[i].rarity==Rarity::LEGENDARY){
            selected_items.push_back({&user.inv.item[i], false});
        }
    }
    int ch;
    print_inventory_with_quality_points(main_win, context, csr_pos, page_num, user, selected_items, calculate_quality_points(*target), total_quality);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<selected_items.size()-1&&csr_pos<29)){
            csr_pos++;
            print_inventory_with_quality_points(main_win, context, csr_pos, page_num, user, selected_items, calculate_quality_points(*target), total_quality);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            print_inventory_with_quality_points(main_win, context, csr_pos, page_num, user, selected_items, calculate_quality_points(*target), total_quality);
        }
        else if((ch=='a'||ch==SDLK_LEFT)&&page_num>0){
            page_num--;
            csr_pos=0;
            print_inventory_with_quality_points(main_win, context, csr_pos, page_num, user, selected_items, calculate_quality_points(*target), total_quality);
        }
        else if((ch=='d'||ch==SDLK_RIGHT)&&((page_num+1)*30<selected_items.size())){
            page_num++;
            csr_pos=0;
            print_inventory_with_quality_points(main_win, context, csr_pos, page_num, user, selected_items, calculate_quality_points(*target), total_quality);
        }
        else if(ch=='q'){
            return 0;
        }
        else if(ch==SDLK_RETURN){
            if(selected_items[csr_pos+(page_num*30)].second){
                selected_items[csr_pos+(page_num*30)].second=false;
                total_quality-=(int) (calculate_quality_points(*selected_items[csr_pos+(page_num*30)].first));
                selected_for_trade.erase(std::remove(selected_for_trade.begin(), selected_for_trade.end(), selected_items[csr_pos+(page_num*30)].first->id), selected_for_trade.end());
            }
            else{
                selected_items[csr_pos+(page_num*30)].second=true;
                total_quality+=(int) (calculate_quality_points(*selected_items[csr_pos+(page_num*30)].first));
                selected_for_trade.push_back(selected_items[csr_pos+(page_num*30)].first->id);
            }
            print_inventory_with_quality_points(main_win, context, csr_pos, page_num, user, selected_items, calculate_quality_points(*target), total_quality);
        }
        else if(ch=='B'&&!selected_for_trade.empty()){
            if(total_quality>=calculate_quality_points(*target)){
                for(int i=0; i<selected_for_trade.size(); i++){
                    user.delete_item_with_id(selected_for_trade[i]);
                }
                selected_items.clear();
                selected_for_trade.clear();
                return 2;
            }
            else{
                return 1;
            }
        }
    }
}

void mysterious_trader_items(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &mysterious_merchant){
    unsigned int page_num=0;
    int csr_pos=0;
    std::vector<Item> original_copy;
    for(const auto &i: mysterious_merchant.store){
        original_copy.push_back(i.first);
    }
    std::vector<Item *> items_copy;
    init_copy(original_copy, items_copy);
    int ch;
    draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            for(int i=0; i<user.inv.item.size(); i++){
                if(user.inv.item[i].rarity==Rarity::LEGENDARY){
                    int status=select_inventory_for_exchange(main_win, context, user, &mysterious_merchant.store[csr_pos].first);
                    if(status==1){
                        draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
                        clear_and_draw_dialog(main_win, context, "[Trader] ...*Stares*");
                    }
                    else if(status==2){
                        user.add_item(mysterious_merchant.store[csr_pos].first);
                        mysterious_merchant.store.erase(mysterious_merchant.store.begin()+csr_pos);
                        if(mysterious_merchant.store.empty()){
                            return;
                        }
                        original_copy.clear();
                        items_copy.clear();
                        for(const auto &i: mysterious_merchant.store){
                            original_copy.push_back(i.first);
                        }
                        init_copy(original_copy, items_copy);
                        csr_pos=0;
                        draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
                        clear_and_draw_dialog(main_win, context, "[Trader] Let fate intertwine our paths once more...");
                    }
                    else{
                        draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
                    }
                    break;
                }
            }
        }
    }
}
