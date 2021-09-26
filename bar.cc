#include <fstream>
#include <random>
#include <sstream>
#include "headers/mode.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
void refresh_gear_merchant_store(Merchant &gear_merchant){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> item_type(1, 100);
    for(int i=0; i<20; i++){
        int item=item_type(generator);
        if(item<20){
            gear_merchant.store.push_back(generate_loot_from_rarity_type('c'));
        }
        else if(item<40){
            gear_merchant.store.push_back(generate_loot_from_rarity_type('u'));
        }
        else if(item<70){
            gear_merchant.store.push_back(generate_loot_from_rarity_type('r'));
        }
        else if(item<90){
            gear_merchant.store.push_back(generate_loot_from_rarity_type('e'));
        }
        else{
            gear_merchant.store.push_back(generate_loot_from_rarity_type('l'));
        }
    }
}
void redraw_bar(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::vector<std::string> pub_layout, Csr csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    for(int i=0; i<pub_layout.size(); i++){
        for(int j=0; j<pub_layout[i].size(); j++){
            if(pub_layout[i][j]=='.'){
                TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], GREEN, BLACK);
            }
            else if(pub_layout[i][j]=='~'){
                TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], LIGHT_BLUE, BLACK);
            }
            else if(pub_layout[i][j]=='{'||pub_layout[i][j]=='}'){
                TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], DARK_RED, BLACK);
            }
            else if(pub_layout[i][j]=='M'||pub_layout[i][j]=='B'||pub_layout[i][j]=='F'||pub_layout[i][j]=='G'||pub_layout[i][j]=='T'||pub_layout[i][j]=='S'){
                TCOD_console_put_char_ex(main_win.get(), j, i+1, '@', MAGENTA, BLACK);
            }
            else{
                TCOD_console_put_char_ex(main_win.get(), j, i+1, pub_layout[i][j], WHITE, BLACK);
            }
        }
    }
    TCOD_console_put_char_ex(main_win.get(), 78, 2, '>', WHITE, BLACK);
    draw_player(main_win, context, csr_pos.first, csr_pos.second);
    tcod::print(*main_win, {0,0}, "Pub", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}
void refresh_mysterious_merchant_store(Merchant &mysterious_trader){
    mysterious_trader.store.push_back(generate_loot_from_rarity_type('a'));
    mysterious_trader.store.push_back(generate_loot_from_rarity_type('a'));
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
    tcod::print(*main_win, {0,3}, "[Bartender] How can I serve you today?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    std::stringstream output;
    tcod::print(*main_win, {0,5}, "1. A bottle of water (Bottle included)", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output<<"Price: "<<5+50*((100.0-bartender.relation)/100.0);
    tcod::print(*main_win, {0,6}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,8}, "2. A can of sparking juice", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    output<<"Price: "<<20+100*((100.0-bartender.relation)/100.0);
    tcod::print(*main_win, {0,9}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    tcod::print(*main_win, {0,13}, "Backpack:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output<<"Water:"<<User.inv.water.water<<"/8"<<" Sparking Juice:"<<User.inv.water.sparkling_juice<<"/10";
    tcod::print(*main_win, {0,14}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}
void bartender_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Bartender &bartender, Player &User){
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
                clear_and_draw_dialog(main_win, context, "[Bartender] Confirm you want to purchase a bottle of water? [y/n]");
                while(true){
                    ch=SDL_getch(main_win, context);
                    if(ch>0&&ch<128){
                        break;
                    }
                }
                if(ch=='y'){
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
        if(ch=='2'){
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
                clear_and_draw_dialog(main_win, context, "[Bartender] Confirm you want to purchase a can of sparkling water? [y/n]");
                while(true){
                    ch=SDL_getch(main_win, context);
                    if(ch>0&&ch<128){
                        break;
                    }
                }
                if(ch=='y'){
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
        if(ch=='q'){
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
    tcod::print(*main_win, {0,3}, "[Farmer] What brings you here today?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    std::stringstream output;
    tcod::print(*main_win, {0,5}, "1. A piece of bread", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output<<"Price: "<<5+50*((100.0-farmer.relation)/100.0);
    tcod::print(*main_win, {0,6}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,8}, "2. A packet of waffles", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    output<<"Price: "<<20+100*((100.0-farmer.relation)/100.0);
    tcod::print(*main_win, {0,9}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    tcod::print(*main_win, {0,11}, "3. An energy bar", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output<<"Price: "<<50+150*((100.0-farmer.relation)/100.0);
    tcod::print(*main_win, {0,12}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output.str(std::string());
    tcod::print(*main_win, {0,16}, "Backpack:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    output<<"Bread:"<<User.inv.food.bread<<"/8"<<" Waffles:"<<User.inv.food.waffle<<"/10"<<" Energy Bar:"<<User.inv.food.energy_bar<<"/10";
    tcod::print(*main_win, {0,17}, output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*main_win);
}
void farmer_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Farmer &farmer, Player &User){
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
                clear_and_draw_dialog(main_win, context, "[Farmer] Confirm you want to purchase a piece of bread? [y/n]");
                while(true){
                    ch=SDL_getch(main_win, context);
                    if(ch>0&&ch<128){
                        break;
                    }
                }
                if(ch=='y'){
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
        if(ch=='2'){
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
                clear_and_draw_dialog(main_win, context, "[Farmer] Confirm you want to purchase a packet of waffles? [y/n]");
                while(true){
                    ch=SDL_getch(main_win, context);
                    if(ch>0&&ch<128){
                        break;
                    }
                }
                if(ch=='y'){
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
        if(ch=='3'){
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
                clear_and_draw_dialog(main_win, context, "[Farmer] Confirm you want to purchase a energy bar? [y/n]");
                while(true){
                    ch=SDL_getch(main_win, context);
                    if(ch>0&&ch<128){
                        break;
                    }
                }
                if(ch=='y'){
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
        if(ch=='q'){
            return;
        }
    }
}
void char_move(int ch, tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Csr &csr_pos, const std::vector<std::string> &pub_layout){
    if((ch=='a'||ch==SDLK_LEFT)&&csr_pos.first>1&&pub_layout[csr_pos.second][csr_pos.first-1]==' '){
        csr_pos.first--;
        redraw_bar(main_win, context, pub_layout, csr_pos);
    }
    if((ch=='d'||ch==SDLK_RIGHT)&&csr_pos.first<78&&pub_layout[csr_pos.second][csr_pos.first+1]==' '){
        csr_pos.first++;
        redraw_bar(main_win, context, pub_layout, csr_pos);
    }
    if((ch=='w'||ch==SDLK_UP)&&csr_pos.second>1&&pub_layout[csr_pos.second-1][csr_pos.first]==' '){
        csr_pos.second--;
        redraw_bar(main_win, context, pub_layout, csr_pos);
    }
    if((ch=='s'||ch==SDLK_DOWN)&&csr_pos.second<48&&pub_layout[csr_pos.second+1][csr_pos.first]==' '){
        csr_pos.second++;
        redraw_bar(main_win, context, pub_layout, csr_pos);
    }
}
void draw_bank_menu_selections(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int line, bool is_bold, std::string output){
    if(is_bold){
        tcod::print(*main_win, {0,line}, output, &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*main_win, {0,line}, output, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
}
void draw_bank_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Chest &chest, Bank &bank, unsigned int csr_pos, unsigned long long steps){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0,0}, "Bank Menu", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,1}, "[Kiosk] How can we help you?", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    draw_bank_menu_selections(main_win, context, 3, csr_pos==0, "[Store Gears]");
    draw_bank_menu_selections(main_win, context, 4, csr_pos==1, "[Retrieve Gears]");
    draw_bank_menu_selections(main_win, context, 6, csr_pos==2, "[Store Materials and Blueprints]");
    draw_bank_menu_selections(main_win, context, 7, csr_pos==3, "[Retrieve Materials and Blueprints]");
    draw_bank_menu_selections(main_win, context, 9, csr_pos==4, "[Store Gold]");
    draw_bank_menu_selections(main_win, context, 10, csr_pos==5, "[Retrieve Gold]");
    // Draw bank stats
    if(!chest.gear_storage.empty()){
        tcod::print(*main_win, {0,47}, "Total Gear stored: ["+std::to_string(chest.gear_storage.size())+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*main_win, {0,47}, "Total Gear stored: ["+std::to_string(chest.gear_storage.size())+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    tcod::print(*main_win, {0,48}, "Total Gold stored: ["+std::to_string(bank.saved_gold)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(bank.saved_gold>0){
        tcod::print(*main_win, {0,49}, "Next Interest Compound in: ["+std::to_string(1000-steps-bank.storage_last_applied)+"] moves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*main_win, {0,49}, "Next Interest Compound in: [Invalid] moves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    context->present(*main_win);
}
void bank_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config, Player &User, Chest &chest, Bank &bank){
    unsigned int csr_pos=0;
    int ch;
    draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<5){
            csr_pos++;
            draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
        }
        if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            csr_pos--;
            draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
        }
        if(ch=='q'){
            return;
        }
        if(ch==SDLK_RETURN){
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
                    store_misc_items(main_win, context, User.inv.misc);
                    draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    break;
                case 3:
                    retrieve_misc_items(main_win, context, User.inv.misc);
                    draw_bank_menu(main_win, context, chest, bank, csr_pos, User.steps);
                    break;
                default:
                    break;
            }
        }
    }
}
void bar_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NPC &npc, NoDelete &perm_config){
    Csr csr_pos{78,1};
    std::ifstream pub_layout_file("res/bar_layout.txt");
    std::vector<std::string> pub_layout; // {50,80}
    for(int i=0; i<50; i++){
        std::string line;
        std::getline(pub_layout_file, line);
        pub_layout.push_back(line);
    }
    redraw_bar(main_win, context, pub_layout, csr_pos);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==SDLK_LEFT||ch==SDLK_RIGHT||ch==SDLK_DOWN||ch==SDLK_UP){
            char_move(ch, main_win, context, csr_pos, pub_layout);
        }
        if(ch=='x'){
            char target=search_surroundings(pub_layout, csr_pos.first, csr_pos.second);
            if(target=='0'){
                continue;
            }
            else if(target=='M'){
                // Mysterious Trader
            }
            else if(target=='B'){
                bartender_mode(main_win, context, npc.bartender, User);
                redraw_bar(main_win, context, pub_layout, csr_pos);
            }
            else if(target=='F'){
                farmer_mode(main_win, context, npc.farmer, User);
                redraw_bar(main_win, context, pub_layout, csr_pos);
            }
            else if(target=='G'){
                // Gear Merchant & Sells First Aid Kits
            }
            else if(target=='T'){
                bank_mode(main_win, context, perm_config, User, npc.chest, npc.bank);
                redraw_bar(main_win, context, pub_layout, csr_pos);
            }
            else if(target=='S'){
                reforge_repair_mode(main_win, context, User, perm_config);
                redraw_bar(main_win, context, pub_layout, csr_pos);
            }
        }
        if(ch=='r'){
            redraw_bar(main_win, context, pub_layout, csr_pos);
        }
        if(ch=='c'){
            if(csr_pos.first==78&&csr_pos.second==1){
                return;
            }
        }
        if(ch=='i'){
            if(!User.inv.item.empty()){
                inventory_mode(main_win, context, User, perm_config);
                redraw_bar(main_win, context, pub_layout, csr_pos);
            }
        }
        if(ch=='e'){
            eat_drink_mode(main_win, context, User);
            redraw_bar(main_win, context, pub_layout, csr_pos);
        }
        if(ch=='q'){
            return;
        }
    }
}
