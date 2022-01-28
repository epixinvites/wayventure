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

#include <vector>
#include <atomic>
#include <thread>
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include "headers/mode.h"
#include "headers/main.h"
#include "headers/threads.h"

struct Char_Move_Data{
    int status=0;
    Csr csr_pos={0, 0};
};

bool check_position_for_monsters(const std::vector<Monster> &monsters, int x, int y){
    for(const auto &i:monsters){
        if(x==i.x&&y==i.y){
            return true;
        }
    }
    return false;
}

bool check_surroundings_for_obstacles(const std::vector<LootData> &loot_in_room, int x, int y){
    for(const auto &i:loot_in_room){
        if(x==i.room_position.x&&y==i.room_position.y){
            return true;
        }
    }
    return false;
}

int check_monster_id(const std::vector<Monster> &monsters, int x, int y){
    for(const auto &i:monsters){
        if(x==i.x&&y==i.y){
            return i.id;
        }
    }
    return 0;
}

bool check_position_for_doors(const std::vector<DoorData> &door_data, int x, int y){
    for(const auto &i:door_data){
        if(x==i.x&&y==i.y){
            return true;
        }
    }
    return false;
}

bool check_position_for_staircase(const std::vector<StaircaseData> &staircase_data, int x, int y){
    for(const auto &i:staircase_data){
        if(x==i.x&&y==i.y){
            return true;
        }
    }
    return false;
}

Char_Move_Data char_move(tcod::Console &main_win, tcod::ContextPtr &context, int ch, Csr &csr_pos, RoomData *cur_room, Player &user, Npc &npc){
    Char_Move_Data data;
    bool require_move=false;
    if(ch=='a'||ch==SDLK_LEFT){
        if(check_position_for_monsters(cur_room->enemy_data, csr_pos.x-1, csr_pos.y)){
            data.status=1;
            data.csr_pos={csr_pos.x-1, csr_pos.y};
        }
        else if(check_position_for_doors(cur_room->door_data, csr_pos.x-1, csr_pos.y)){
            data.status=2;
            data.csr_pos={csr_pos.x-1, csr_pos.y};
        }
        else if(check_position_for_staircase(cur_room->staircase_data, csr_pos.x-1, csr_pos.y)){
            data.status=3;
            data.csr_pos={csr_pos.x-1, csr_pos.y};
        }
        else if(csr_pos.x>1&&!check_surroundings_for_obstacles(cur_room->loot_in_room, csr_pos.x-1, csr_pos.y)){
            TCOD_console_put_char_ex(main_win.get(), csr_pos.x, csr_pos.y+1, ' ', WHITE, BLACK);
            csr_pos.x--;
            require_move=true;
        }
    }
    else if(ch=='d'||ch==SDLK_RIGHT){ // csr_pos.x+1, csr_pos.y
        if(check_position_for_monsters(cur_room->enemy_data, csr_pos.x+1, csr_pos.y)){
            data.status=1;
            data.csr_pos={csr_pos.x+1, csr_pos.y};
        }
        else if(check_position_for_doors(cur_room->door_data, csr_pos.x+1, csr_pos.y)){
            data.status=2;
            data.csr_pos={csr_pos.x+1, csr_pos.y};
        }
        else if(check_position_for_staircase(cur_room->staircase_data, csr_pos.x+1, csr_pos.y)){
            data.status=3;
            data.csr_pos={csr_pos.x+1, csr_pos.y};
        }
        else if(csr_pos.x<78&&!check_surroundings_for_obstacles(cur_room->loot_in_room, csr_pos.x+1, csr_pos.y)){
            TCOD_console_put_char_ex(main_win.get(), csr_pos.x, csr_pos.y+1, ' ', WHITE, BLACK);
            csr_pos.x++;
            require_move=true;
        }
    }
    else if(ch=='w'||ch==SDLK_UP){ // csr_pos.x, csr_pos.y-1
        if(check_position_for_monsters(cur_room->enemy_data, csr_pos.x, csr_pos.y-1)){
            data.status=1;
            data.csr_pos={csr_pos.x, csr_pos.y-1};
        }
        else if(check_position_for_doors(cur_room->door_data, csr_pos.x, csr_pos.y-1)){
            data.status=2;
            data.csr_pos={csr_pos.x, csr_pos.y-1};
        }
        else if(check_position_for_staircase(cur_room->staircase_data, csr_pos.x, csr_pos.y-1)){
            data.status=3;
            data.csr_pos={csr_pos.x, csr_pos.y-1};
        }
        else if(csr_pos.y>1&&!check_surroundings_for_obstacles(cur_room->loot_in_room, csr_pos.x, csr_pos.y-1)){
            TCOD_console_put_char_ex(main_win.get(), csr_pos.x, csr_pos.y+1, ' ', WHITE, BLACK);
            csr_pos.y--;
            require_move=true;
        }
    }
    else if(ch=='s'||ch==SDLK_DOWN){ // csr_pos.x, csr_pos.y+1
        if(check_position_for_monsters(cur_room->enemy_data, csr_pos.x, csr_pos.y+1)){
            data.status=1;
            data.csr_pos={csr_pos.x, csr_pos.y+1};
        }
        else if(check_position_for_doors(cur_room->door_data, csr_pos.x, csr_pos.y+1)){
            data.status=2;
            data.csr_pos={csr_pos.x, csr_pos.y+1};
        }
        else if(check_position_for_staircase(cur_room->staircase_data, csr_pos.x, csr_pos.y+1)){
            data.status=3;
            data.csr_pos={csr_pos.x, csr_pos.y+1};
        }
        else if(csr_pos.y<48&&!check_surroundings_for_obstacles(cur_room->loot_in_room, csr_pos.x, csr_pos.y+1)){
            TCOD_console_put_char_ex(main_win.get(), csr_pos.x, csr_pos.y+1, ' ', WHITE, BLACK);
            csr_pos.y++;
            require_move=true;
        }
    }
    if(require_move){
        user.steps++;
        if(user.steps%35==0){
            user.hydration--;
        }
        if(user.steps%50==0){
            user.saturation--;
        }
        if(user.steps==npc.bank.interest_next_applied){
            npc.bank.interest_next_applied+=50000;
            npc.bank.saved_gold*=npc.bank.storage_interest;
        }
        draw_doors(main_win, context, cur_room->door_data, cur_room->staircase_data);
        draw_monster(main_win, context, cur_room->enemy_data);
        draw_player(main_win, context, csr_pos.x, csr_pos.y);
        draw_stats(main_win, context, user);
        context->present(main_win);
    }
    return data;
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

bool player_battle(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Monster *monster_data){
    std::pair<std::string, std::string> log;
    int ch;
    while(true){
        SDL_wclear_main_win(main_win, context);
        draw_border(main_win, context);
        std::stringstream user_output;
        std::stringstream enemy_output;
        draw_stats(main_win, context, user);
        user_output << username << "~ HP:" << user.cur_hp << " Defence:" << user.def << " Shield:" << user.cur_shield << " Heal:" << user.inv.misc.heal_amount;
        enemy_output << "Enemy~ HP:" << monster_data->stats.hp << " Attk:" << monster_data->stats.attk << " Def:" << monster_data->stats.def;
        tcod::print(main_win, {1, 49}, user_output.str(), WHITE, BLACK);
        tcod::print(main_win, {1, 2}, enemy_output.str(), WHITE, BLACK);
        tcod::print(main_win, {39, 25}, log.first, WHITE, BLACK, TCOD_CENTER, TCOD_BKGND_SET);
        tcod::print(main_win, {39, 26}, log.second, WHITE, BLACK, TCOD_CENTER, TCOD_BKGND_SET);
        context->present(main_win);
        ch=SDL_getch(main_win, context);
        if(ch=='1'){
            log=calculate_damage(user, monster_data->stats);
            if(user.cur_hp<=0){
                return false;
            }
            if(monster_data->stats.hp<=0){
                user.gold+=generate_gold(monster_data->type);
                Item loot=generate_loot_from_monster_type(monster_data->type);
                SDL_wclear_main_win(main_win, context);
                tcod::print(main_win, {0, 1}, "Press any key to keep and [r] to trash", WHITE, BLACK);
                draw_stats(main_win, context, user);
                print_description(main_win, context, &loot, 1);
                ch=SDL_getch(main_win, context);
                while(!(ch>0&&ch<128)){
                    ch=SDL_getch(main_win, context);
                }
                if(ch!='r'){
                    user.add_item(loot);
                }
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

std::pair<bool, bool> attack_monster(tcod::Console &main_win, tcod::ContextPtr &context, std::vector<Monster> &monsters, Csr csr_pos, Player &user){
    std::pair<bool, bool> attack_status{false, false}; // .first = if surroundings have monsters, .y is_alive
    unsigned int id = 0;
    if(check_position_for_monsters(monsters, csr_pos.x-1, csr_pos.y)){
        attack_status.first=true;
        id=check_monster_id(monsters, csr_pos.x-1, csr_pos.y);
        attack_status.second=player_battle(main_win, context, user, get_pointer_with_id(monsters, id));
        delete_item_with_id(monsters, id);
    }
    else if(check_position_for_monsters(monsters, csr_pos.x+1, csr_pos.y)){
        attack_status.first=true;
        id=check_monster_id(monsters, csr_pos.x+1, csr_pos.y);
        attack_status.second=player_battle(main_win, context, user, get_pointer_with_id(monsters, id));
        delete_item_with_id(monsters, id);
    }
    else if(check_position_for_monsters(monsters, csr_pos.x, csr_pos.y-1)){
        attack_status.first=true;
        id=check_monster_id(monsters, csr_pos.x, csr_pos.y-1);
        attack_status.second=player_battle(main_win, context, user, get_pointer_with_id(monsters, id));
        delete_item_with_id(monsters, id);
    }
    else if(check_position_for_monsters(monsters, csr_pos.x, csr_pos.y+1)){
        attack_status.first=true;
        id=check_monster_id(monsters, csr_pos.x, csr_pos.y+1);
        attack_status.second=player_battle(main_win, context, user, get_pointer_with_id(monsters, id));
        delete_item_with_id(monsters, id);
    }
    return attack_status;
}

void move_door(Dungeon &dungeon_data, const std::vector<DoorData> &door_data, int x, int y){
    for(int i=0; i<door_data.size(); i++){
        if(door_data[i].x==x&&door_data[i].y==y){
            switch(door_data[i].behaviour){
                case DoorData::Behaviour::UP:
                    dungeon_data.current.y++;
                    dungeon_data.csr_pos={39, 48};
                    break;
                case DoorData::Behaviour::DOWN:
                    dungeon_data.current.y--;
                    dungeon_data.csr_pos={39, 1};
                    break;
                case DoorData::Behaviour::LEFT:
                    dungeon_data.current.x--;
                    dungeon_data.csr_pos={78, 24};
                    break;
                case DoorData::Behaviour::RIGHT:
                    dungeon_data.current.x++;
                    dungeon_data.csr_pos={1, 24};
                    break;
                default:
                    break;
            }
        }
    }
}

void move_staircase(const std::vector<StaircaseData> &staircase_data, Level &level, Csr csr_pos){
    for(const auto &i:staircase_data){
        if(i.x==csr_pos.x&&i.y==csr_pos.y){
            if(i.behaviour==StaircaseData::Behaviour::UP){
                level.lvl--;
                level.x=DUNGEON_X_MAX, level.y=DUNGEON_Y_MAX;
            }
            else if(i.behaviour==StaircaseData::Behaviour::DOWN){
                level.lvl++;
                level.x=1, level.y=1;
            }
        }
    }
}

void print_food(tcod::Console &main_win, tcod::ContextPtr &context, Player &user){
    SDL_wclear_main_win(main_win, context);
    std::stringstream ss;
    ss << "[1] Bread (30 Saturation Points): " << user.inv.food.bread;
    tcod::print(main_win, {0, 1}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "[2] Waffle (50 Saturation Points): " << user.inv.food.waffle;
    tcod::print(main_win, {0, 2}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "[3] Energy Bar (100 Saturation Points): " << user.inv.food.energy_bar;
    tcod::print(main_win, {0, 3}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "[4] Water (50 Hydration Points): " << user.inv.water.water;
    tcod::print(main_win, {0, 4}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    ss << "[5] Sparkling Water (100 Hydration Points): " << user.inv.water.sparkling_juice;
    tcod::print(main_win, {0, 5}, ss.str(), WHITE, BLACK);
    ss.str(std::string());
    context->present(main_win);
}

void eat_drink_mode(tcod::Console &main_win, tcod::ContextPtr &context, Player &user){
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
    csr_pos=Csr();
    current=Level(1,1,1);
}

bool search_for_lootbox(tcod::Console &main_win, tcod::ContextPtr &context, Dungeon &dungeon_data, RoomData *cur_room, Player &User, Csr &csr_pos){
    for(const auto &i:dungeon_data.loot_data){
        if(i.dungeon_position==cur_room->id){
            if((i.room_position.x==csr_pos.x+1&&i.room_position.y==csr_pos.y)||(i.room_position.x==csr_pos.x-1&&i.room_position.y==csr_pos.y)||(i.room_position.x==csr_pos.x&&i.room_position.y==csr_pos.y+1)||(i.room_position.x==csr_pos.x&&i.room_position.y==csr_pos.y-1)){
                // Generate loot box
                // Generate food/water/gold/gear
                SDL_wclear_main_win(main_win, context);
                int line = 1;
                int bread_amount=generate_random_number(1,4); // Generate 1-3 pieces of bread
                int water_amount=generate_random_number(1,2); // Generate 1-3 pieces of water
                if(bread_amount==1){
                    tcod::print(main_win, {0, line}, "You obtained 1 piece of bread", WHITE, BLACK);
                }
                else{
                    tcod::print(main_win, {0, line}, "You obtained "+std::to_string(bread_amount)+" pieces of bread", WHITE, BLACK);
                }
                User.inv.food.bread += bread_amount;
                if(User.inv.food.bread>8){
                    User.inv.food.bread=8;
                }
                line++;
                if(water_amount==1){
                    tcod::print(main_win, {0, line}, "You obtained 1 bottle of water", WHITE, BLACK);
                }
                else{
                    tcod::print(main_win, {0, line}, "You obtained "+std::to_string(water_amount)+" bottles of water", WHITE, BLACK);
                }
                line++;
                User.inv.water.water += water_amount;
                if(User.inv.water.water>8){
                    User.inv.water.water=8;
                }
                if(return_chance(50)){ // Drop 5-15 gold
                    int gold = generate_random_number(5, 15);
                    tcod::print(main_win, {0, line}, "You obtained "+std::to_string(gold)+" gold pieces", WHITE, BLACK);
                    User.gold+=gold;
                    line++;
                }
                if(return_chance(20)){ // Drop gear
                    Item loot=generate_loot_from_monster_type(Enemy_Type::ROOM_BOSS);
                    print_description(main_win, context, &loot, line);
                    User.add_item(loot);
                }
                clear_and_draw_dialog(main_win, context, "Press any key to continue");
                int ch = SDL_getch(main_win, context);
                while(!(ch>0&&ch<128)){
                    ch = SDL_getch(main_win, context);
                }
                delete_item_with_id(dungeon_data.loot_data, i.id);
                dungeon_data.get_loot_in_room(cur_room->id, cur_room->loot_in_room);
                return true;
            }
        }
    }
    return false;
}

void main_dungeon(tcod::Console &main_win, tcod::ContextPtr &context, Dungeon &dungeon_data, Player &user, No_Delete &perm_config, Thread_Flags &thread_flags){
    RoomData *cur_room = dungeon_data.get_pointer_of_room(dungeon_data.current);
    dungeon_data.get_loot_in_room(cur_room->id, cur_room->loot_in_room);
    redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
    int ch;
    while(!thread_flags.get_flag(thread_flags.terminate)){
        if(thread_flags.get_flag(thread_flags.main_dungeon_require_update)){
            dungeon_data.get_loot_in_room(cur_room->id, cur_room->loot_in_room);
            thread_flags.update_flag(thread_flags.main_dungeon_require_update, false);
            redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
        }
        ch=SDL_getch(main_win, context);
        if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch==SDLK_LEFT||ch==SDLK_RIGHT||ch==SDLK_DOWN||ch==SDLK_UP){
            Char_Move_Data data = char_move(main_win, context, ch, dungeon_data.csr_pos, cur_room, user, dungeon_data.npc);
            if(data.status==1){
                std::pair<bool, bool> attack_status=attack_monster(main_win, context, cur_room->enemy_data, dungeon_data.csr_pos, user);
                if(attack_status.first){
                    if(attack_status.second){
                        user.cur_shield=user.ori_shield;
                        redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
                    }
                    else{
                        end_program(1);
                        drop_items_on_death(user, dungeon_data.csr_pos, dungeon_data.current);
                        thread_flags.update_flag(thread_flags.terminate, true);
                        return;
                    }
                }
            }
            else if(data.status==2){
                move_door(dungeon_data, cur_room->door_data, data.csr_pos.x, data.csr_pos.y);
                cur_room=dungeon_data.get_pointer_of_room(dungeon_data.current);
                dungeon_data.get_loot_in_room(cur_room->id, cur_room->loot_in_room);
                redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
            }
            else if(data.status==3){
                move_staircase(cur_room->staircase_data, dungeon_data.current, data.csr_pos);
                if(dungeon_data.current==Level{0, DUNGEON_X_MAX, DUNGEON_Y_MAX}){
                    dungeon_data.old_pos=dungeon_data.csr_pos;
                    dungeon_data.csr_pos={78, 2};
                }
                return;
            }
        }
        else if(ch=='x'){
            if(search_for_lootbox(main_win, context, dungeon_data, cur_room, user, dungeon_data.csr_pos)){
                redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
            }
        }
        else if(ch=='i'){
            if(!user.inv.item.empty()){
                inventory_mode(main_win, context, user, perm_config);
                redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Error: Inventory Empty");
            }
        }
        else if(ch=='e'){
            eat_drink_mode(main_win, context, user);
            redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
        }
        else if(ch=='.'){
            if(user.cur_hp<user.ori_hp){
                user.saturation-=1;
                user.cur_hp+=(user.ori_hp*0.03);
            }
            if(user.cur_hp>user.ori_hp){
                user.cur_hp=user.ori_hp;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
        }
        else if(ch=='S'){
            save_data(dungeon_data, user, perm_config);
            clear_and_draw_dialog(main_win, context, "Data saved successfully!");
        }
        else if(ch=='H'){
            help_mode(main_win, context, "dungeon_mode");
            redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
        }
        else if(ch=='q'){
            if(SDL_getch_y_or_n(main_win, context, "Do you really wish to quit? [y] to quit, any other key to abort.")){
                end_program(0);
                thread_flags.update_flag(thread_flags.terminate, true);
                return;
            }
            redraw_main_dungeon(main_win, context, dungeon_data.csr_pos, user, dungeon_data, cur_room);
        }
        if(user.saturation<=0){
            drop_items_on_death(user, dungeon_data.csr_pos, dungeon_data.current);
            end_program(2);
            thread_flags.update_flag(thread_flags.terminate, true);
            return;
        }
        if(user.hydration<=0){
            drop_items_on_death(user, dungeon_data.csr_pos, dungeon_data.current);
            end_program(3);
            thread_flags.update_flag(thread_flags.terminate, true);
            return;
        }
    }
}


