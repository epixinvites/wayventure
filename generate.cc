#include "headers/generate.h"
char generate_loot(char type){ // monster types: e, b, x, f
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> loot_type(1,1000);
    int loot = loot_type(generator);
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
        if(loot<985){
            return 'l';
        }
        if(loot>=985){
            return 'a';
        }
    }
    return '0';
}
void generate_monsters(std::vector<monster> &monsters, level Current, std::pair<int,int> csr_pos){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> x_generator(1,78);
    std::uniform_int_distribution<int> y_generator(1,48);
    std::uniform_int_distribution<int> monster_type(1,30);
    std::uniform_int_distribution<int> amount(9,18);
    for(int i = 0; i<amount(generator); i++){
        monster tmp_monster;
        tmp_monster.x = x_generator(generator);
        tmp_monster.y = y_generator(generator);
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
            tmp_monster.type = 'b';
        }
        else{
            tmp_monster.type = 'e';
        }
        monsters.push_back(tmp_monster);
    }
    if(Current.x==5&&Current.y==5){
        monster tmp_monster;
        tmp_monster.x=39;
        tmp_monster.y=24;
        if(Current.lvl<5){
            tmp_monster.type = 'x';
        }
        if(Current.lvl==5){
            tmp_monster.type = 'f';
        }
        monsters.push_back(tmp_monster);
    }
}
monster_stats create_monster(level Current, char type){
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> diff_generator(-20,20);
    monster_stats monster;
    double x = Current.x + Current.y;
    double lvl = Current.lvl;
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
    std::uniform_int_distribution<int> probability(1,100);
    return(chance>=probability(generator));
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

