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

#include <fstream>
#include <random>
#include <sstream>
#include "headers/mode.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include <cereal/types/vector.hpp>

void refresh_gear_merchant_store(Merchant &gear_merchant, unsigned long long int steps){
    if((steps>19999&&steps%20000==0)||gear_merchant.initial_refresh){
        if(!gear_merchant.store.empty()){
            gear_merchant.store.erase(std::remove_if(gear_merchant.store.begin(), gear_merchant.store.end(), [](const std::pair<Item, bool> &cur){return cur.second==false;}), gear_merchant.store.end());
        }
        gear_merchant.initial_refresh=false;
        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<int> item_type(1, 100);
        const int store_size=gear_merchant.store.size();
        for(int i=0; i<20-store_size; i++){
            int item=item_type(generator);
            if(item<20){
                gear_merchant.store.push_back({generate_trade_items(Rarity::COMMON), false});
            }
            else if(item<40){
                gear_merchant.store.push_back({generate_trade_items(Rarity::UNCOMMON), false});
            }
            else if(item<70){
                gear_merchant.store.push_back({generate_trade_items(Rarity::RARE), false});
            }
            else if(item<90){
                gear_merchant.store.push_back({generate_trade_items(Rarity::EPIC), false});
            }
            else{
                gear_merchant.store.push_back({generate_trade_items(Rarity::LEGENDARY), false});
            }
        }
        std::sort(gear_merchant.store.begin(), gear_merchant.store.end(), sort_ascending);
    }
}

void refresh_mysterious_merchant_store(Merchant &mysterious_trader, long long int steps){
    if((steps>99999&&steps%100000==0)||mysterious_trader.initial_refresh){
        mysterious_trader.initial_refresh=false;
        if(!mysterious_trader.store.empty()){
            mysterious_trader.store.clear();
        }
        for(int i=0; i<5; i++){
            mysterious_trader.store.push_back({generate_trade_items(Rarity::ARTIFACT), false});
        }
    }
}

void draw_gear_merchant_store(tcod::Console &main_win, tcod::ContextPtr &context, Item &cur, int x, int y){
    switch(cur.rarity){
        case Rarity::COMMON:
            TCOD_console_put_char_ex(main_win.get(), x, y, '/', WHITE, BLACK);
            break;
        case Rarity::UNCOMMON:
            TCOD_console_put_char_ex(main_win.get(), x, y, '/', GREEN, BLACK);
            break;
        case Rarity::RARE:
            TCOD_console_put_char_ex(main_win.get(), x, y, '/', BLUE, BLACK);
            break;
        case Rarity::EPIC:
            TCOD_console_put_char_ex(main_win.get(), x, y, '/', PURPLE, BLACK);
            break;
        case Rarity::LEGENDARY:
            TCOD_console_put_char_ex(main_win.get(), x, y, '/', YELLOW, BLACK);
            break;
        default:
            break;
    }
}

void redraw_bar(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &gear_merchant, std::vector<std::string> pub_layout, Csr csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    int gear_output_count=0;
    for(int i=0; i<pub_layout.size(); i++){
        for(int j=0; j<pub_layout[i].size(); j++){
            if(pub_layout[i][j]!=' '){
                if(pub_layout[i][j]=='.'){
                    TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], GREEN, BLACK);
                }
                else if(pub_layout[i][j]=='/'&&gear_output_count<gear_merchant.store.size()){
                    draw_gear_merchant_store(main_win, context, gear_merchant.store[gear_output_count].first, j, i+1);
                    gear_output_count++;
                }
                else if(pub_layout[i][j]=='/'&&gear_output_count>=gear_merchant.store.size()){
                    continue;
                }
                else if(pub_layout[i][j]=='~'){
                    TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], LIGHT_BLUE, BLACK);
                }
                else if(pub_layout[i][j]=='{'||pub_layout[i][j]=='}'){
                    TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], DARK_RED, BLACK);
                }
                else if(pub_layout[i][j]=='x'){
                    TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], LIGHT_RED, BLACK);
                }
                else if(pub_layout[i][j]=='S'||pub_layout[i][j]=='M'||pub_layout[i][j]=='B'||pub_layout[i][j]=='F'||pub_layout[i][j]=='G'||pub_layout[i][j]=='T'||pub_layout[i][j]=='N'||pub_layout[i][j]=='I'||pub_layout[i][j]=='A'){
                    TCOD_console_put_char_ex(main_win.get(), j, i+1, '@', MAGENTA, BLACK);
                }
                else{
                    TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], WHITE, BLACK);
                }
            }
        }
    }
    TCOD_console_put_char_ex(main_win.get(), 78, 2, '>', WHITE, BLACK);
    draw_player(main_win, context, csr_pos.x, csr_pos.y);
    draw_stats(main_win, context, user);
    tcod::print(main_win, {0, 0}, "Town", WHITE, BLACK);
    context->present(main_win);
}

char search_surroundings(std::vector<std::string> pub_layout, int x, int y){
    if(pub_layout[y][x-1]!='#'&&pub_layout[y][x-1]!=' '){
        return pub_layout[y][x-1];
    }
    else if(pub_layout[y][x+1]!='#'&&pub_layout[y][x+1]!=' '){
        return pub_layout[y][x+1];
    }
    else if(pub_layout[y-1][x]!='#'&&pub_layout[y-1][x]!=' '){
        return pub_layout[y-1][x];
    }
    else if(pub_layout[y+1][x]!='#'&&pub_layout[y+1][x]!=' '){
        return pub_layout[y+1][x];
    }
    return '0';
}

void draw_bartender_mode(tcod::Console &main_win, tcod::ContextPtr &context, const Bartender &bartender, Player &user, bool refresh_everything=true){
    SDL_wclear_main_win(main_win, context);
    if(refresh_everything){
        draw_stats(main_win, context, user);
        clear_and_draw_dialog(main_win, context, "[System] Welcome to the Bar!");
    }
    tcod::print(main_win, {0, 3}, "[Bartender] How can I serve you today?", WHITE, BLACK);
    std::stringstream output;
    tcod::print(main_win, {0, 5}, "1. A bottle of water (Bottle included)", WHITE, BLACK);
    output << "Price: " << 5+50*((100.0-bartender.relation)/100.0);
    tcod::print(main_win, {0, 6}, output.str(), WHITE, BLACK);
    tcod::print(main_win, {0, 8}, "2. A can of sparking juice", WHITE, BLACK);
    output.str(std::string());
    output << "Price: " << 20+100*((100.0-bartender.relation)/100.0);
    tcod::print(main_win, {0, 9}, output.str(), WHITE, BLACK);
    output.str(std::string());
    tcod::print(main_win, {0, 13}, "Backpack:", WHITE, BLACK);
    output << "Water:" << user.inv.water.water << "/8" << " Sparking Juice:" << user.inv.water.sparkling_juice << "/10";
    tcod::print(main_win, {0, 14}, output.str(), WHITE, BLACK);
    context->present(main_win);
}

void bartender_interface(tcod::Console &main_win, tcod::ContextPtr &context, Bartender &bartender, Player &user){
    draw_bartender_mode(main_win, context, bartender, user);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='1'){
            if(user.gold<5+50*((100.0-bartender.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Bartender] Don't waste my time if you don't have enough gold.");
                if(bartender.relation>=5){
                    bartender.relation-=5;
                }
                draw_bartender_mode(main_win, context, bartender, user, false);
                continue;
            }
            else if(user.inv.water.water>=8){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of water you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Bartender] Confirm you want to purchase a bottle of water? [y/n]")){
                    user.gold-=5+50*((100.0-bartender.relation)/100.0);
                    user.inv.water.water++;
                    if(bartender.relation<100){
                        bartender.relation+=((100.0-bartender.relation)/100.0);
                        if(bartender.relation>100){
                            bartender.relation=100;
                        }
                    }
                }
                draw_bartender_mode(main_win, context, bartender, user);
            }
        }
        else if(ch=='2'){
            if(user.gold<20+100*((100.0-bartender.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Bartender] Don't waste my time if you don't have enough gold.");
                if(bartender.relation>=5){
                    bartender.relation-=5;
                }
                draw_bartender_mode(main_win, context, bartender, user, false);
                continue;
            }
            else if(user.inv.water.sparkling_juice>=10){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of sparking water you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Bartender] Confirm you want to purchase a can of sparkling water? [y/n]")){
                    user.gold-=20+100*((100.0-bartender.relation)/100.0);
                    user.inv.water.sparkling_juice++;
                    if(bartender.relation<100){
                        bartender.relation+=((100.0-bartender.relation)/50.0);
                        if(bartender.relation>100){
                            bartender.relation=100;
                        }
                    }
                }
                draw_bartender_mode(main_win, context, bartender, user);
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void draw_farmer_mode(tcod::Console &main_win, tcod::ContextPtr &context, const Farmer &farmer, Player &user, bool refresh_everything=true){
    SDL_wclear_main_win(main_win, context);
    if(refresh_everything){
        draw_stats(main_win, context, user);
        clear_and_draw_dialog(main_win, context, "[Farmer] Welcome to my farm!");
    }
    tcod::print(main_win, {0, 3}, "[Farmer] What brings you here today?", WHITE, BLACK);
    std::stringstream output;
    tcod::print(main_win, {0, 5}, "1. A piece of bread", WHITE, BLACK);
    output << "Price: " << 5+50*((100.0-farmer.relation)/100.0);
    tcod::print(main_win, {0, 6}, output.str(), WHITE, BLACK);
    tcod::print(main_win, {0, 8}, "2. A packet of waffles", WHITE, BLACK);
    output.str(std::string());
    output << "Price: " << 20+100*((100.0-farmer.relation)/100.0);
    tcod::print(main_win, {0, 9}, output.str(), WHITE, BLACK);
    output.str(std::string());
    tcod::print(main_win, {0, 11}, "3. An energy bar", WHITE, BLACK);
    output << "Price: " << 50+150*((100.0-farmer.relation)/100.0);
    tcod::print(main_win, {0, 12}, output.str(), WHITE, BLACK);
    output.str(std::string());
    tcod::print(main_win, {0, 16}, "Backpack:", WHITE, BLACK);
    output << "Bread:" << user.inv.food.bread << "/8" << " Waffles:" << user.inv.food.waffle << "/10" << " Energy Bar:" << user.inv.food.energy_bar << "/10";
    tcod::print(main_win, {0, 17}, output.str(), WHITE, BLACK);
    context->present(main_win);
}

void farmer_interface(tcod::Console &main_win, tcod::ContextPtr &context, Farmer &farmer, Player &user){
    draw_farmer_mode(main_win, context, farmer, user);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='1'){
            if(user.gold<5+50*((100.0-farmer.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                draw_farmer_mode(main_win, context, farmer, user, false);
                continue;
            }
            else if(user.inv.food.bread>=8){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of bread you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Farmer] Confirm you want to purchase a piece of bread? [y/n]")){
                    user.gold-=5+50*((100.0-farmer.relation)/100.0);
                    user.inv.food.bread++;
                    if(farmer.relation<100){
                        farmer.relation+=((100.0-farmer.relation)/100.0);
                        if(farmer.relation>100){
                            farmer.relation=100;
                        }
                    }
                }
                draw_farmer_mode(main_win, context, farmer, user);
            }
        }
        else if(ch=='2'){
            if(user.gold<20+100*((100.0-farmer.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                draw_farmer_mode(main_win, context, farmer, user, false);
                continue;
            }
            else if(user.inv.food.waffle>=10){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of waffles you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Farmer] Confirm you want to purchase a packet of waffles? [y/n]")){
                    user.gold-=20+100*((100.0-farmer.relation)/100.0);
                    user.inv.food.waffle++;
                    if(farmer.relation<100){
                        farmer.relation+=((100.0-farmer.relation)/100.0);
                        if(farmer.relation>100){
                            farmer.relation=100;
                        }
                    }
                }
                draw_farmer_mode(main_win, context, farmer, user);
            }
        }
        else if(ch=='3'){
            if(user.gold<50+150*((100.0-farmer.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                draw_farmer_mode(main_win, context, farmer, user, false);
                continue;
            }
            else if(user.inv.food.energy_bar>=10){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of energy bars you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Farmer] Confirm you want to purchase a energy bar? [y/n]")){
                    user.gold-=50+150*((100.0-farmer.relation)/100.0);
                    user.inv.food.energy_bar++;
                    if(farmer.relation<100){
                        farmer.relation+=((100.0-farmer.relation)/100.0);
                        if(farmer.relation>100){
                            farmer.relation=100;
                        }
                    }
                }
                draw_farmer_mode(main_win, context, farmer, user);
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void char_move(int ch, tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Csr &csr_pos, const std::vector<std::string> &pub_layout){
    Csr original_pos=csr_pos;
    if((ch=='a'||ch==SDLK_LEFT)&&csr_pos.x>1&&pub_layout[csr_pos.y][csr_pos.x-1]==' '){
        csr_pos.x--;
    }
    if((ch=='d'||ch==SDLK_RIGHT)&&csr_pos.x<78&&pub_layout[csr_pos.y][csr_pos.x+1]==' '){
        csr_pos.x++;
    }
    if((ch=='w'||ch==SDLK_UP)&&csr_pos.y>1&&pub_layout[csr_pos.y-1][csr_pos.x]==' '){
        csr_pos.y--;
    }
    if((ch=='s'||ch==SDLK_DOWN)&&csr_pos.y<48&&pub_layout[csr_pos.y+1][csr_pos.x]==' '){
        csr_pos.y++;
    }
    TCOD_console_put_char_ex(main_win.get(), original_pos.x, original_pos.y+1, ' ', WHITE, BLACK);
    if(original_pos.x==78&&original_pos.y==1){
        TCOD_console_put_char_ex(main_win.get(), 78, 2, '>', WHITE, BLACK);
    }
    draw_player(main_win, context, csr_pos.x, csr_pos.y);
    context->present(main_win);
}

void draw_menu_selections(tcod::Console &main_win, tcod::ContextPtr &context, int line, bool is_bold, std::string output){
    if(is_bold){
        tcod::print(main_win, {0, line}, output, BLACK, WHITE);
    }
    else{
        tcod::print(main_win, {0, line}, output, WHITE, BLACK);
    }
}

void draw_bank_menu(tcod::Console &main_win, tcod::ContextPtr &context, Chest &chest, Bank &bank, unsigned int csr_pos, unsigned long long int steps){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Bank Menu", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "[Kiosk] How can we help you?", WHITE, BLACK);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Store Gears]");
    draw_menu_selections(main_win, context, 4, csr_pos==1, "[Retrieve Gears]");
    draw_menu_selections(main_win, context, 6, csr_pos==2, "[Store Materials and Blueprints]");
    draw_menu_selections(main_win, context, 7, csr_pos==3, "[Retrieve Materials and Blueprints]");
    draw_menu_selections(main_win, context, 9, csr_pos==4, "[Store Gold]");
    draw_menu_selections(main_win, context, 10, csr_pos==5, "[Retrieve Gold]");
    // Draw bank stats
    if(!chest.gear_storage.empty()){
        tcod::print(main_win, {0, 47}, "Total Gear stored: ["+std::to_string(chest.gear_storage.size())+"]", WHITE, BLACK);
    }
    else{
        tcod::print(main_win, {0, 47}, "Total Gear stored: ["+std::to_string(chest.gear_storage.size())+"]", WHITE, BLACK);
    }
    tcod::print(main_win, {0, 48}, "Total Gold stored: ["+std::to_string(bank.saved_gold)+"]", WHITE, BLACK);
    if(bank.interest_next_applied>0){
        tcod::print(main_win, {0, 49}, "Next Interest Compound in: ["+std::to_string(bank.interest_next_applied-steps)+"] moves", WHITE, BLACK);
    }
    else{
        tcod::print(main_win, {0, 49}, "Next Interest Compound in: [Invalid] moves", WHITE, BLACK);
    }
    context->present(main_win);
}

void bank_interface(tcod::Console &main_win, tcod::ContextPtr &context, No_Delete &perm_config, Player &user, Chest &chest, Bank &bank){
    unsigned int csr_pos=0;
    int ch;
    draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<5){
            csr_pos++;
            draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            csr_pos--;
            draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            switch(csr_pos){
                case 0:
                    if(!user.inv.item.empty()){
                        inventory_storage(main_win, context, user, perm_config, chest);
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Inventory Empty");
                    }
                    break;
                case 1:
                    if(!chest.gear_storage.empty()){
                        inventory_retrieve(main_win, context, user, perm_config, chest);
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Storage Empty");
                    }
                    break;
                case 2:
                    store_misc_items(main_win, context, user.inv.misc, chest);
                    draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                    break;
                case 3:
                    retrieve_misc_items(main_win, context, user.inv.misc, chest);
                    draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                    break;
                case 4:{
                    unsigned long long int input=get_ullint(main_win, context, "Please enter a valid amount of gold to store: ");
                    if(input<=user.gold&&input>0){
                        if(bank.initial_gold_store_time==0){
                            bank.initial_gold_store_time=user.steps;
                        }
                        user.gold-=input;
                        bank.saved_gold+=input;
                        if(bank.interest_next_applied==0){
                            bank.interest_next_applied=50000+(bank.initial_gold_store_time%50000);
                        }
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                        draw_stats(main_win, context, user);
                        clear_and_draw_dialog(main_win, context, "Transfer Successful!");
                    }
                    else{
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                    }
                    break;
                }
                case 5:{
                    unsigned long long int input=get_ullint(main_win, context, "Please enter a valid amount of gold to retrieve: ");
                    if(input<=bank.saved_gold&&input>0){
                        bank.saved_gold-=input;
                        user.gold+=input;
                        if(bank.saved_gold==0){
                            bank.initial_gold_store_time=0;
                            bank.interest_next_applied=0;
                        }
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                        draw_stats(main_win, context, user);
                        clear_and_draw_dialog(main_win, context, "Transfer Successful!");
                    }
                    else{
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, user.steps);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void draw_gear_merchant(tcod::Console &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Gear Shop", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "[Merchant] How can I help you?", WHITE, BLACK);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Buy Gears]");
    draw_menu_selections(main_win, context, 4, csr_pos==1, "[Sell Gears]");
    draw_menu_selections(main_win, context, 6, csr_pos==2, "[Buy Materials/Blueprints/First Aid Kits]");
    context->present(main_win);
}

void gear_merchant_interface(tcod::Console &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &user, No_Delete &perm_config){
    unsigned int csr_pos=0;
    int ch;
    draw_gear_merchant(main_win, context, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<2){
            csr_pos++;
            draw_gear_merchant(main_win, context, csr_pos);
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            csr_pos--;
            draw_gear_merchant(main_win, context, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            switch(csr_pos){
                case 0:
                    if(!gear_merchant.store.empty()){
                        show_merchant_items(main_win, context, gear_merchant, user);
                        draw_stats(main_win, context, user);
                        draw_gear_merchant(main_win, context, csr_pos);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[Merchant] Sorry, out of stock! Please come back later.");
                    }
                    break;
                case 1:
                    if(!user.inv.item.empty()){
                        sell_items_to_merchant(main_win, context, gear_merchant, user, perm_config);
                        draw_stats(main_win, context, user);
                        draw_gear_merchant(main_win, context, csr_pos);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Inventory Empty");
                    }
                    break;
                case 2:
                    trader_misc_menu(main_win, context, user, gear_merchant);
                    draw_stats(main_win, context, user);
                    draw_gear_merchant(main_win, context, csr_pos);
                    break;
                default:
                    break;
            }
        }
    }
}

void draw_mysterious_trader_menu(tcod::Console &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Mysterious Hut", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "[Mysterious Trader] ...", WHITE, BLACK);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Exchange items for Artifact Materials]");
    tcod::print(main_win, {0, 4}, "- 5 Legendary Material", WHITE, BLACK);
    tcod::print(main_win, {0, 5}, "- 1 Ancient Core", WHITE, BLACK);
    draw_menu_selections(main_win, context, 7, csr_pos==1, "[Exchange items for Ancient Cores]");
    tcod::print(main_win, {0, 8}, "- 1 Mysterious Shard", WHITE, BLACK);
    tcod::print(main_win, {0, 9}, "- 1 Crystallium", WHITE, BLACK);
    draw_menu_selections(main_win, context, 11, csr_pos==2, "[Exchange gears for Artifact Gears]");
    context->present(main_win);
}

void mysterious_trader_interface(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Merchant &mysterious_trader){
    unsigned int csr_pos=0;
    int ch;
    draw_mysterious_trader_menu(main_win, context, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<2){
            csr_pos++;
            draw_mysterious_trader_menu(main_win, context, csr_pos);
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            csr_pos--;
            draw_mysterious_trader_menu(main_win, context, csr_pos);
        }
        else if(ch=='m'){
            show_misc_items(main_win, context, user.inv.misc);
            draw_mysterious_trader_menu(main_win, context, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            switch(csr_pos){
                case 0:{
                    unsigned long long int amount=get_ullint(main_win, context, "Amount of Artifact Materials: ");
                    if(amount>0&&user.inv.misc.materials.legendary>=5*amount&&user.inv.misc.cores.ancient_core>=amount){
                        user.inv.misc.materials.legendary-=(5*amount);
                        user.inv.misc.cores.ancient_core-=amount;
                        user.inv.misc.materials.artifact+=amount;
                        clear_and_draw_dialog(main_win, context, "[System] Success");
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Failure");
                    }
                    break;
                }
                case 1:{
                    unsigned long long int amount=get_ullint(main_win, context, "Amount of Ancient Cores: ");
                    if(amount>0&&user.inv.misc.cores.crystallium>=amount&&user.inv.misc.cores.mysterious_shard>=amount){
                        user.inv.misc.cores.crystallium-=amount;
                        user.inv.misc.cores.mysterious_shard-=amount;
                        user.inv.misc.cores.ancient_core+=amount;
                        clear_and_draw_dialog(main_win, context, "[System] Success");
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Failure");
                    }
                    break;
                }
                case 2:
                    if(!mysterious_trader.store.empty()){
                        mysterious_trader_items(main_win, context, user, mysterious_trader);
                        draw_mysterious_trader_menu(main_win, context, csr_pos);
                        draw_stats(main_win, context, user);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[...] Return only when the black moon howls.");
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void miner_hire_menu(tcod::Console &main_win, tcod::ContextPtr &context, const Miner &miner, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Miner's Inn", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "[Innkeeper] Ay matey, what brings you here today?", WHITE, BLACK);
    int line=3;
    draw_menu_selections(main_win, context, line, csr_pos==0, "[Hire Miners]");
    line+=2;
    draw_menu_selections(main_win, context, line, csr_pos==1, "[Accumulated Loot]");
    context->present(main_win);
}

void draw_miner_hire_selections(tcod::Console &main_win, tcod::ContextPtr &context, Miner &miner, unsigned int csr_pos, int ideal_payment, int actual_payment){
    SDL_wclear_main_win(main_win, context);
    tcod::print(main_win, {0, 1}, "Hire Miners", WHITE, BLACK);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "Amount of Miners: "+std::to_string(miner.job.number_of_miners));
    std::stringstream payment_string;
    payment_string << "Payment [" << ideal_payment << "]: " << actual_payment;
    draw_menu_selections(main_win, context, 5, csr_pos==1, payment_string.str());
    std::stringstream time_string;
    time_string << "Total duration: " << Time{miner.job.original_job_duration.time_to_seconds()*static_cast<long long int>((1000-miner.skill_level)/1000)};
    tcod::print(main_win, {0, 7}, time_string.str(), WHITE, BLACK);
    draw_menu_selections(main_win, context, 9, csr_pos==2, "[Done]");
    draw_menu_selections(main_win, context, 10, csr_pos==3, "[Cancel]");
    clear_and_draw_dialog(main_win, context, "Miner's Inn");
}

void miner_hire_selection(tcod::Console &main_win, tcod::ContextPtr &context, Miner &miner, Player &user){
    unsigned int csr_pos=0;
    int actual_payment=0, ideal_payment=0;
    int ch;
    draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<3){
            csr_pos++;
            draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
        }
        else if(ch==SDLK_RETURN){
            switch(csr_pos){
                case 0:{
                    miner.job.number_of_miners=get_int(main_win, context, "[System] Please enter the amount of miners you wish to hire: ");
                    ideal_payment=(pow(miner.job.number_of_miners, 2))*1000*((10000-miner.relation)/10000.0);
                    draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
                    break;
                }
                case 1:{
                    actual_payment=get_int(main_win, context, "[System] Please enter the amount you wish to pay: ");
                    draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
                    break;
                }
                case 2:{
                    if(miner.job.number_of_miners>10){
                        clear_and_draw_dialog(main_win, context, "[System] You cannot hire more than 10 miners");
                        continue;
                    }
                    if(miner.job.number_of_miners<=0){
                        clear_and_draw_dialog(main_win, context, "[System] Value of hired miners must be more than 0");
                        continue;
                    }
                    if(actual_payment>user.gold){
                        clear_and_draw_dialog(main_win, context, "[System] Insufficient gold");
                        continue;
                    }
                    if(actual_payment<(ideal_payment)*0.9){
                        clear_and_draw_dialog(main_win, context, "[Innkeeper] If you're here to tale advantage of us, SCRAM!");
                        continue;
                    }
                    miner.job.has_active_job=true;
                    miner.job.total_job_duration=Time{miner.job.original_job_duration.time_to_seconds()*static_cast<long long int>((1000-miner.skill_level)/1000)};
                    miner.job.job_start=std::chrono::steady_clock::now();
                    miner.job.loot_multiplier=double(actual_payment)/double(ideal_payment);
                    if(miner.job.loot_multiplier>2.0){
                        miner.job.loot_multiplier=2.0;
                    }
                    if(miner.relation<100){
                        miner.relation+=(miner.job.loot_multiplier-1);
                    }
                    user.gold-=actual_payment;
                    draw_stats(main_win, context, user);
                    return;
                }
                case 3:
                    miner.job=Miner::Job_Details();
                    return;
                default:
                    break;
            }

        }
        else if(ch=='q'){
            miner.job=Miner::Job_Details();
            return;
        }
    }
}

void print_mysterious_loot(tcod::Console &main_win, tcod::ContextPtr &context, Miner &miner, const unsigned &csr_pos){
    SDL_wclear_main_win(main_win, context);
    tcod::print(main_win, {0, 1}, "[Keeper] How are you doing today?", WHITE, BLACK);
    std::stringstream ss;
    ss << "Mysterious Piece: " << miner.loot.mysterious_piece;
    tcod::print(main_win, {0, 3}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "Mysterious Artifact: " << miner.loot.mysterious_artifact;
    tcod::print(main_win, {0, 5}, ss.str(), WHITE, BLACK);
    draw_menu_selections(main_win, context, 7, csr_pos==0, "[Cancel]");
    draw_menu_selections(main_win, context, 8, csr_pos==1, "[Transfer to Archaeologist]");
    clear_and_draw_dialog(main_win, context, "Miner's Storage");
}

void show_miner_loot(tcod::Console &main_win, tcod::ContextPtr &context, Miner &miner, Archaeologist &archaeologist){
    unsigned csr_pos=0;
    int ch;
    print_mysterious_loot(main_win, context, miner, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<1){
            csr_pos++;
            print_mysterious_loot(main_win, context, miner, csr_pos);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            print_mysterious_loot(main_win, context, miner, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==0){
                return;
            }
            else if(csr_pos==1&&SDL_getch_y_or_n(main_win, context, "[Keeper] Transfer raw loot to Archaeologist? [y/n]")){
                archaeologist.raw_storage.mysterious_artifact+=miner.loot.mysterious_artifact;
                archaeologist.raw_storage.mysterious_piece+=miner.loot.mysterious_piece;
                miner.loot=Raw_Loot();
            }
            print_mysterious_loot(main_win, context, miner, csr_pos);
        }
    }
}

void miner_hire_interface(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Miner &miner, Archaeologist &archaeologist){
    unsigned int csr_pos=0;
    miner_hire_menu(main_win, context, miner, csr_pos);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            miner_hire_menu(main_win, context, miner, csr_pos);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<1){
            csr_pos++;
            miner_hire_menu(main_win, context, miner, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==0){
                if(!miner.job.has_active_job){
                    miner_hire_selection(main_win, context, miner, user);
                    miner_hire_menu(main_win, context, miner, csr_pos);
                }
                else{
                    std::stringstream ss;
                    Time time_left{miner.job.total_job_duration.time_to_seconds()-std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-miner.job.job_start).count()};
                    ss << "[System] Time left: " << time_left;
                    clear_and_draw_dialog(main_win, context, ss.str());
                }
            }
            else if(csr_pos==1){
                show_miner_loot(main_win, context, miner, archaeologist);
                miner_hire_menu(main_win, context, miner, csr_pos);
            }
        }
    }
}

void draw_archaeologist_decryption_menu(tcod::Console &main_win, tcod::ContextPtr &context, Archaeologist &archaeologist, unsigned int csr_pos, long long int ideal_payment, long long int actual_payment){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Archaeologist's Working Site", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "[System] Decryption Menu", WHITE, BLACK);
    std::stringstream ss;
    ss << "Mysterious Piece (Materials)[Max: " << archaeologist.raw_storage.mysterious_piece << "]: " << archaeologist.job.decryption_amount.mysterious_piece;
    draw_menu_selections(main_win, context, 3, csr_pos==0, ss.str());
    ss.str(std::string());
    ss << "Mysterious Artifact (Cores)[Max: " << archaeologist.raw_storage.mysterious_artifact << "]: " << archaeologist.job.decryption_amount.mysterious_artifact;
    draw_menu_selections(main_win, context, 5, csr_pos==1, ss.str());
    ss.str(std::string());
    ss << "Payment [" << ideal_payment << "]: " << actual_payment;
    draw_menu_selections(main_win, context, 7, csr_pos==2, ss.str());
    ss.str(std::string());
    ss << "Total duration: " << Time{((archaeologist.job.decryption_amount.mysterious_artifact*archaeologist.job.decrypt_artifact_duration.time_to_seconds()+archaeologist.job.decryption_amount.mysterious_piece*archaeologist.job.decrypt_piece_duration.time_to_seconds())*static_cast<long long int>(500-archaeologist.skill_level)/500)};
    tcod::print(main_win, {0,9}, ss.str(), WHITE, BLACK);
    draw_menu_selections(main_win, context, 11, csr_pos==3, "[Start decryption]");
    draw_menu_selections(main_win, context, 12, csr_pos==4, "[Cancel]");
    context->present(main_win);
}

void archaeologist_decryption_menu(tcod::Console &main_win, tcod::ContextPtr &context, Archaeologist &archaeologist, Player &user){
    unsigned csr_pos=0;
    long long int ideal_payment=0, actual_payment=0;
    int ch;
    draw_archaeologist_decryption_menu(main_win, context, archaeologist, csr_pos, ideal_payment, actual_payment);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_archaeologist_decryption_menu(main_win, context, archaeologist, csr_pos, ideal_payment, actual_payment);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<4){
            csr_pos++;
            draw_archaeologist_decryption_menu(main_win, context, archaeologist, csr_pos, ideal_payment, actual_payment);
        }
        else if(ch=='q'){
            archaeologist.job=Archaeologist::Job_Details();
            return;
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==0){
                archaeologist.job.decryption_amount.mysterious_piece=get_int(main_win, context, "[System] Amount of mysterious pieces you wish to decrypt: ");
                ideal_payment=((archaeologist.job.decryption_amount.mysterious_piece*100)+(archaeologist.job.decryption_amount.mysterious_artifact*1000))*((10000-archaeologist.relation)/10000.0);
            }
            else if(csr_pos==1){
                archaeologist.job.decryption_amount.mysterious_artifact=get_int(main_win, context, "[System] Amount of mysterious artifacts you wish to decrypt: ");
                ideal_payment=((archaeologist.job.decryption_amount.mysterious_piece*100)+(archaeologist.job.decryption_amount.mysterious_artifact*1000))*((10000-archaeologist.relation)/10000.0);
            }
            else if(csr_pos==2){
                actual_payment=get_llint(main_win, context, "[System] Please enter the amount you wish to pay: ");
            }
            else if(csr_pos==3){
                if(archaeologist.job.decryption_amount.mysterious_piece>archaeologist.raw_storage.mysterious_piece||archaeologist.job.decryption_amount.mysterious_artifact>archaeologist.raw_storage.mysterious_artifact){
                    clear_and_draw_dialog(main_win, context, "[System] Insufficient items in storage");
                    continue;
                }
                if(archaeologist.job.decryption_amount.mysterious_artifact==0&&archaeologist.job.decryption_amount.mysterious_piece==0){
                    clear_and_draw_dialog(main_win, context, "[System] No items to decrypt");
                    continue;
                }
                if(actual_payment>user.gold){
                    clear_and_draw_dialog(main_win, context, "[System] Insufficient gold");
                    continue;
                }
                archaeologist.job.total_job_duration=Time{((archaeologist.job.decryption_amount.mysterious_artifact*archaeologist.job.decrypt_artifact_duration.time_to_seconds()+archaeologist.job.decryption_amount.mysterious_piece*archaeologist.job.decrypt_piece_duration.time_to_seconds())*static_cast<long long>(500-archaeologist.skill_level)/500)};
                archaeologist.job.job_start=std::chrono::steady_clock::now();
                archaeologist.job.has_active_job=true;
                if(archaeologist.relation<100){
                    archaeologist.relation+=(1-(double(actual_payment)/double(ideal_payment)));
                }
                archaeologist.raw_storage.mysterious_piece-=archaeologist.job.decryption_amount.mysterious_piece;
                archaeologist.raw_storage.mysterious_artifact-=archaeologist.job.decryption_amount.mysterious_artifact;
                user.gold-=actual_payment;
                return;
            }
            else if(csr_pos==4){
                archaeologist.job=Archaeologist::Job_Details();
                return;
            }
            draw_archaeologist_decryption_menu(main_win, context, archaeologist, csr_pos, ideal_payment, actual_payment);
        }
    }
}

void draw_archaeologist_menu(tcod::Console &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(main_win, {0, 0}, "Archaeologist's Working Site", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "[Archaeologist] State your request.", WHITE, BLACK);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Decrypt stored artifacts/pieces]");
    draw_menu_selections(main_win, context, 5, csr_pos==1, "[Storage]");
    context->present(main_win);
}

void draw_archaeologist_storage(tcod::Console &main_win, tcod::ContextPtr &context, Archaeologist &archaeologist, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    tcod::print(main_win, {0, 0}, "Storage Area", WHITE, BLACK);
    tcod::print(main_win, {0, 1}, "Processed loot: ", WHITE, BLACK);
    tcod::print(main_win, {0, 2}, "Cores:", WHITE, BLACK);
    tcod::print(main_win, {0, 3}, "Ancient Cores: "+std::to_string(archaeologist.loot_storage.cores.ancient_core), DARK_RED, BLACK);
    tcod::print(main_win, {0, 4}, "Mysterious Shard: "+std::to_string(archaeologist.loot_storage.cores.mysterious_shard), DARK_RED, BLACK);
    tcod::print(main_win, {0, 5}, "Crystallium: "+std::to_string(archaeologist.loot_storage.cores.crystallium), YELLOW, BLACK);
    tcod::print(main_win, {0, 6}, "Crystal Cores: "+std::to_string(archaeologist.loot_storage.cores.crystal_core), PURPLE, BLACK);
    tcod::print(main_win, {0, 8}, "Materials:", WHITE, BLACK);
    tcod::print(main_win, {0, 9}, "Common: "+std::to_string(archaeologist.loot_storage.materials.common), WHITE, BLACK);
    tcod::print(main_win, {0, 10}, "Uncommon: "+std::to_string(archaeologist.loot_storage.materials.uncommon), GREEN, BLACK);
    tcod::print(main_win, {0, 11}, "Rare: "+std::to_string(archaeologist.loot_storage.materials.rare), BLUE, BLACK);
    tcod::print(main_win, {0, 12}, "Epic: "+std::to_string(archaeologist.loot_storage.materials.epic), PURPLE, BLACK);
    tcod::print(main_win, {0, 13}, "Legendary: "+std::to_string(archaeologist.loot_storage.materials.legendary), YELLOW, BLACK);
    tcod::print(main_win, {0, 14}, "Artifact: "+std::to_string(archaeologist.loot_storage.materials.artifact), DARK_RED, BLACK);
    tcod::print(main_win, {0, 16}, "Raw loot: ", WHITE, BLACK);
    tcod::print(main_win, {0, 17}, "Mysterious pieces: "+std::to_string(archaeologist.raw_storage.mysterious_piece), WHITE, BLACK);
    tcod::print(main_win, {0, 18}, "Mysterious artifacts: "+std::to_string(archaeologist.raw_storage.mysterious_artifact), WHITE, BLACK);
    draw_menu_selections(main_win, context, 20, csr_pos==0, "[Retrieve all processed loot]");
    draw_menu_selections(main_win, context, 21, csr_pos==1, "[Return]");
    context->present(main_win);
}

void archaeologist_storage_interface(tcod::Console &main_win, tcod::ContextPtr &context, Archaeologist &archaeologist, Player &user){
    unsigned csr_pos=0;
    int ch;
    draw_archaeologist_storage(main_win, context, archaeologist, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_archaeologist_storage(main_win, context, archaeologist, csr_pos);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<1){
            csr_pos++;
            draw_archaeologist_storage(main_win, context, archaeologist, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==0){
                user.inv.misc.cores+=archaeologist.loot_storage.cores;
                user.inv.misc.materials+=archaeologist.loot_storage.materials;
                archaeologist.loot_storage=Miscellaneous();
                draw_archaeologist_storage(main_win, context, archaeologist, csr_pos);
            }
            else if(csr_pos==1){
                return;
            }
        }
    }
}

void archaeologist_interface(tcod::Console &main_win, tcod::ContextPtr &context, Archaeologist &archaeologist, Player &user){
    unsigned csr_pos=0;
    int ch;
    draw_archaeologist_menu(main_win, context, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_archaeologist_menu(main_win, context, csr_pos);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<1){
            csr_pos++;
            draw_archaeologist_menu(main_win, context, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==0){
                if(!archaeologist.job.has_active_job){
                    archaeologist_decryption_menu(main_win, context, archaeologist, user);
                    draw_archaeologist_menu(main_win, context, csr_pos);
                }
                else{
                    std::stringstream ss;
                    Time time_left{archaeologist.job.total_job_duration.time_to_seconds()-std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-archaeologist.job.job_start).count()};
                    ss << "[System] Time left: " << time_left;
                    clear_and_draw_dialog(main_win, context, ss.str());
                }
            }
            else if(csr_pos==1){
                archaeologist_storage_interface(main_win, context, archaeologist, user);
                draw_archaeologist_menu(main_win, context, csr_pos);
            }
        }
    }
}

void town_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Dungeon &dungeon_data, No_Delete &perm_config, Thread_Flags &thread_flags){
    refresh_gear_merchant_store(dungeon_data.npc.gear_merchant, user.steps);
    refresh_mysterious_merchant_store(dungeon_data.npc.mysterious_trader, user.steps);
    std::ifstream pub_layout_file("src/res/bar_layout.txt");
    std::vector<std::string> pub_layout; // {50,80}
    for(int i=0; i<50; i++){
        std::string line;
        std::getline(pub_layout_file, line);
        pub_layout.push_back(line);
    }
    redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
    int ch;
    while(!thread_flags.get_flag(thread_flags.terminate)){
        ch=SDL_getch(main_win, context);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==SDLK_LEFT||ch==SDLK_RIGHT||ch==SDLK_DOWN||ch==SDLK_UP){
            char_move(ch, main_win, context, user, dungeon_data.csr_pos, pub_layout);
        }
        else if(ch=='x'){
            char target=search_surroundings(pub_layout, dungeon_data.csr_pos.x, dungeon_data.csr_pos.y);
            if(target=='M'){
                mysterious_trader_interface(main_win, context, user, dungeon_data.npc.mysterious_trader);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='B'){
                bartender_interface(main_win, context, dungeon_data.npc.bartender, user);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='F'){
                farmer_interface(main_win, context, dungeon_data.npc.farmer, user);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='G'){
                gear_merchant_interface(main_win, context, dungeon_data.npc.gear_merchant, user, perm_config);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='T'){
                bank_interface(main_win, context, perm_config, user, dungeon_data.npc.chest, dungeon_data.npc.bank);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='S'){
                reforge_repair_mode(main_win, context, user, perm_config);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='I'){
                miner_hire_interface(main_win, context, user, dungeon_data.npc.miner, dungeon_data.npc.archaeologist);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
            else if(target=='A'){
                archaeologist_interface(main_win, context, dungeon_data.npc.archaeologist, user);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
        }
        else if(ch=='c'){
            if(dungeon_data.csr_pos.x==78&&dungeon_data.csr_pos.y==1){
                dungeon_data.csr_pos=dungeon_data.old_pos;
                dungeon_data.current={1,1,1};
                return;
            }
        }
        else if(ch=='i'){
            if(!user.inv.item.empty()){
                inventory_mode(main_win, context, user, perm_config);
                redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
            }
        }
        else if(ch=='e'){
            eat_drink_mode(main_win, context, user);
            redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
        }
        else if(ch=='q'){
            if(SDL_getch_y_or_n(main_win, context, "Do you really wish to quit? [y] to quit, any other key to abort.")){
                end_program(0);
                thread_flags.update_flag(thread_flags.terminate, true);
                return;
            }
            redraw_bar(main_win, context, user, dungeon_data.npc.gear_merchant, pub_layout, dungeon_data.csr_pos);
        }
        else if(ch=='S'){
            save_data(dungeon_data, user, perm_config);
            clear_and_draw_dialog(main_win, context, "Data saved successfully!");
        }
    }
}
