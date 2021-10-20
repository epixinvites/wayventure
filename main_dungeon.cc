#include <vector>
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include "headers/mode.h"
#include "headers/main.h"

bool check_surroundings(std::vector<Monster> monsters, int x, int y){
    for(int i=0; i<monsters.size(); i++){
        if(x==monsters[i].x&&y==monsters[i].y){
            return true;
        }
    }
    return false;
}

std::pair<int, Dungeon> check_monster_pos(std::vector<Monster> monsters, int x, int y){
    for(int i=0; i<monsters.size(); i++){
        if(x==monsters[i].x&&y==monsters[i].y){
            return {i, monsters[i].type};
        }
    }
    return {-1, Dungeon::NONE};
}

void char_move(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int ch, Csr &csr_pos, const std::vector<Monster> &monsters, Player &user, Npc &npc, Level Current){
    bool require_move=false;
    if((ch=='a'||ch==SDLK_LEFT)&&csr_pos.first>1&&!check_surroundings(monsters, csr_pos.first-1, csr_pos.second)){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.first--;
        require_move=true;
    }
    else if((ch=='d'||ch==SDLK_RIGHT)&&csr_pos.first<78&&!check_surroundings(monsters, csr_pos.first+1, csr_pos.second)){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.first++;
        require_move=true;
    }
    else if((ch=='w'||ch==SDLK_UP)&&csr_pos.second>1&&!check_surroundings(monsters, csr_pos.first, csr_pos.second-1)){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.second--;
        require_move=true;
    }
    else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos.second<48&&!check_surroundings(monsters, csr_pos.first, csr_pos.second+1)){
        TCOD_console_put_char_ex(main_win.get(), csr_pos.first, csr_pos.second+1, ' ', WHITE, BLACK);
        csr_pos.second++;
        require_move=true;
    }
    if(require_move){
        user.steps++;
        if(user.steps%25==0){
            user.hydration--;
        }
        if(user.steps%35==0){
            user.saturation--;
        }
        if(user.steps==npc.bank.interest_next_applied){
            npc.bank.interest_next_applied+=50000;
            npc.bank.saved_gold*=npc.bank.storage_interest;
        }
        draw_doors(main_win, context, Current);
        draw_player(main_win, context, csr_pos.first, csr_pos.second);
        draw_stats(main_win, context, user);
        context->present(*main_win);
    }
}

void process_gear(Player &user, Item *&processed_item, int damage){
    if(processed_item!=nullptr){
        processed_item->uses++;
        processed_item->durability-=((damage/500.0)*(1+((101-processed_item->durability)/100)))/rarity_value(processed_item->rarity);
        if(processed_item->durability<=0.0){
            processed_item->durability=0;
            processed_item->is_equipped=false;
            processed_item=nullptr;
        }
        else{
            processed_item->reinitialize_item();
        }
    }
}

std::pair<std::string, std::string> calculate_damage(Player &user, Monster_Stats &monster){
    std::stringstream first, second;
    if(user.crit_chance>0&&return_chance(user.crit_chance)){ // Calculate damage that enemy takes
        monster.hp=monster.hp-(user.attk*(1+(user.crit_dmg/100.0)));
        first << "Critical Hit: " << "You dealt " << user.attk*(1+(user.crit_dmg/100.0)) << " damage";
    }
    else{
        monster.hp=monster.hp-(user.attk-monster.def);
        first << "You dealt " << user.attk-monster.def << " damage";
    }
    if(user.def<monster.attk){ // Calculate damage I take
        if(user.cur_shield>monster.attk){
            user.cur_shield-=monster.attk;
            second << "You blocked " << monster.attk << " damage";
        }
        else if(user.cur_shield<monster.attk&&user.cur_shield>0){
            user.cur_hp-=(monster.attk-user.cur_shield);
            second << "You received " << monster.attk-user.cur_shield << " damage";
            user.cur_shield=0;
        }
        else{
            user.cur_hp-=(monster.attk-user.def);
            second << "You received " << monster.attk-user.def << " damage";
        }
    }
    else{
        second << "You received no damage";
    }
    user.uninitialize_stats();
    process_gear(user, user.equip.helmet, monster.attk);
    process_gear(user, user.equip.chestplate, monster.attk);
    process_gear(user, user.equip.greaves, monster.attk);
    process_gear(user, user.equip.boots, monster.attk);
    process_gear(user, user.equip.shield, monster.attk);
    process_gear(user, user.equip.weapon, monster.attk);
    user.initialize_stats();
    return {first.str(), second.str()};
}

bool player_battle(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &user, Level Current, Dungeon monster_type){
    Monster_Stats monster=create_monster(Current, monster_type);
    std::pair<std::string, std::string> log;
    int ch;
    while(true){
        SDL_wclear_main_win(main_win, context);
        draw_border(main_win, context);
        std::stringstream user_output;
        std::stringstream enemy_output;
        draw_stats(main_win, context, user);
        user_output << username << "~ HP:" << user.cur_hp << " Defence:" << user.def << " Shield:" << user.cur_shield << " Heal:" << user.inv.misc.heal_amount;
        enemy_output << "Enemy~ HP:" << monster.hp << " Attk:" << monster.attk << " Def:" << monster.def;
        tcod::print(*main_win, {1, 49}, user_output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        tcod::print(*main_win, {1, 2}, enemy_output.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        tcod::print(*main_win, {39, 25}, log.first, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_CENTER);
        tcod::print(*main_win, {39, 26}, log.second, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_CENTER);
        context->present(*main_win);
        ch=SDL_getch(main_win, context);
        if(ch=='1'){
            log=calculate_damage(user, monster);
            if(user.cur_hp<=0){
                return false;
            }
            if(monster.hp<=0){
                user.gold+=generate_gold(monster_type);
                Item loot=generate_loot_from_monster_type(monster_type);
                SDL_wclear_main_win(main_win, context);
                tcod::print(*main_win, {0, 1}, "Press any key to keep and [r] to trash", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
                draw_stats(main_win, context, user);
                print_description(main_win, context, &loot, 1);

                ch=SDL_getch(main_win, context);
                while(!(ch>0&&ch<128)){
                    ch=SDL_getch(main_win, context);
                }
                if(ch=='r'){
                    return true;
                }
                user.add_item(loot);
                return true;
            }
        }
        else if(ch=='2'){
            if(user.inv.misc.heal_amount>0&&user.cur_hp!=user.ori_hp){
                user.inv.misc.heal_amount--;
                user.cur_hp=user.ori_hp;
            }
        }
    }
}

std::pair<bool, bool> attack_monster(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::vector<Monster> &monsters, Csr csr_pos, Player &user, Level Current){
    std::pair<bool, bool> attack_status{false, false}; // .first = if surroundings have monsters, .second is_alive
    std::pair<int, Dungeon> pos;
    if(check_surroundings(monsters, csr_pos.first-1, csr_pos.second)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first-1, csr_pos.second);
        attack_status.second=player_battle(main_win, context, user, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    else if(check_surroundings(monsters, csr_pos.first+1, csr_pos.second)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first+1, csr_pos.second);
        attack_status.second=player_battle(main_win, context, user, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    else if(check_surroundings(monsters, csr_pos.first, csr_pos.second-1)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first, csr_pos.second-1);
        attack_status.second=player_battle(main_win, context, user, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    else if(check_surroundings(monsters, csr_pos.first, csr_pos.second+1)){
        attack_status.first=true;
        pos=check_monster_pos(monsters, csr_pos.first, csr_pos.second+1);
        attack_status.second=player_battle(main_win, context, user, Current, pos.second);
        monsters.erase(monsters.begin()+pos.first);
    }
    return {attack_status};
}

void move_door(std::vector<std::pair<int, int>> &doors, std::vector<Monster> &monsters, Level &Current, Csr &csr_pos){
    for(int i=0; i<doors.size(); i++){
        if(doors[i].first==csr_pos.first&&doors[i].second==csr_pos.second-1){
            ++Current.y;
            csr_pos={39, 48};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first&&doors[i].second==csr_pos.second+1){
            --Current.y;
            csr_pos={39, 1};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first+1&&doors[i].second==csr_pos.second){
            ++Current.x;
            csr_pos={1, 24};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
        else if(doors[i].first==csr_pos.first-1&&doors[i].second==csr_pos.second){
            --Current.x;
            csr_pos={78, 24};
            doors.clear();
            monsters.clear();
            generate_doors(doors, Current);
            generate_monsters(monsters, Current, csr_pos);
            break;
        }
    }
}

bool move_staircase(Level &Current, Csr csr_pos){
    if(Current.lvl<5&&Current.x==5&&Current.y==5){ // go down
        if(csr_pos.first==39&&csr_pos.second==24){
            Current.lvl++;
            Current.reset(0);
            return true;
        }
    }
    if(Current.lvl>1&&Current.x==1&&Current.y==1){ // go down
        if(csr_pos.first==39&&csr_pos.second==24){
            Current.lvl--;
            Current.reset(1);
            return true;
        }
    }
    return false;
}

void print_food(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &user){
    SDL_wclear_main_win(main_win, context);
    std::stringstream ss;
    ss << "[1] Bread (30 Saturation Points): " << user.inv.food.bread;
    tcod::print(*main_win, {0, 1}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss << "[2] Waffle (50 Saturation Points): " << user.inv.food.waffle;
    tcod::print(*main_win, {0, 2}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss << "[3] Energy Bar (100 Saturation Points): " << user.inv.food.energy_bar;
    tcod::print(*main_win, {0, 3}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss << "[4] Water (50 Hydration Points): " << user.inv.water.water;
    tcod::print(*main_win, {0, 4}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    ss << "[5] Sparkling Water (100 Hydration Points): " << user.inv.water.sparkling_juice;
    tcod::print(*main_win, {0, 5}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    ss.str(std::string());
    context->present(*main_win);
}

void eat_drink_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &user){
    int ch;
    while(true){
        print_food(main_win, context, user);
        draw_stats(main_win, context, user);
        ch=SDL_getch(main_win, context);
        if(ch=='1'&&user.inv.food.bread>0){
            user.eat(&user.inv.food.bread);
        }
        else if(ch=='2'&&user.inv.food.waffle>0){
            user.eat(&user.inv.food.waffle);
        }
        else if(ch=='3'&&user.inv.food.energy_bar>0){
            user.eat(&user.inv.food.energy_bar);
        }
        else if(ch=='4'&&user.inv.water.water>0){
            user.drink(&user.inv.water.water);
        }
        else if(ch=='5'&&user.inv.water.sparkling_juice>0){
            user.drink(&user.inv.water.sparkling_juice);
        }
        else if(ch=='q'){
            return;
        }
    }
}

void drop_items_on_death(Player &user, Csr &csr_pos, Level &current){
    user=Player();
    csr_pos={1, 1};
    current={1, 1, 1};
}

void main_dungeon(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Csr &csr_pos, Player &user, Level &Current, std::vector<Monster> &monsters, Npc &npc, No_Delete &perm_config){
    std::vector<std::pair<int, int>> doors;
    if(monsters.empty()){
        generate_monsters(monsters, Current, csr_pos);
    }
    generate_doors(doors, Current);
    redraw_everything(main_win, context, csr_pos, user, Current, monsters);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==SDLK_LEFT||ch==SDLK_RIGHT||ch==SDLK_DOWN||ch==SDLK_UP){
            char_move(main_win, context, ch, csr_pos, monsters, user, npc, Current);
        }
        else if(ch=='z'){
            move_door(doors, monsters, Current, csr_pos);
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
        }
        else if(ch=='x'){
            std::pair<bool, bool> attack_status=attack_monster(main_win, context, monsters, csr_pos, user, Current);
            if(attack_status.first&&!attack_status.second){ // If dead return to main menu
                end_program(1);
                drop_items_on_death(user, csr_pos, Current);
                save_data(user, Current, csr_pos, monsters, npc, perm_config);
                return;
            }
            else if(attack_status.first&&attack_status.second){ // If win redraw dungeon and move on
                user.cur_shield=user.ori_shield;
                redraw_everything(main_win, context, csr_pos, user, Current, monsters);
            }
        }
        else if(ch=='c'){
            if(move_staircase(Current, csr_pos)){
                generate_doors(doors, Current);
            }
            else if(Current.lvl==1&&Current.x==1&&Current.y==1&&csr_pos.first==1&&csr_pos.second==48){
                bar_mode(main_win, context, user, npc, perm_config);
            }
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
        }
        else if(ch=='i'){
            if(!user.inv.item.empty()){
                inventory_mode(main_win, context, user, perm_config);
                redraw_everything(main_win, context, csr_pos, user, Current, monsters);
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Error: Inventory Empty");
            }
        }
        else if(ch=='e'){
            eat_drink_mode(main_win, context, user);
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
        }
        else if(ch=='.'){
            if(user.cur_hp<user.ori_hp){
                user.saturation-=1;
                user.cur_hp+=(user.ori_hp*0.015);
            }
            if(user.cur_hp>user.ori_hp){
                user.cur_hp=user.ori_hp;
            }
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
        }
        else if(ch=='S'){
            save_data(user, Current, csr_pos, monsters, npc, perm_config);
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
            clear_and_draw_dialog(main_win, context, "Data saved successfully!");
        }
        else if(ch=='H'){
            help_mode(main_win, context, "dungeon_mode");
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
        }
        else if(ch=='q'){
            if(SDL_getch_y_or_n(main_win, context, "Do you really wish to quit? [y] to quit, any other key to abort.")){
                end_program(0);
                save_data(user, Current, csr_pos, monsters, npc, perm_config);
                return;
            }
            redraw_everything(main_win, context, csr_pos, user, Current, monsters);
        }
        if(user.saturation<=0){
            drop_items_on_death(user, csr_pos, Current);
            save_data(user, Current, csr_pos, monsters, npc, perm_config);
            end_program(2);
            return;
        }
        if(user.hydration<=0){
            drop_items_on_death(user, csr_pos, Current);
            save_data(user, Current, csr_pos, monsters, npc, perm_config);
            end_program(3);
            return;
        }
    }
}


