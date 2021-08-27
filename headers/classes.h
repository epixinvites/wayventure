#pragma once
#include <string>
#include <vector>
#include <unordered_map>
struct monster{
    int x, y; // x cords and y cords
    char type; // 'e' for enemy, 'b' for boss
};
struct Csr{
    int first = 0, second = 0;
    template<class Archive>void serialize(Archive &archive){archive(first,second);}
};
struct monster_stats{ // base stats
    int hp=150, attk=60, def=0;
};
struct Item{
    struct Original{
        int hp = 0, attk = 0, def = 0, shield = 0, crit_chance = 0, crit_dmg = 0;
        template<class Archive>void serialize(Archive &archive){archive(hp,attk,def,shield,crit_chance,crit_dmg);}
    };
    Item() = default;
    Original original;
    std::string name;
    char type, rarity;
    // type: 'h' helmet, 'c' chestplate, 'g' greaves, 'b' boots, 's' shield, 'w' weapon
    // rarity: 'c' common, 'u' uncommon, 'r' rare, 'e' epic, 'l' legendary, 'a' artifact
    bool is_equipped;
    int hp = 0, attk = 0, def = 0, shield = 0, crit_chance = 0, crit_dmg = 0;
    unsigned int calibration = 0, uses = 0;
    Item(std::string name, char type, char rarity, bool is_equipped, int hp, int attk, int def, int shield, int crit_chance, int crit_dmg, unsigned int calibration, unsigned int uses);
    void initialize_item(); // MUST RUN BEFORE MODIFYING ANYTHING
    void reinitialize_item();
    void calculate_calibration();
    void recover_item();
    template<class Archive>void serialize(Archive &archive){archive(original,name,type,rarity,is_equipped,calibration,uses);}
};
struct Food{
	int bread=5; // 30 points of saturation
	int waffle=0; // 50 points of saturation
	int energy_bar=0; // Fill up entire saturation
	template<class Archive>void serialize(Archive &archive){archive(bread,waffle,energy_bar);}
};
struct Water{
	int water=3; // 50 points of hydration
	int sparkling_juice=0; // Fill up entire hydration
	template<class Archive>void serialize(Archive &archive){archive(water,sparkling_juice);}
};
struct Inventory{
    std::vector<Item> item;
    Food food;
    Water water;
    int heal_amount=20;
    template<class Archive>void serialize(Archive &archive){archive(food,water,heal_amount,item);} // fix vector
};
struct Equipped{
    Item *helmet=nullptr;
    Item *chestplate=nullptr;
    Item *greaves=nullptr;
    Item *boots=nullptr;
    Item *shield=nullptr;
    Item *weapon=nullptr;
};
class Player{ // Default HP: 500, Attk: 80, Def: 0, Shield: 0, Crit_chance: 0%, Crit_dmg: 0%
public:
    std::string name = "Wayfarrer";
    unsigned long long int steps = 0;
    int saturation = 100, hydration = 100;
    int ori_hp, attk, def, ori_shield, crit_chance, crit_dmg, gold;
    int cur_hp = 500, cur_shield = 0;
    Inventory inv;
    Equipped equip;
    Player(int ori_hp = 500, int attk = 80, int def = 0, int ori_shield = 0, int crit_chance = 0, int crit_dmg = 0, int gold = 0);
    void init();
    void eat(int *food);
    void drink(int *water);
    void initialize_gear(Item *gear);
    void uninitialize_gear(Item *gear);
    void initialize_stats();
    void uninitialize_stats();
    void add_item(Item input);
    void recover_original_stats();
    template<class Archive>void serialize(Archive &archive){archive(steps,saturation,hydration,gold,cur_hp,cur_shield,inv);}
};
struct level{
    int lvl, x, y;
    void reset(int sig){
        if(sig==0){
            x=1;
            y=1;
        }
        if(sig==1){
            x=5;
            y=5;
        }
    }
    level(int lvl, int x, int y):lvl{lvl},x{x},y{y}{};
    template<class Archive>void serialize(Archive &archive){archive(lvl,x,y);}
};

