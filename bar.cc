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
            gear_merchant.store.erase(std::remove_if(gear_merchant.store.begin(), gear_merchant.store.end(), [](const std::pair<Item, bool> cur){return cur.second==false;}), gear_merchant.store.end());
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

void draw_gear_merchant_store(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Item &cur, int x, int y){
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
    }
}

void redraw_bar(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Merchant &gear_merchant, std::vector<std::string> pub_layout, Csr csr_pos){
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
    draw_player(main_win, context, csr_pos.first, csr_pos.second);
    draw_stats(main_win, context, User);
    tcod::print(*main_win, {0, 0}, "Town", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
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

void draw_bartender_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Bartender &bartender, Player &User, bool refresh_everything=true){
    SDL_wclear_main_win(main_win, context);
    if(refresh_everything){
        draw_stats(main_win, context, User);
        clear_and_draw_dialog(main_win, context, "[System] Welcome to the Bar!");
    }
    tcod::print(*main_win, {0, 3}, "[Bartender] How can I serve you today?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    std::stringstream output;
    tcod::print(*main_win, {0, 5}, "1. A bottle of water (Bottle included)", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output << "Price: " << 5+50*((100.0-bartender.relation)/100.0);
    tcod::print(*main_win, {0, 6}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 8}, "2. A can of sparking juice", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    output << "Price: " << 20+100*((100.0-bartender.relation)/100.0);
    tcod::print(*main_win, {0, 9}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    tcod::print(*main_win, {0, 13}, "Backpack:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output << "Water:" << User.inv.water.water << "/8" << " Sparking Juice:" << User.inv.water.sparkling_juice << "/10";
    tcod::print(*main_win, {0, 14}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}

void bartender_interface(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Bartender &bartender, Player &User){
    draw_bartender_mode(main_win, context, bartender, User);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='1'){
            if(User.gold<5+50*((100.0-bartender.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Bartender] Don't waste my time if you don't have enough gold.");
                if(bartender.relation>=5){
                    bartender.relation-=5;
                }
                draw_bartender_mode(main_win, context, bartender, User, false);
                continue;
            }
            else if(User.inv.water.water>=8){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of water you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Bartender] Confirm you want to purchase a bottle of water? [y/n]")){
                    User.gold-=5+50*((100.0-bartender.relation)/100.0);
                    User.inv.water.water++;
                    if(bartender.relation<100){
                        bartender.relation+=((100.0-bartender.relation)/100.0);
                        if(bartender.relation>100){
                            bartender.relation=100;
                        }
                    }
                }
                draw_bartender_mode(main_win, context, bartender, User);
            }
        }
        else if(ch=='2'){
            if(User.gold<20+100*((100.0-bartender.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Bartender] Don't waste my time if you don't have enough gold.");
                if(bartender.relation>=5){
                    bartender.relation-=5;
                }
                draw_bartender_mode(main_win, context, bartender, User, false);
                continue;
            }
            else if(User.inv.water.sparkling_juice>=10){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of sparking water you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Bartender] Confirm you want to purchase a can of sparkling water? [y/n]")){
                    User.gold-=20+100*((100.0-bartender.relation)/100.0);
                    User.inv.water.sparkling_juice++;
                    if(bartender.relation<100){
                        bartender.relation+=((100.0-bartender.relation)/50.0);
                        if(bartender.relation>100){
                            bartender.relation=100;
                        }
                    }
                }
                draw_bartender_mode(main_win, context, bartender, User);
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void draw_farmer_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Farmer &farmer, Player &User, bool refresh_everything=true){
    SDL_wclear_main_win(main_win, context);
    if(refresh_everything){
        draw_stats(main_win, context, User);
        clear_and_draw_dialog(main_win, context, "[Farmer] Welcome to my farm!");
    }
    tcod::print(*main_win, {0, 3}, "[Farmer] What brings you here today?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    std::stringstream output;
    tcod::print(*main_win, {0, 5}, "1. A piece of bread", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output << "Price: " << 5+50*((100.0-farmer.relation)/100.0);
    tcod::print(*main_win, {0, 6}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 8}, "2. A packet of waffles", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    output << "Price: " << 20+100*((100.0-farmer.relation)/100.0);
    tcod::print(*main_win, {0, 9}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    tcod::print(*main_win, {0, 11}, "3. An energy bar", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output << "Price: " << 50+150*((100.0-farmer.relation)/100.0);
    tcod::print(*main_win, {0, 12}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    tcod::print(*main_win, {0, 16}, "Backpack:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output << "Bread:" << User.inv.food.bread << "/8" << " Waffles:" << User.inv.food.waffle << "/10" << " Energy Bar:" << User.inv.food.energy_bar << "/10";
    tcod::print(*main_win, {0, 17}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}

void farmer_interface(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Farmer &farmer, Player &User){
    draw_farmer_mode(main_win, context, farmer, User);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='1'){
            if(User.gold<5+50*((100.0-farmer.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                draw_farmer_mode(main_win, context, farmer, User, false);
                continue;
            }
            else if(User.inv.food.bread>=8){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of bread you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Farmer] Confirm you want to purchase a piece of bread? [y/n]")){
                    User.gold-=5+50*((100.0-farmer.relation)/100.0);
                    User.inv.food.bread++;
                    if(farmer.relation<100){
                        farmer.relation+=((100.0-farmer.relation)/100.0);
                        if(farmer.relation>100){
                            farmer.relation=100;
                        }
                    }
                }
                draw_farmer_mode(main_win, context, farmer, User);
            }
        }
        else if(ch=='2'){
            if(User.gold<20+100*((100.0-farmer.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                draw_farmer_mode(main_win, context, farmer, User, false);
                continue;
            }
            else if(User.inv.food.waffle>=10){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of waffles you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Farmer] Confirm you want to purchase a packet of waffles? [y/n]")){
                    User.gold-=20+100*((100.0-farmer.relation)/100.0);
                    User.inv.food.waffle++;
                    if(farmer.relation<100){
                        farmer.relation+=((100.0-farmer.relation)/100.0);
                        if(farmer.relation>100){
                            farmer.relation=100;
                        }
                    }
                }
                draw_farmer_mode(main_win, context, farmer, User);
            }
        }
        else if(ch=='3'){
            if(User.gold<50+150*((100.0-farmer.relation)/100.0)){
                clear_and_draw_dialog(main_win, context, "[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                draw_farmer_mode(main_win, context, farmer, User, false);
                continue;
            }
            else if(User.inv.food.energy_bar>=10){
                clear_and_draw_dialog(main_win, context, "[System] You've reached the limit of energy bars you can carry.");
                continue;
            }
            else{
                if(SDL_getch_y_or_n(main_win, context, "[Farmer] Confirm you want to purchase a energy bar? [y/n]")){
                    User.gold-=50+150*((100.0-farmer.relation)/100.0);
                    User.inv.food.energy_bar++;
                    if(farmer.relation<100){
                        farmer.relation+=((100.0-farmer.relation)/100.0);
                        if(farmer.relation>100){
                            farmer.relation=100;
                        }
                    }
                }
                draw_farmer_mode(main_win, context, farmer, User);
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void char_move(int ch, tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Csr &csr_pos, const std::vector<std::string> &pub_layout){
    if((ch=='a'||ch==SDLK_LEFT)&&csr_pos.first>1&&pub_layout[csr_pos.second][csr_pos.first-1]==' '){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.first--;
        draw_player(main_win, context, csr_pos.first, csr_pos.second);
    }
    if((ch=='d'||ch==SDLK_RIGHT)&&csr_pos.first<78&&pub_layout[csr_pos.second][csr_pos.first+1]==' '){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.first++;
        draw_player(main_win, context, csr_pos.first, csr_pos.second);
    }
    if((ch=='w'||ch==SDLK_UP)&&csr_pos.second>1&&pub_layout[csr_pos.second-1][csr_pos.first]==' '){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.second--;
        draw_player(main_win, context, csr_pos.first, csr_pos.second);
    }
    if((ch=='s'||ch==SDLK_DOWN)&&csr_pos.second<48&&pub_layout[csr_pos.second+1][csr_pos.first]==' '){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.second++;
        draw_player(main_win, context, csr_pos.first, csr_pos.second);
    }
    context->present(*main_win);
}

void draw_menu_selections(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int line, bool is_bold, std::string output){
    if(is_bold){
        tcod::print(*main_win, {0, line}, output, &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*main_win, {0, line}, output, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
}

void draw_bank_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Chest &chest, Bank &bank, unsigned int csr_pos, unsigned long long int steps){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0, 0}, "Bank Menu", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 1}, "[Kiosk] How can we help you?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Store Gears]");
    draw_menu_selections(main_win, context, 4, csr_pos==1, "[Retrieve Gears]");
    draw_menu_selections(main_win, context, 6, csr_pos==2, "[Store Materials and Blueprints]");
    draw_menu_selections(main_win, context, 7, csr_pos==3, "[Retrieve Materials and Blueprints]");
    draw_menu_selections(main_win, context, 9, csr_pos==4, "[Store Gold]");
    draw_menu_selections(main_win, context, 10, csr_pos==5, "[Retrieve Gold]");
    // Draw bank stats
    if(!chest.gear_storage.empty()){
        tcod::print(*main_win, {0, 47}, "Total Gear stored: ["+std::to_string(chest.gear_storage.size())+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*main_win, {0, 47}, "Total Gear stored: ["+std::to_string(chest.gear_storage.size())+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    tcod::print(*main_win, {0, 48}, "Total Gold stored: ["+std::to_string(bank.saved_gold)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(bank.interest_next_applied>0){
        tcod::print(*main_win, {0, 49}, "Next Interest Compound in: ["+std::to_string(bank.interest_next_applied-steps)+"] moves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*main_win, {0, 49}, "Next Interest Compound in: [Invalid] moves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    context->present(*main_win);
}

void bank_interface(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config, Player &User, Chest &chest, Bank &bank){
    unsigned int csr_pos=0;
    int ch;
    draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<5){
            csr_pos++;
            draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            csr_pos--;
            draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            switch(csr_pos){
                case 0:
                    if(!User.inv.item.empty()){
                        inventory_storage(main_win, context, User, perm_config, chest);
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Inventory Empty");
                    }
                    break;
                case 1:
                    if(!chest.gear_storage.empty()){
                        inventory_retrieve(main_win, context, User, perm_config, chest);
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Storage Empty");
                    }
                    break;
                case 2:
                    store_misc_items(main_win, context, User.inv.misc, chest);
                    draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    break;
                case 3:
                    retrieve_misc_items(main_win, context, User.inv.misc, chest);
                    draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    break;
                case 4:{
                    unsigned long long int input=get_ullint(main_win, context, "Please enter a valid amount of gold to store: ");
                    if(input<=User.gold&&input>0){
                        User.gold-=input;
                        bank.saved_gold+=input;
                        if(bank.interest_next_applied==0){
                            bank.interest_next_applied=50000;
                        }
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                        draw_stats(main_win, context, User);
                        clear_and_draw_dialog(main_win, context, "Transfer Successful!");
                    }
                    else{
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    }
                    break;
                }
                case 5:{
                    unsigned long long int input=get_ullint(main_win, context, "Please enter a valid amount of gold to retrieve: ");
                    if(input<=bank.saved_gold&&input>0){
                        bank.saved_gold-=input;
                        User.gold+=input;
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                        draw_stats(main_win, context, User);
                        clear_and_draw_dialog(main_win, context, "Transfer Successful!");
                    }
                    else{
                        draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void draw_gear_merchant(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0, 0}, "Gear Shop", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 1}, "[Merchant] How can I help you?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Buy Gears]");
    draw_menu_selections(main_win, context, 4, csr_pos==1, "[Sell Gears]");
    draw_menu_selections(main_win, context, 6, csr_pos==2, "[Buy Materials/Blueprints/First Aid Kits]");
    context->present(*main_win);
}

void gear_merchant_interface(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &User, NoDelete &perm_config){
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
                        show_merchant_items(main_win, context, gear_merchant, User);
                        draw_stats(main_win, context, User);
                        draw_gear_merchant(main_win, context, csr_pos);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[Merchant] Sorry, out of stock! Please come back later.");
                    }
                    break;
                case 1:
                    if(!User.inv.item.empty()){
                        sell_items_to_merchant(main_win, context, gear_merchant, User, perm_config);
                        draw_stats(main_win, context, User);
                        draw_gear_merchant(main_win, context, csr_pos);
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Error: Inventory Empty");
                    }
                    break;
                case 2:
                    trader_misc_menu(main_win, context, User, gear_merchant);
                    draw_stats(main_win, context, User);
                    draw_gear_merchant(main_win, context, csr_pos);
                    break;
                default:
                    break;
            }
        }
    }
}

void draw_mysterious_trader_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0, 0}, "Mysterious Hut", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 1}, "[Mysterious Trader] ...", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "[Exchange items for Artifact Materials]");
    draw_menu_selections(main_win, context, 5, csr_pos==1, "[Exchange items for Ancient Cores]");
    draw_menu_selections(main_win, context, 7, csr_pos==2, "[Exchange gears for Artifact Gears]");
    context->present(*main_win);
}

void mysterious_trader_interface(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Merchant &mysterious_trader){
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
            show_misc_items(main_win, context, User.inv.misc);
            draw_mysterious_trader_menu(main_win, context, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch==SDLK_RETURN){
            switch(csr_pos){
                case 0:{
                    unsigned long long int amount=get_ullint(main_win, context, "Amount of Artifact Materials: ");
                    if(amount>0&&User.inv.misc.materials.legendary>=5*amount&&User.inv.misc.cores.ancient_core>=amount){
                        User.inv.misc.materials.legendary-=(5*amount);
                        User.inv.misc.cores.ancient_core-=amount;
                        User.inv.misc.materials.artifact+=amount;
                        clear_and_draw_dialog(main_win, context, "[System] Success");
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Failure");
                    }
                    break;
                }
                case 1:{
                    unsigned long long int amount=get_ullint(main_win, context, "Amount of Ancient Cores: ");
                    if(amount>0&&User.inv.misc.cores.crystallium>=amount&&User.inv.misc.cores.mysterious_shard>=amount){
                        User.inv.misc.cores.crystallium-=amount;
                        User.inv.misc.cores.mysterious_shard-=amount;
                        User.inv.misc.cores.ancient_core+=amount;
                        clear_and_draw_dialog(main_win, context, "[System] Success");
                    }
                    else{
                        clear_and_draw_dialog(main_win, context, "[System] Failure");
                    }
                    break;
                }
                case 2:
                    if(!mysterious_trader.store.empty()){
                        mysterious_trader_items(main_win, context, User, mysterious_trader);
                        draw_mysterious_trader_menu(main_win, context, csr_pos);
                        draw_stats(main_win, context, User);
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

void miner_hire_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Miner &miner, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0, 0}, "Miner's Inn", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 1}, "[Innkeeper] Ay matey, what brings you here today?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    int line = 3;
    draw_menu_selections(main_win, context, line, csr_pos==0, "[Hire Miners]");
    line+=2;
    draw_menu_selections(main_win, context, line, csr_pos==1, "[Accumulated Loot]");
    context->present(*main_win);
}

void draw_miner_hire_selections(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miner &miner, unsigned int csr_pos, long long int ideal_payment, long long int actual_payment){
    SDL_wclear_main_win(main_win, context);
    tcod::print(*main_win, {0,1}, "Hire Miners", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    draw_menu_selections(main_win, context, 3, csr_pos==0, "Amount of Miners: "+std::to_string(miner.job.number_of_miners));
    std::stringstream payment_string;
    payment_string<<"Payment ["<<ideal_payment<<"]: "<<actual_payment;
    draw_menu_selections(main_win, context, 5, csr_pos==1, payment_string.str());
    draw_menu_selections(main_win, context, 7, csr_pos==2, "[Done]");
    draw_menu_selections(main_win, context, 8, csr_pos==3, "[Cancel]");
    clear_and_draw_dialog(main_win, context, "Miner's Inn");
}

void miner_hire_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miner &miner, Player &User){
    unsigned int csr_pos=0;
    long long int actual_payment=0, ideal_payment=0;
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
                case 0:
                {
                    miner.job.number_of_miners=get_int(main_win, context, "[System] Please enter the amount of miners you wish to hire: ");
                    ideal_payment=(pow(miner.job.number_of_miners,2))*1000*((1000-miner.relation)/1000);
                    draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
                    break;
                }
                case 1:
                {
                    actual_payment=get_llint(main_win, context, "[System] Please enter the amount you wish to pay: ");
                    draw_miner_hire_selections(main_win, context, miner, csr_pos, ideal_payment, actual_payment);
                    break;
                }
                case 2:
                {
                    if(actual_payment>User.gold){
                        clear_and_draw_dialog(main_win, context, "[System] Insufficient gold");
                        continue;
                    }
                    else if(miner.job.number_of_miners>10){
                        clear_and_draw_dialog(main_win, context, "[System] You cannot hire more than 10 miners");
                        continue;
                    }
                    else if(miner.job.number_of_miners<=0){
                        clear_and_draw_dialog(main_win, context, "[System] Value of hired miners must be more than 0");
                        continue;
                    }
                    break;
                }
                case 3:
                    miner.job=Miner::Job_Details();
                    return;
                default:
                    break;
            }

        }
        else if(ch=='q'){
            return;
        }
    }
}

void miner_hire_interface(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Miner &miner, Archaeologist &archaeologist){
    unsigned int csr_pos = 0;
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
                    miner_hire_selection(main_win, context, miner, User);
                    miner_hire_menu(main_win, context, miner, csr_pos);
                }
                else{
                    const std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                    std::stringstream ss;
                    Time time_left{86400-std::chrono::duration_cast<std::chrono::seconds>(now-miner.job.job_start).count()};
                    ss<<"[System] Time left: "<<time_left.hours<<" hours "<<time_left.minutes<<" minutes "<<time_left.seconds<<" seconds";
                    clear_and_draw_dialog(main_win, context, ss.str());
                }
            }
        }
    }
}

void bar_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NPC &npc, NoDelete &perm_config){
    Csr csr_pos{78, 1};
    refresh_gear_merchant_store(npc.gear_merchant, User.steps);
    refresh_mysterious_merchant_store(npc.mysterious_trader, User.steps);
    std::ifstream pub_layout_file("res/bar_layout.txt");
    std::vector<std::string> pub_layout; // {50,80}
    for(int i=0; i<50; i++){
        std::string line;
        std::getline(pub_layout_file, line);
        pub_layout.push_back(line);
    }
    redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==SDLK_LEFT||ch==SDLK_RIGHT||ch==SDLK_DOWN||ch==SDLK_UP){
            char_move(ch, main_win, context, User, csr_pos, pub_layout);
        }
        else if(ch=='x'){
            char target=search_surroundings(pub_layout, csr_pos.first, csr_pos.second);
            if(target=='M'){
                mysterious_trader_interface(main_win, context, User, npc.mysterious_trader);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='B'){
                bartender_interface(main_win, context, npc.bartender, User);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='F'){
                farmer_interface(main_win, context, npc.farmer, User);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='G'){
                gear_merchant_interface(main_win, context, npc.gear_merchant, User, perm_config);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='T'){
                bank_interface(main_win, context, perm_config, User, npc.chest, npc.bank);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='S'){
                reforge_repair_mode(main_win, context, User, perm_config);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='I'){
                miner_hire_interface(main_win, context, User, npc.miner, npc.archaeologist);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
            else if(target=='A'){
                // Archaeologist interface
            }
        }
        else if(ch=='c'){
            if(csr_pos.first==78&&csr_pos.second==1){
                return;
            }
        }
        else if(ch=='i'){
            if(!User.inv.item.empty()){
                inventory_mode(main_win, context, User, perm_config);
                redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
            }
        }
        else if(ch=='e'){
            eat_drink_mode(main_win, context, User);
            redraw_bar(main_win, context, User, npc.gear_merchant, pub_layout, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
    }
}
