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
    for(int i = 0; i<20; i++){
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
void redraw_bar(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, WINDOW *interaction_bar, std::vector<std::string> pub_layout, Csr csr_pos){
    wclear(main_win);
    wclear(interaction_bar);
    mvwaddstr(interaction_bar,0,0,"Pub");
    for(int i = 0; i<pub_layout.size(); i++){
        for(int j = 0; j<pub_layout[i].size(); j++){
            if(pub_layout[i][j]=='.'){
                wattron(main_win,COLOR_PAIR(6));
                mvwaddch(main_win,i,j,pub_layout[i][j]);
                wattroff(main_win,COLOR_PAIR(6));
            }
            else if(pub_layout[i][j]=='~'){
                wattron(main_win,COLOR_PAIR(7));
                mvwaddch(main_win,i,j,pub_layout[i][j]);
                wattroff(main_win,COLOR_PAIR(7));
            }
            else if(pub_layout[i][j]=='{'||pub_layout[i][j]=='}'){
                wattron(main_win,COLOR_PAIR(10));
                mvwaddch(main_win,i,j,pub_layout[i][j]);
                wattroff(main_win,COLOR_PAIR(10));
            }
            else if(pub_layout[i][j]=='M'||pub_layout[i][j]=='B'||pub_layout[i][j]=='F'||pub_layout[i][j]=='G'||pub_layout[i][j]=='T'||pub_layout[i][j]=='S'){
                wattron(main_win,COLOR_PAIR(12));
                mvwaddch(main_win,i,j,'@');
                wattroff(main_win,COLOR_PAIR(12));
            }
            else{
                mvwaddch(main_win,i,j,pub_layout[i][j]);
            }
        }
    }
    wattron(main_win,COLOR_PAIR(12));
    mvwaddch(main_win,1,78,'>');
    wattroff(main_win,COLOR_PAIR(12));
    draw_player(main_win, csr_pos.first, csr_pos.second);
    wrefresh(interaction_bar);
    wrefresh(main_win);
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
void draw_bartender_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, Bartender bartender, Player &User){
    wclear(main_win);
    mvwaddstr(main_win,2,0,"[Bartender] How can I serve you today?");
    std::stringstream output;
    mvwaddstr(main_win,4,0,"1. A bottle of water (Bottle included)");
    output<<"Price: "<<5+50*((100.0-bartender.relation)/100.0);
    mvwaddstr(main_win,5,0,output.str().c_str());
    mvwaddstr(main_win,7,0,"2. A can of sparking juice (Might contain diabetes so don't drink too much)");
    output.str(std::string());
    output<<"Price: "<<20+100*((100.0-bartender.relation)/100.0);
    mvwaddstr(main_win,8,0,output.str().c_str());
    output.str(std::string());
    mvwaddstr(main_win,12,0,"Backpack:");
    output<<"Water:"<<User.inv.water.water<<"/8"<<" Sparking Juice:"<<User.inv.water.sparkling_juice<<"/10";
    mvwaddstr(main_win,13,0,output.str().c_str());
    wrefresh(main_win);
}
void draw_bartender_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context,  Bartender bartender, Player &User){
    wclear(main_win);
    wclear(interaction_bar);
    draw_stats(status_win, User);
    mvwaddstr(interaction_bar,0,0,"[Bartender] Welcome to the Bar!");
    mvwaddstr(main_win,2,0,"[Bartender] How can I serve you today?");
    std::stringstream output;
    mvwaddstr(main_win,4,0,"1. A bottle of water (Bottle included)");
    output<<"Price: "<<5+50*((100.0-bartender.relation)/100.0);
    mvwaddstr(main_win,5,0,output.str().c_str());
    mvwaddstr(main_win,7,0,"2. A can of sparking juice (Might contain diabetes so don't drink too much)");
    output.str(std::string());
    output<<"Price: "<<20+100*((100.0-bartender.relation)/100.0);
    mvwaddstr(main_win,8,0,output.str().c_str());
    output.str(std::string());
    mvwaddstr(main_win,12,0,"Backpack:");
    output<<"Water:"<<User.inv.water.water<<"/8"<<" Sparking Juice:"<<User.inv.water.sparkling_juice<<"/10";
    mvwaddstr(main_win,13,0,output.str().c_str());
    wrefresh(main_win);
    wrefresh(interaction_bar);
}
void bartender_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context,  Bartender &bartender, Player &User){
    draw_bartender_mode(main_win, interaction_bar, status_win, bartender, User);
    int ch;
    while(true){
        ch=wgetch(main_win);
        if(ch=='1'){
            if(User.gold<5+50*((100.0-bartender.relation)/100.0)){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Bartender] Don't waste my time if you don't have enough gold.");
                if(bartender.relation>=5){
                    bartender.relation-=5;
                }
                wrefresh(interaction_bar);
                draw_bartender_mode(main_win, bartender, User);
                continue;
            }
            else if(User.inv.water.water>=8){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[System] You've reached the limit of water you can carry.");
                wrefresh(interaction_bar);
                continue;
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Bartender] Confirm you want to purchase a bottle of water? [y/n]");
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
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
                draw_bartender_mode(main_win, interaction_bar, status_win, bartender, User);
            }
        }
        if(ch=='2'){
            if(User.gold<20+100*((100.0-bartender.relation)/100.0)){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Bartender] Don't waste my time if you don't have enough gold.");
                if(bartender.relation>=5){
                    bartender.relation-=5;
                }
                wrefresh(interaction_bar);
                draw_bartender_mode(main_win, bartender, User);
                continue;
            }
            else if(User.inv.water.sparkling_juice>=10){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[System] You've reached the limit of sparking water you can carry.");
                wrefresh(interaction_bar);
                continue;
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Bartender] Confirm you want to purchase a can of sparkling water? [y/n]");
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
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
                draw_bartender_mode(main_win, interaction_bar, status_win, bartender, User);
            }
        }
        if(ch=='q'){
            return;
        }
    }
}
void draw_farmer_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, Farmer farmer, Player &User){
    wclear(main_win);
    mvwaddstr(main_win,2,0,"[Farmer] What brings you here today?");
    std::stringstream output;
    mvwaddstr(main_win,4,0,"1. A piece of bread");
    output<<"Price: "<<5+50*((100.0-farmer.relation)/100.0);
    mvwaddstr(main_win,5,0,output.str().c_str());
    mvwaddstr(main_win,7,0,"2. A packet of waffles");
    output.str(std::string());
    output<<"Price: "<<20+100*((100.0-farmer.relation)/100.0);
    mvwaddstr(main_win,8,0,output.str().c_str());
    output.str(std::string());
    mvwaddstr(main_win,10,0,"3. An energy bar");
    output<<"Price: "<<50+150*((100.0-farmer.relation)/100.0);
    mvwaddstr(main_win,11,0,output.str().c_str());
    output.str(std::string());
    mvwaddstr(main_win,15,0,"Backpack:");
    output<<"Bread:"<<User.inv.food.bread<<"/8"<<" Waffles:"<<User.inv.food.waffle<<"/10"<<" Energy Bar:"<<User.inv.food.energy_bar<<"/10";
    mvwaddstr(main_win,16,0,output.str().c_str());
    wrefresh(main_win);
}
void draw_farmer_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context,  Farmer farmer, Player &User){
    wclear(main_win);
    wclear(interaction_bar);
    draw_stats(status_win, User);
    mvwaddstr(interaction_bar,0,0,"[Farmer] Welcome to my farm!");
    mvwaddstr(main_win,2,0,"[Farmer] What brings you here today?");
    std::stringstream output;
    mvwaddstr(main_win,4,0,"1. A piece of bread");
    output<<"Price: "<<5+50*((100.0-farmer.relation)/100.0);
    mvwaddstr(main_win,5,0,output.str().c_str());
    mvwaddstr(main_win,7,0,"2. A packet of waffles");
    output.str(std::string());
    output<<"Price: "<<20+100*((100.0-farmer.relation)/100.0);
    mvwaddstr(main_win,8,0,output.str().c_str());
    output.str(std::string());
    mvwaddstr(main_win,10,0,"3. An energy bar");
    output<<"Price: "<<50+150*((100.0-farmer.relation)/100.0);
    mvwaddstr(main_win,11,0,output.str().c_str());
    output.str(std::string());
    mvwaddstr(main_win,15,0,"Backpack:");
    output<<"Bread:"<<User.inv.food.bread<<"/8"<<" Waffles:"<<User.inv.food.waffle<<"/10"<<" Energy Bar:"<<User.inv.food.energy_bar<<"/10";
    mvwaddstr(main_win,16,0,output.str().c_str());
    wrefresh(main_win);
    wrefresh(interaction_bar);
}
void farmer_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context,  Farmer &farmer, Player &User){
    draw_farmer_mode(main_win, interaction_bar, status_win, farmer, User);
    int ch;
    while(true){
        ch=wgetch(main_win);
        if(ch=='1'){
            if(User.gold<5+50*((100.0-farmer.relation)/100.0)){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                wrefresh(interaction_bar);
                draw_farmer_mode(main_win, farmer, User);
                continue;
            }
            else if(User.inv.food.bread>=8){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] You've reached the limit of bread you can carry.");
                wrefresh(interaction_bar);
                continue;
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] Confirm you want to purchase a piece of bread? [y/n]");
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
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
                draw_farmer_mode(main_win, interaction_bar, status_win, farmer, User);
            }
        }
        if(ch=='2'){
            if(User.gold<20+100*((100.0-farmer.relation)/100.0)){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                wrefresh(interaction_bar);
                draw_farmer_mode(main_win, farmer, User);
                continue;
            }
            else if(User.inv.food.waffle>=10){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] You've reached the limit of waffles you can carry.");
                wrefresh(interaction_bar);
                continue;
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] Confirm you want to purchase a packet of waffles? [y/n]");
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
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
                draw_farmer_mode(main_win, interaction_bar, status_win, farmer, User);
            }
        }
        if(ch=='3'){
            if(User.gold<50+150*((100.0-farmer.relation)/100.0)){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] Don't waste my time if you don't have enough gold.");
                if(farmer.relation>=5){
                    farmer.relation-=5;
                }
                wrefresh(interaction_bar);
                draw_farmer_mode(main_win, farmer, User);
                continue;
            }
            else if(User.inv.food.energy_bar>=10){
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] You've reached the limit of energy bars you can carry.");
                wrefresh(interaction_bar);
                continue;
            }
            else{
                wclear(interaction_bar);
                mvwaddstr(interaction_bar,0,0,"[Farmer] Confirm you want to purchase a energy bar? [y/n]");
                wrefresh(interaction_bar);
                ch=wgetch(main_win);
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
                draw_farmer_mode(main_win, interaction_bar, status_win, farmer, User);
            }
        }
        if(ch=='q'){
            return;
        }
    }
}
void draw_blacksmith_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, WINDOW *interaction_bar, WINDOW *status_bar, Player &User){

}
void blacksmith_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, WINDOW *interaction_bar, WINDOW *status_bar, Player &User){
    draw_blacksmith_mode(main_win, interaction_bar, status_bar, User);
}
void char_move(int ch, std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, Csr &csr_pos, const std::vector<std::string> &pub_layout){
    if((ch=='a'||ch==KEY_LEFT)&&csr_pos.first>1&&pub_layout[csr_pos.second][csr_pos.first-1]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first,' ');
        csr_pos.first--;
    }
    if((ch=='d'||ch==KEY_RIGHT)&&csr_pos.first<78&&pub_layout[csr_pos.second][csr_pos.first+1]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.first++;
    }
    if((ch=='w'||ch==KEY_UP)&&csr_pos.second>1&&pub_layout[csr_pos.second-1][csr_pos.first]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first, ' ');
        csr_pos.second--;
    }
    if((ch=='s'||ch==KEY_DOWN)&&csr_pos.second<48&&pub_layout[csr_pos.second+1][csr_pos.first]==' '){
        mvwaddch(main_win, csr_pos.second, csr_pos.first,' ');
        csr_pos.second++;
    }
}
void bar_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context,  Player &User, NPC &npc){
    Csr csr_pos{78,1};
    std::ifstream pub_layout_file("res/bar_layout.txt");
    std::vector<std::string> pub_layout; // {50,80}
    for(int i = 0; i<50; i++){
        std::string line; std::getline(pub_layout_file,line);
        pub_layout.push_back(line);
    }
    redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
    int ch;
    while(true){
        ch=wgetch(main_win);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==KEY_LEFT||ch==KEY_RIGHT||ch==KEY_DOWN||ch==KEY_UP){
            char_move(ch, main_win, csr_pos, pub_layout);
            redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
        }
        if(ch=='x'){
            char target = search_surroundings(pub_layout, csr_pos.first, csr_pos.second);
            if(target=='0'){
                continue;
            }
            else if(target=='M'){
                // Mysterious Trader
            }
            else if(target=='B'){
                bartender_mode(main_win, interaction_bar, status_win, npc.bartender, User);
                redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
            }
            else if(target=='F'){
                farmer_mode(main_win, interaction_bar, status_win, npc.farmer, User);
                redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
            }
            else if(target=='G'){
                // Gear Merchant & Sells First Aid Kits
            }
            else if(target=='T'){
                // Bank & Chest
                // show_misc_items()
            }
            else if(target=='S'){
                if(!User.inv.item.empty()){
                    reforge_repair_mode(main_win, status_win, interaction_bar, User);
                    redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
                }
            }
        }
        if(ch=='r'){
            redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
        }
        if(ch=='c'){
            if(csr_pos.first==78&&csr_pos.second==1){
                return;
            }
        }
        if(ch=='i'){
            if(!User.inv.item.empty()){
                inventory_mode(main_win, status_win, interaction_bar, User);
                redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
            }
        }
        if(ch=='e'){
            eat_drink_mode(main_win, status_win, User);
            redraw_bar(main_win, interaction_bar, pub_layout, csr_pos);
        }
        if(ch=='q'){
            return;
        }
    }
}
