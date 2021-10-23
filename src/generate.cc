#include "headers/generate.h"

Rarity generate_loot_rarity(Enemy_Type type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1, 1000);
    int loot=loot_type(generator);
    if(type==Enemy_Type::ENEMY){
        if(loot<=500){
            return Rarity::COMMON;
        }
        else if(loot<=950){
            return Rarity::UNCOMMON;
        }
        else if(loot<=1000){
            return Rarity::RARE;
        }
    }
    else if(type==Enemy_Type::ROOM_BOSS){
        if(loot<=500){
            return Rarity::UNCOMMON;
        }
        else if(loot<=800){
            return Rarity::RARE;
        }
        else if(loot<=1000){
            return Rarity::EPIC;
        }
    }
    else if(type==Enemy_Type::LEVEL_BOSS){
        if(loot<=400){
            return Rarity::RARE;
        }
        else if(loot<=950){
            return Rarity::EPIC;
        }
        else if(loot<=1000){
            return Rarity::LEGENDARY;
        }
    }
    else if(type==Enemy_Type::MINI_BOSS){
        if(loot<=200){
            return Rarity::EPIC;
        }
        else if(loot<=980){
            return Rarity::LEGENDARY;
        }
        else if(loot<=1000){
            return Rarity::ARTIFACT;
        }
    }
    else if(type==Enemy_Type::FINAL_BOSS){
        if(loot<=500){
            return Rarity::LEGENDARY;
        }
        else if(loot<=1000){
            return Rarity::ARTIFACT;
        }
    }
    return Rarity::NONE;
}

Type generate_loot_type(){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1, 100);
    int loot=loot_type(generator);
    if(loot<=20){
        return Type::CHESTPLATE; // chestplate
    }
    else if(loot<=40){
        return Type::HELMET; // helmet
    }
    else if(loot<=60){
        return Type::GREAVES; // greaves
    }
    else if(loot<=80){
        return Type::WEAPON; // weapon
    }
    else if(loot<=90){
        return Type::BOOTS; // boots
    }
    else if(loot<=100){
        return Type::SHIELD; // shield
    }
    return Type::NONE;
}

double rarity_value(Rarity rarity){ // increase the stats value for generated item
    if(rarity==Rarity::COMMON){
        return 1.0;
    }
    else if(rarity==Rarity::UNCOMMON){
        return 1.5;
    }
    else if(rarity==Rarity::RARE){
        return 2.0;
    }
    else if(rarity==Rarity::EPIC){
        return 3.0;
    }
    else if(rarity==Rarity::LEGENDARY){
        return 5.0;
    }
    else if(rarity==Rarity::ARTIFACT){
        return 10.0;
    }
    return 0;
}

double rarity_bonus(Rarity rarity){ // increase bonus stats chance
    if(rarity==Rarity::COMMON){
        return 1.0;
    }
    else if(rarity==Rarity::UNCOMMON){
        return 1.2;
    }
    else if(rarity==Rarity::RARE){
        return 1.5;
    }
    else if(rarity==Rarity::EPIC){
        return 2.0;
    }
    else if(rarity==Rarity::LEGENDARY){
        return 2.5;
    }
    else if(rarity==Rarity::ARTIFACT){
        return 4.0;
    }
    return 0;
}

Item generate_loot(Rarity loot_rarity){
    Type loot_type=generate_loot_type();
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    double durability=bonus_stats(generator);
    if(loot_type==Type::CHESTPLATE){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=3*rarity_bonus(loot_rarity)){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=2*rarity_bonus(loot_rarity)){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::HELMET){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(6*rarity_bonus(loot_rarity))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(3*rarity_bonus(loot_rarity))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::GREAVES){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::BOOTS){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::WEAPON){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::SHIELD){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            def=20*rarity_value(loot_rarity);
        }
    }
    return {"Placeholder", loot_type, loot_rarity, false, hp, attk, def, shield, crit_chance, crit_dmg, 0, 0, durability};
}

Item generate_loot_from_monster_type(Enemy_Type type){
    Rarity loot_rarity=generate_loot_rarity(type);
    return generate_loot(loot_rarity);
}

Item generate_loot_from_rarity_type(Rarity loot_rarity){
    return generate_loot(loot_rarity);
}

Item generate_trade_items(Rarity loot_rarity){
    Type loot_type=generate_loot_type();
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    if(loot_type==Type::CHESTPLATE){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=6*rarity_bonus(loot_rarity)){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=4*rarity_bonus(loot_rarity)){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::HELMET){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(6*rarity_bonus(loot_rarity))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::GREAVES){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::BOOTS){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(15*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::WEAPON){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::SHIELD){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(15*rarity_bonus(loot_rarity))){
            def=20*rarity_value(loot_rarity);
        }
    }
    // if(item.rarity==RARITY_ARTIFACT){ WIP: 10% chance to come with special skills }
    return {"Placeholder", loot_type, loot_rarity, false, hp, attk, def, shield, crit_chance, crit_dmg, 0, 0, 100.0};
}

Item craft_item(Rarity loot_rarity, Type loot_type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    if(loot_type==Type::CHESTPLATE){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=5*rarity_bonus(loot_rarity)){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::HELMET){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(6*rarity_bonus(loot_rarity))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(3*rarity_bonus(loot_rarity))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::GREAVES){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::BOOTS){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::WEAPON){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::SHIELD){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            def=20*rarity_value(loot_rarity);
        }
    }
    return {"Placeholder", loot_type, loot_rarity, false, hp, attk, def, shield, crit_chance, crit_dmg, 0, 0, 100.0};
}

void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item *item){
    Rarity loot_rarity=item->rarity;
    unsigned int ancient_cores_bonus=1;
    unsigned int crystallium_bonus=1;
    if(ancient_cores>0){
        ancient_cores_bonus=log(ancient_cores+1)/log(1.013);
    }
    if(crystallium>0){
        crystallium_bonus=log(crystallium+1)/log(1.02);
    }
    item->uses=0;
    item->enhancement=0;
    item->durability=100;
    Type loot_type=item->type;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    if(loot_type==Type::CHESTPLATE){ // chestplate
        item->original.hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_dmg=10*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::HELMET){ // helmet
        item->original.hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_chance=3*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=10*rarity_bonus(loot_rarity)){
            item->original.shield=20*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::GREAVES){ // greaves
        item->original.def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def+=20*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=5*rarity_bonus(loot_rarity)){
            item->original.shield=15*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::BOOTS){ // boots
        item->original.def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def+=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_chance=5*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::WEAPON){ // weapon
        item->original.attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_dmg=30*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_chance=2*rarity_value(loot_rarity);
        }
    }
    else if(loot_type==Type::SHIELD){ // shield
        item->original.shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=10*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def=20*rarity_value(loot_rarity);
        }
    }
    // if(item.rarity==RARITY_ARTIFACT){ WIP: 50/50 to get special skills }
}

Monster generate_room_monsters(std::vector<Monster> &enemy_data, Level current, Csr csr_pos){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> x_generator(1, 78);
    std::uniform_int_distribution<int> y_generator(1, 48);
    std::uniform_int_distribution<int> monster_type(1, 30);
    std::uniform_int_distribution<int> amount(15, 25);
    auto check_if_monster_already_exist = [](const std::vector<Monster> &enemy_data, int x, int y){
        if(!enemy_data.empty()){
            for(const auto &i:enemy_data){
                if(x==i.x&&y==i.y){
                    return true;
                }
            }
            return false;
        }
        return false;
    };
    for(int i=0; i<amount(generator); i++){
        Monster tmp_monster;
        tmp_monster.x=x_generator(generator);
        tmp_monster.y=y_generator(generator);
        if(check_if_monster_already_exist(enemy_data, tmp_monster.x, tmp_monster.y)){
            i--;
            continue;
        }
        if(tmp_monster.x==csr_pos.x&&tmp_monster.y==csr_pos.y){
            i--;
            continue;
        }
        if(current.x==1&&current.y==1&&current.lvl>1){
            if(tmp_monster.x==39&&tmp_monster.y==24){
                i--;
                continue;
            }
        }
        if(current.x==5&&current.y==5&&current.lvl<5){
            if(tmp_monster.x==39&&tmp_monster.y==24){
                i--;
                continue;
            }
        }
        if(current.lvl==1&&current.x==1&&current.y==1&&tmp_monster.x==1&&tmp_monster.y==48){
            i--;
            continue;
        }
        if(monster_type(generator)==30){
            tmp_monster.type=Enemy_Type::ROOM_BOSS;
        }
        else{
            tmp_monster.type=Enemy_Type::ENEMY;
        }
        // Generate monster stats
        monsters.push_back(tmp_monster);
    }
    if(current.x==5&&current.y==5){
        Monster tmp_monster;
        tmp_monster.x=39;
        tmp_monster.y=24;
        if(current.lvl<5){
            tmp_monster.type=Enemy_Type::LEVEL_BOSS;
        }
        if(current.lvl==5){
            tmp_monster.type=Enemy_Type::FINAL_BOSS;
        }
        // Generate monster stats
        monsters.push_back(tmp_monster);
    }
}

Monster_Stats create_monster(Level Current, Enemy_Type type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> diff_generator(-20, 20);
    Monster_Stats monster;
    double x=Current.x+Current.y;
    double lvl=Current.lvl;
    monster.hp=150*(lvl+(x/5.0))+diff_generator(generator);
    monster.attk=60*(lvl+(x/5.0))+diff_generator(generator);
    if(diff_generator(generator)>10){
        monster.def=5*(lvl+(x/5.0));
    }
    if(type==Enemy_Type::ENEMY){
        return monster;
    }
    else if(type==Enemy_Type::ROOM_BOSS){
        monster.hp*=10;
        monster.attk*=3;
        return monster;
    }
    else if(type==Enemy_Type::LEVEL_BOSS){
        monster.hp*=15;
        monster.attk*=4;
        return monster;
    }
    else if(type==Enemy_Type::FINAL_BOSS){
        monster.hp*=50;
        monster.attk*=7;
        return monster;
    }
    return monster;
}

bool return_chance(int chance){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> probability(1, 100);
    return (chance>=probability(generator));
}

int generate_random_number(int range_lo, int range_hi){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> numgenerator(range_lo, range_hi);
    return numgenerator(generator);
}

void generate_doors(std::vector<std::pair<int, int>> &doors, Level Current){
    if(Current.y>1){ // {x , y}
        doors.push_back({39, 49}); // bottom door
    }
    if(Current.y<5){
        doors.push_back({39, 0}); // top door
    }
    if(Current.x>1){
        doors.push_back({0, 24}); // left door
    }
    if(Current.x<5){
        doors.push_back({79, 24}); // right door
    }
}

int generate_gold(Enemy_Type type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> gold(0, 10);
    if(type==Enemy_Type::ENEMY){
        return (gold(generator));
    }
    else if(type==Enemy_Type::ROOM_BOSS){
        return (gold(generator)*gold(generator));
    }
    else if(type==Enemy_Type::LEVEL_BOSS){
        return (gold(generator)*30);
    }
    else if(type==Enemy_Type::FINAL_BOSS){
        return (gold(generator)*100);
    }
    return 0;
}
