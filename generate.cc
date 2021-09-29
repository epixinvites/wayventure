#include "headers/generate.h"
char generate_loot_rarity(char type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1, 1000);
    int loot=loot_type(generator);
    if(type==DUNGEON_ENEMY){
        if(loot<500){
            return RARITY_COMMON;
        }
        if(loot>=500&&loot<950){
            return RARITY_UNCOMMON;
        }
        if(loot>=950){
            return RARITY_RARE;
        }
    }
    else if(type==DUNGEON_ROOM_BOSS){
        if(loot<500){
            return RARITY_UNCOMMON;
        }
        if(loot>=500&&loot<800){
            return RARITY_RARE;
        }
        if(loot>=800){
            return RARITY_EPIC;
        }
        if(loot>=999){
            return RARITY_LEGENDARY;
        }
    }
    else if(type==DUNGEON_LEVEL_BOSS){
        if(loot<400){
            return RARITY_RARE;
        }
        if(loot>=400&&loot<950){
            return RARITY_EPIC;
        }
        if(loot>=950){
            return RARITY_LEGENDARY;
        }
    }
    else if(type==DUNGEON_FINAL_BOSS){
        if(loot<990){
            return RARITY_LEGENDARY;
        }
        if(loot>=990){
            return RARITY_ARTIFACT;
        }
    }
    return '0';
}
char generate_loot_type(){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1, 100);
    int loot=loot_type(generator);
    if(loot<=20){
        return TYPE_CHESTPLATE; // chestplate
    }
    else if(loot<=40){
        return TYPE_HELMET; // helmet
    }
    else if(loot<=60){
        return TYPE_GREAVES; // greaves
    }
    else if(loot<=80){
        return TYPE_WEAPON; // weapon
    }
    else if(loot<=90){
        return TYPE_BOOTS; // boots
    }
    else if(loot<=100){
        return TYPE_SHIELD; // shield
    }
    return '0';
}
double rarity_value(char rarity){ // increase the stats value for generated item
    if(rarity==RARITY_COMMON){
        return 1.0;
    }
    if(rarity==RARITY_UNCOMMON){
        return 1.5;
    }
    if(rarity==RARITY_RARE){
        return 2.0;
    }
    if(rarity==RARITY_EPIC){
        return 3.0;
    }
    if(rarity==RARITY_LEGENDARY){
        return 5.0;
    }
    if(rarity==RARITY_ARTIFACT){
        return 10.0;
    }
    return 0;
}
double rarity_bonus(char rarity){ // increase bonus stats chance
    if(rarity==RARITY_COMMON){
        return 1.0;
    }
    if(rarity==RARITY_UNCOMMON){
        return 1.0;
    }
    if(rarity==RARITY_RARE){
        return 1.0;
    }
    if(rarity==RARITY_EPIC){
        return 2.0;
    }
    if(rarity==RARITY_LEGENDARY){
        return 2.5;
    }
    if(rarity==RARITY_ARTIFACT){
        return 4.0;
    }
    return 0;
}
Item generate_loot(char loot_rarity){
    char loot_type=generate_loot_type();
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    double durability = bonus_stats(generator);
    if(loot_type==TYPE_CHESTPLATE){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=3*rarity_bonus(loot_rarity)){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=2*rarity_bonus(loot_rarity)){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_HELMET){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(6*rarity_bonus(loot_rarity))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(3*rarity_bonus(loot_rarity))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_GREAVES){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_BOOTS){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_WEAPON){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_SHIELD){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            def=20*rarity_value(loot_rarity);
        }
    }
    return{"Placeholder",loot_type,loot_rarity,false,hp,attk,def,shield,crit_chance,crit_dmg,0,0,durability};
}
Item generate_loot_from_monster_type(char type){
    char loot_rarity=generate_loot_rarity(type);
    return generate_loot(loot_rarity);
}
Item generate_loot_from_rarity_type(char loot_rarity){
    return generate_loot(loot_rarity);
}
Item generate_trade_items(char loot_rarity){
    char loot_type=generate_loot_type();
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    if(loot_type==TYPE_CHESTPLATE){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=6*rarity_bonus(loot_rarity)){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=4*rarity_bonus(loot_rarity)){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_HELMET){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(6*rarity_bonus(loot_rarity))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_GREAVES){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_BOOTS){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(15*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_WEAPON){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_SHIELD){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(15*rarity_bonus(loot_rarity))){
            def=20*rarity_value(loot_rarity);
        }
    }
    // if(item.rarity==RARITY_ARTIFACT){ WIP: 10% chance to come with special skills }
    return{"Placeholder",loot_type,loot_rarity,false,hp,attk,def,shield,crit_chance,crit_dmg,0,0,100.0};
}
Item craft_item(char loot_rarity, char loot_type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    if(loot_type==TYPE_CHESTPLATE){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=5*rarity_bonus(loot_rarity)){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_HELMET){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(6*rarity_bonus(loot_rarity))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(3*rarity_bonus(loot_rarity))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_GREAVES){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_BOOTS){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_WEAPON){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(5*rarity_bonus(loot_rarity))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_SHIELD){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(10*rarity_bonus(loot_rarity))){
            def=20*rarity_value(loot_rarity);
        }
    }
    return{"Placeholder",loot_type,loot_rarity,false,hp,attk,def,shield,crit_chance,crit_dmg,0,0,100.0};
}
void reforge_item(unsigned int ancient_cores, unsigned int crystallium, Item *item){
    char loot_rarity = item->rarity;
    unsigned int ancient_cores_bonus = 1;
    unsigned int crystallium_bonus = 1;
    if(ancient_cores>0){
        ancient_cores_bonus = log(ancient_cores+1)/log(1.013);
    }
    if(crystallium>0){
        crystallium_bonus = log(crystallium+1)/log(1.02);
    }
    item->uses=0;
    item->enhancement=0;
    item->durability=100;
    char loot_type=item->type;
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    if(loot_type==TYPE_CHESTPLATE){ // chestplate
        item->original.hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_dmg=10*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_HELMET){ // helmet
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
    if(loot_type==TYPE_GREAVES){ // greaves
        item->original.def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def+=20*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=5*rarity_bonus(loot_rarity)){
            item->original.shield=15*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_BOOTS){ // boots
        item->original.def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def+=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_WEAPON){ // weapon
        item->original.attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_dmg=30*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=1*rarity_bonus(loot_rarity)*crystallium_bonus){
            item->original.crit_chance=2*rarity_value(loot_rarity);
        }
    }
    if(loot_type==TYPE_SHIELD){ // shield
        item->original.shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=10*rarity_bonus(loot_rarity)*ancient_cores_bonus){
            item->original.def=20*rarity_value(loot_rarity);
        }
    }
    // if(item.rarity==RARITY_ARTIFACT){ WIP: 50/50 to get special skills }
}
void generate_monsters(std::vector<monster> &monsters, level Current, Csr csr_pos){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> x_generator(1, 78);
    std::uniform_int_distribution<int> y_generator(1, 48);
    std::uniform_int_distribution<int> monster_type(1, 30);
    std::uniform_int_distribution<int> amount(9, 18);
    for(int i=0; i<amount(generator); i++){
        monster tmp_monster;
        tmp_monster.x=x_generator(generator);
        tmp_monster.y=y_generator(generator);
        if(Current.x==1&&Current.y==1&&Current.lvl>1){
            if(tmp_monster.x==39&&tmp_monster.y==24){
                i--;
                continue;
            }
        }
        if(Current.x==5&&Current.y==5&&Current.lvl<5){
            if(tmp_monster.x==39&&tmp_monster.y==24){
                i--;
                continue;
            }
        }
        if(tmp_monster.x==csr_pos.first&&tmp_monster.y==csr_pos.second){
            i--;
            continue;
        }
        if(Current.lvl==1&&Current.x==1&&Current.y==1&&tmp_monster.x==1&&tmp_monster.y==48){
            i--;
            continue;
        }
        if(monster_type(generator)==30){
            tmp_monster.type=DUNGEON_ROOM_BOSS;
        }
        else{
            tmp_monster.type=DUNGEON_ENEMY;
        }
        monsters.push_back(tmp_monster);
    }
    if(Current.x==5&&Current.y==5){
        monster tmp_monster;
        tmp_monster.x=39;
        tmp_monster.y=24;
        if(Current.lvl<5){
            tmp_monster.type=DUNGEON_LEVEL_BOSS;
        }
        if(Current.lvl==5){
            tmp_monster.type=DUNGEON_FINAL_BOSS;
        }
        monsters.push_back(tmp_monster);
    }
}
monster_stats create_monster(level Current, char type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> diff_generator(-20, 20);
    monster_stats monster;
    double x=Current.x+Current.y;
    double lvl=Current.lvl;
    monster.hp=150*(lvl+(x/5.0))+diff_generator(generator);
    monster.attk=60*(lvl+(x/5.0))+diff_generator(generator);
    if(diff_generator(generator)>10){
        monster.def=5*(lvl+(x/5.0));
    }
    if(type==DUNGEON_ENEMY){
        return monster;
    }
    else if(type==DUNGEON_ROOM_BOSS){
        monster.hp*=10;
        monster.attk*=3;
        return monster;
    }
    else if(type==DUNGEON_LEVEL_BOSS){
        monster.hp*=15;
        monster.attk*=4;
        return monster;
    }
    else if(type==DUNGEON_FINAL_BOSS){
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
void generate_doors(std::vector<std::pair<int,int>> &doors, level Current){
    if(Current.y>1){ // {x , y}
        doors.push_back({39,49}); // bottom door
    }
    if(Current.y<5){
        doors.push_back({39,0}); // top door
    }
    if(Current.x>1){
        doors.push_back({0,24}); // left door
    }
    if(Current.x<5){
        doors.push_back({79,24}); // right door
    }
}
int generate_gold(char type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> gold(0, 10);
    if(type==DUNGEON_ENEMY){
        return (gold(generator));
    }
    if(type==DUNGEON_ROOM_BOSS){
        return (gold(generator)*gold(generator));
    }
    if(type==DUNGEON_LEVEL_BOSS){
        return (gold(generator)*30);
    }
    if(type==DUNGEON_FINAL_BOSS){
        return (gold(generator)*100);
    }
    return 0;
}
