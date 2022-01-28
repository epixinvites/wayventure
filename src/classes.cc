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

#include "headers/classes.h"
#include "headers/generate.h"
#include <cmath>
#include <iostream>

Item::Item(std::string name, Type type, Rarity rarity, bool is_equipped, int hp, int attk, int def, int shield, int crit_chance, int crit_dmg, unsigned int calibration, unsigned int uses, double durability):name{name}, type{type}, rarity{rarity}, is_equipped{is_equipped}, hp{hp}, attk{attk},
                                                                                                                                                                                                               def{def}, shield{shield}, crit_chance{crit_chance}, crit_dmg{crit_dmg},
                                                                                                                                                                                                               calibration{calibration}, uses{uses}, durability{durability}{
}

Player::Player(int ori_hp, int attk, int def, int ori_shield, int crit_chance, int crit_dmg, unsigned long long int gold):ori_hp{ori_hp}, attk{attk}, def{def}, ori_shield{ori_shield}, crit_chance{crit_chance}, crit_dmg{crit_dmg}, gold{gold}{
};

void Player::init(){
    for(int i=0; i<inv.item.size(); i++){ // loops through every single item and finds all items that is_equipped
        inv.item[i].reinitialize_item();
        if(inv.item[i].is_equipped){
            if(inv.item[i].type==Type::HELMET){
                equip.helmet=&inv.item[i];
            }
            if(inv.item[i].type==Type::CHESTPLATE){
                equip.chestplate=&inv.item[i];
            }
            if(inv.item[i].type==Type::GREAVES){
                equip.greaves=&inv.item[i];
            }
            if(inv.item[i].type==Type::BOOTS){
                equip.boots=&inv.item[i];
            }
            if(inv.item[i].type==Type::SHIELD){
                equip.shield=&inv.item[i];
            }
            if(inv.item[i].type==Type::WEAPON){
                equip.weapon=&inv.item[i];
            }
        }
    }
    initialize_stats();
}

void Player::initialize_gear(Item *gear){
    if(gear!=nullptr){
        ori_hp+=gear->hp;
        cur_hp+=gear->hp;
        ori_shield+=gear->shield;
        cur_shield+=gear->shield;
        attk+=gear->attk;
        def+=gear->def;
        crit_chance+=gear->crit_chance;
        crit_dmg+=gear->crit_dmg;
    }
}

void Player::uninitialize_gear(Item *&gear){
    if(gear!=nullptr){
        ori_hp-=gear->hp;
        cur_hp-=gear->hp;
        ori_shield-=gear->shield;
        cur_shield-=gear->shield;
        attk-=gear->attk;
        def-=gear->def;
        crit_chance-=gear->crit_chance;
        crit_dmg-=gear->crit_dmg;
    }
}

void Player::initialize_stats(){
    initialize_gear(equip.helmet);
    initialize_gear(equip.chestplate);
    initialize_gear(equip.greaves);
    initialize_gear(equip.boots);
    initialize_gear(equip.shield);
    initialize_gear(equip.weapon);
}

void Player::uninitialize_stats(){
    uninitialize_gear(equip.helmet);
    uninitialize_gear(equip.chestplate);
    uninitialize_gear(equip.greaves);
    uninitialize_gear(equip.boots);
    uninitialize_gear(equip.shield);
    uninitialize_gear(equip.weapon);
}

void Item::calculate_calibration(){
    int pow=2;
    for(int i=0;; i++){
        if(uses<pow){
            calibration=i;
            break;
        }
        pow*=2;
    }
}

double calculate_enhancement(unsigned int enhancement){
    if(enhancement>0){
        return (1+((log(enhancement+1)/log(1.1))/100.0));
    }
    else{
        return 1;
    }
}

void Item::reinitialize_item(){
    attk=original.attk;
    hp=original.hp;
    def=original.def;
    shield=original.shield;
    crit_chance=original.crit_chance;
    crit_dmg=original.crit_dmg;
    calculate_calibration();
    attk*=(1.0+(calibration/20.0));
    attk*=calculate_enhancement(enhancement);
    hp*=(1.0+(calibration/20.0));
    hp*=calculate_enhancement(enhancement);
    def*=(1.0+(calibration/20.0));
    def*=calculate_enhancement(enhancement);
    shield*=(1.0+(calibration/20.0));
    shield*=calculate_enhancement(enhancement);
    crit_chance*=(1.0+(calibration/20.0));
    crit_chance*=calculate_enhancement(enhancement);
    crit_dmg*=(1.0+(calibration/20.0));
    crit_dmg*=calculate_enhancement(enhancement);
}

void Item::initialize_item(){
    original.attk=attk;
    original.hp=hp;
    original.def=def;
    original.shield=shield;
    original.crit_chance=crit_chance;
    original.crit_dmg=crit_dmg;
}

Item *Inventory::get_pointer_to_item_with_id(unsigned long long int id){
    for(auto &i: item){
        if(i.id==id){
            return &i;
        }
    }
    return nullptr;
}

void Player::delete_item_with_id(unsigned long long int id){
    remove_item(inv.get_pointer_to_item_with_id(id));
}

void Player::add_item(Item input){
    input.id=get_largest_id(inv.item)+1;
    inv.item.push_back(input);
    for(int i=0; i<inv.item.size(); i++){ // loops through every single item and finds all items that is_equipped
        if(inv.item[i].is_equipped){
            if(inv.item[i].type==Type::HELMET){
                equip.helmet=&inv.item[i];
            }
            if(inv.item[i].type==Type::CHESTPLATE){
                equip.chestplate=&inv.item[i];
            }
            if(inv.item[i].type==Type::GREAVES){
                equip.greaves=&inv.item[i];
            }
            if(inv.item[i].type==Type::BOOTS){
                equip.boots=&inv.item[i];
            }
            if(inv.item[i].type==Type::SHIELD){
                equip.shield=&inv.item[i];
            }
            if(inv.item[i].type==Type::WEAPON){
                equip.weapon=&inv.item[i];
            }
        }
    }
    inv.item[inv.item.size()-1].initialize_item();
}

void Player::remove_item(Item *address){
    uninitialize_stats();
    int pos=0;
    for(; pos<inv.item.size(); pos++){
        if(&inv.item[pos]==address){
            break;
        }
    }
    inv.item.erase(inv.item.begin()+pos);
    equip=Equipped();
    for(int i=0; i<inv.item.size(); i++){ // loops through every single item and finds all items that is_equipped
        if(inv.item[i].is_equipped){
            if(inv.item[i].type==Type::HELMET){
                equip.helmet=&inv.item[i];
            }
            if(inv.item[i].type==Type::CHESTPLATE){
                equip.chestplate=&inv.item[i];
            }
            if(inv.item[i].type==Type::GREAVES){
                equip.greaves=&inv.item[i];
            }
            if(inv.item[i].type==Type::BOOTS){
                equip.boots=&inv.item[i];
            }
            if(inv.item[i].type==Type::SHIELD){
                equip.shield=&inv.item[i];
            }
            if(inv.item[i].type==Type::WEAPON){
                equip.weapon=&inv.item[i];
            }
        }
    }
    initialize_stats();
}

void Player::eat(int *food){
    if(saturation<100){
        if(food==&inv.food.bread&&*food>0){
            --*food;
            saturation+=30;
        }
        else if(food==&inv.food.waffle&&*food>0){
            --*food;
            saturation+=50;
        }
        else if(food==&inv.food.energy_bar&&*food>0){
            --*food;
            saturation=100;
        }
    }
    if(saturation>100){
        saturation=100;
    }
}

void Player::drink(int *water){
    if(hydration<100){
        if(water==&inv.water.water&&*water>0){
            --*water;
            hydration+=50;
        }
        else if(water==&inv.water.sparkling_juice&&*water>0){
            --*water;
            hydration=100;
        }
    }
    if(hydration>100){
        hydration=100;
    }
}

Time::Time(long total_seconds){
    hours=total_seconds/3600;
    minutes=(total_seconds%3600)/60;
    seconds=(total_seconds%3600)%60;
}

Time::Time(long hours, unsigned int minutes, unsigned int seconds):hours{hours}, minutes{minutes}, seconds{seconds}{}

bool Time::operator>(const Time &time){
    return (this->time_to_seconds()>time.time_to_seconds());
}

bool Time::operator>=(const Time &time){
    return (this->time_to_seconds()>=time.time_to_seconds());
}

long Time::time_to_seconds() const{
    return (hours*3600+minutes*60+seconds);
}

bool Miner::Job_Details::is_job_finished() const{
    Time time_passed{std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-job_start).count()};
    return time_passed>=total_job_duration;
}

bool Archaeologist::Job_Details::is_job_finished() const{
    Time time_passed{std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-job_start).count()};
    return time_passed>=total_job_duration;
}

std::ostream &operator<<(std::ostream &os, const Time &time){
    return os << time.hours << " hours " << time.minutes << " minutes " << time.seconds << " seconds";
}

Miscellaneous::Cores Miscellaneous::Cores::operator+=(const Cores &other){
    this->ancient_core+=other.ancient_core;
    this->mysterious_shard+=other.mysterious_shard;
    this->crystallium+=other.crystallium;
    this->crystal_core+=other.crystal_core;
    return *this;
}

Miscellaneous::Material_Rarity Miscellaneous::Material_Rarity::operator+=(const Material_Rarity &other){
    this->artifact+=other.artifact;
    this->legendary+=other.legendary;
    this->epic+=other.epic;
    this->rare+=other.rare;
    this->uncommon+=other.uncommon;
    this->common+=other.common;
    return *this;
}

bool Level::operator==(const Level &other) const{
    return (this->lvl==other.lvl&&this->x==other.x&&this->y==other.y);
}

bool Level::operator!=(const Level &other) const{
    return (this->lvl!=other.lvl||this->x!=other.x||this->y!=other.y);
}

bool Level::operator<=(const Level &other) const{
    return (this->lvl<=other.lvl&&this->x<=other.x&&this->y<=other.y);
}

bool Level::operator>=(const Level &other) const{
    return (this->lvl>=other.lvl&&this->x>=other.x&&this->y>=other.y);
}

void Dungeon::get_loot_in_room(const Level &current, std::vector<LootData> &loot_in_room){
    loot_in_room.clear();
    for(const auto &i:loot_data){
        if(i.dungeon_position==current){
            loot_in_room.push_back(i);
        }
    }
}

void Dungeon::init_rooms(){
    for(int l = 1; l<=5; l++){
        for(int x = 1; x<=5; x++){
            for(int y = 1; y<=5; y++){
                room_data.push_back(RoomData{{csr_pos}, {l, x, y}});
            }
        }
    }
}

RoomData::RoomData(const Csr csr_pos, const Level &id):id{id}{
    generate_room_monsters(enemy_data, id, csr_pos);
    generate_stairs(staircase_data, id);
    generate_doors(door_data, id);
    // Generate traps (WIP)
}

RoomData* Dungeon::get_pointer_of_room(const Level &current){
    for(auto &i:room_data){
        if(i.id==current){
            return &i;
        }
    }
    return nullptr;
}

void Thread_Flags::update_flag(std::atomic<bool> &flag, bool value){
    flag.store(value, std::memory_order_release);
}

bool Thread_Flags::get_flag(std::atomic<bool> &flag){
    return flag.load(std::memory_order_acquire);
}

void Screen_Char_Data::get_char_at(tcod::Console &main_win, int x, int y){
    char_code = TCOD_console_get_char(main_win.get(), x, y);
    foreground = TCOD_console_get_char_foreground(main_win.get(), x, y);
    background = TCOD_console_get_char_background(main_win.get(), x, y);
}

Csr Csr::convert_to_screen_pos(int input_x, int input_y){
    return {input_x, input_y+1};
}