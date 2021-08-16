#include "headers/generate.h"
char generate_loot_rarity(char type){ // monster types: e, b, x, f
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1, 1000);
    int loot=loot_type(generator);
    if(type=='e'){
        if(loot<500){
            return 'c';
        }
        if(loot>=500&&loot<950){
            return 'u';
        }
        if(loot>=950){
            return 'r';
        }
    }
    else if(type=='b'){
        if(loot<500){
            return 'u';
        }
        if(loot>=500&&loot<800){
            return 'r';
        }
        if(loot>=800){
            return 'e';
        }
        if(loot>=999){
            return 'l';
        }
    }
    else if(type=='x'){
        if(loot<400){
            return 'r';
        }
        if(loot>=400&&loot<950){
            return 'e';
        }
        if(loot>=950){
            return 'l';
        }
    }
    else if(type=='f'){
        if(loot<995){
            return 'l';
        }
        if(loot>=995){
            return 'a';
        }
    }
    return '0';
}
char generate_loot_type(){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1, 100);
    int loot=loot_type(generator);
    if(loot<=100){
        return 'c'; // chestplate
    }
     else if(loot<=40){
     return 'h'; // helmet
     }
     else if(loot<=60){
     return 'g'; // greaves
     }
     else if(loot<=80){
     return 'w'; // weapon
     }
     else if(loot<=90){
     return 'b'; // boots
     }
     else if(loot<=100){
     return 's'; // shield
     }
    return '0';
}
double rarity_value(char rarity){
    if(rarity=='c'){
        return 1.0;
    }
    if(rarity=='u'){
        return 1.5;
    }
    if(rarity=='r'){
        return 2.0;
    }
    if(rarity=='e'){
        return 3.0;
    }
    if(rarity=='l'){
        return 5.0;
    }
    if(rarity=='a'){
        return 10.0;
    }
    return 0;
}
double rarity_bonus(char rarity){
    if(rarity=='c'){
        return 1.0;
    }
    if(rarity=='u'){
        return 1.0;
    }
    if(rarity=='r'){
        return 1.0;
    }
    if(rarity=='e'){
        return 2.0;
    }
    if(rarity=='l'){
        return 2.5;
    }
    if(rarity=='a'){
        return 4.0;
    }
    return 0;
}

Item generate_loot(char type){
    char loot_rarity=generate_loot_rarity(type);
    char loot_type=generate_loot_type();
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> deviation(-25, 25);
    std::uniform_int_distribution<int> bonus_stats(1, 100);
    std::string name;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    if(loot_type=='c'){ // chestplate
        hp=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=60){
            def=15*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=10){
            crit_dmg=2*rarity_value(loot_rarity);
        }
    }
    if(loot_type=='h'){ // helmet
        hp=(40+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(30*rarity_bonus(type))){
            def=5*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(10*rarity_bonus(type))){
            shield=20*rarity_value(loot_rarity);
        }
    }
    if(loot_type=='g'){ // greaves
        def=(50+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(30*rarity_bonus(type))){
            shield=15*rarity_value(loot_rarity);
        }
    }
    if(loot_type=='b'){ // boots
        def=(30+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(50*rarity_bonus(type))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type=='w'){ // weapon
        attk=(200+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(50*rarity_bonus(type))){
            crit_dmg=10*rarity_value(loot_rarity);
        }
        if(bonus_stats(generator)<=(30*rarity_bonus(type))){
            crit_chance=5*rarity_value(loot_rarity);
        }
    }
    if(loot_type=='s'){ // shield
        shield=(100+deviation(generator))*rarity_value(loot_rarity);
        if(bonus_stats(generator)<=(50*rarity_bonus(type))){
            def=20*rarity_value(loot_rarity);
        }
    }
    return{"Placeholder",loot_type,loot_rarity,false,hp,attk,def,shield,crit_chance,crit_dmg,0,0};
}
void generate_monsters(std::vector<monster> &monsters, level Current, std::pair<int,int> csr_pos){
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
        if(monster_type(generator)==30){
            tmp_monster.type='b';
        }
        else{
            tmp_monster.type='e';
        }
        monsters.push_back(tmp_monster);
    }
    if(Current.x==5&&Current.y==5){
        monster tmp_monster;
        tmp_monster.x=39;
        tmp_monster.y=24;
        if(Current.lvl<5){
            tmp_monster.type='x';
        }
        if(Current.lvl==5){
            tmp_monster.type='f';
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
    monster.hp=monster.hp*(lvl+(x/5.0))+diff_generator(generator);
    monster.attk=monster.attk*(lvl+(x/5.0))+diff_generator(generator);
    if(diff_generator(generator)>10){
        monster.def=5*(lvl+(x/5.0));
    }
    if(type=='e'){
        return monster;
    }
    else if(type=='b'){
        monster.hp*=10;
        monster.attk*=3;
        return monster;
    }
    else if(type=='x'){
        monster.hp*=15;
        monster.attk*=4;
        return monster;
    }
    else if(type=='f'){
        monster.hp*=30;
        monster.attk*=6;
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
    if(type=='e'){
        return (gold(generator));
    }
    if(type=='b'){
        return (gold(generator)*gold(generator));
    }
    if(type=='x'){
        return (gold(generator)*20);
    }
    if(type=='f'){
        return (gold(generator)*50);
    }
    return 0;
}
