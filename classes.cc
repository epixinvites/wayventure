#include "headers/classes.h"
Item::Item(std::string name, char type, char rarity, bool is_equipped, int hp, int attk, int def, int shield, int crit_chance, int crit_dmg, unsigned int calibration, unsigned int uses) : name{name}, type{type}, rarity{rarity}, is_equipped{is_equipped}, hp{hp}, attk{attk}, def{def}, shield{shield}, crit_chance{crit_chance}, crit_dmg{crit_dmg}, calibration{calibration}, uses{uses}{
}
Player::Player(int ori_hp, int attk, int def, int ori_shield, int crit_chance, int crit_dmg, int gold) : ori_hp{ori_hp}, attk{attk}, def{def}, ori_shield{ori_shield}, crit_chance{crit_chance}, crit_dmg{crit_dmg}, gold{gold}{
}
;
void Player::init(){
    for(int i=0; i<inv.item.size(); i++){ // loops through every single item and finds all items that is_equipped
        inv.item[i].initialize_item();
        if(inv.item[i].is_equipped){
            if(inv.item[i].type=='h'){
                equip.helmet=&inv.item[i];
            }
            if(inv.item[i].type=='c'){
                equip.chestplate=&inv.item[i];
            }
            if(inv.item[i].type=='g'){
                equip.greaves=&inv.item[i];
            }
            if(inv.item[i].type=='b'){
                equip.boots=&inv.item[i];
            }
            if(inv.item[i].type=='s'){
                equip.shield=&inv.item[i];
            }
            if(inv.item[i].type=='w'){
                equip.weapon=&inv.item[i];
            }
        }
    }
    initialize_stats();
    cur_hp=ori_hp;
    cur_shield=ori_shield;
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
void Player::uninitialize_gear(Item *gear){
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
void Item::reinitialize_item(){
    attk=original.attk;
    hp=original.hp;
    def=original.def;
    shield=original.shield;
    crit_chance=original.crit_chance;
    crit_dmg=original.crit_dmg;
    calculate_calibration();
    attk*=(1.0+(calibration/20.0));
    hp*=(1.0+(calibration/20.0));
    def*=(1.0+(calibration/20.0));
    shield*=(1.0+(calibration/20.0));
    crit_chance*=(1.0+(calibration/20.0));
    crit_dmg*=(1.0+(calibration/20.0));
}
void Item::initialize_item(){
    original.attk=attk;
    original.hp=hp;
    original.def=def;
    original.shield=shield;
    original.crit_chance=crit_chance;
    original.crit_dmg=crit_dmg;
    calculate_calibration();
    attk*=(1.0+(calibration/20.0));
    hp*=(1.0+(calibration/20.0));
    def*=(1.0+(calibration/20.0));
    shield*=(1.0+(calibration/20.0));
    crit_chance*=(1.0+(calibration/20.0));
    crit_dmg*=(1.0+(calibration/20.0));
}
void Player::add_item(Item input){
    inv.item.push_back(input);
    for(int i=0; i<inv.item.size(); i++){ // loops through every single item and finds all items that is_equipped
        if(inv.item[i].is_equipped){
            if(inv.item[i].type=='h'){
                equip.helmet=&inv.item[i];
            }
            if(inv.item[i].type=='c'){
                equip.chestplate=&inv.item[i];
            }
            if(inv.item[i].type=='g'){
                equip.greaves=&inv.item[i];
            }
            if(inv.item[i].type=='b'){
                equip.boots=&inv.item[i];
            }
            if(inv.item[i].type=='s'){
                equip.shield=&inv.item[i];
            }
            if(inv.item[i].type=='w'){
                equip.weapon=&inv.item[i];
            }
        }
    }
    inv.item[inv.item.size()-1].initialize_item();
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
