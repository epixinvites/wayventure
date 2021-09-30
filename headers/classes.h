#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "main.h"
struct monster{
    int x, y; // x cords and y cords
    char type; // 'e' for enemy, 'b' for boss
    template<class Archive>void serialize(Archive &archive){archive(x,y,type);}
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
    std::string name; // Depreciated, change your username via `main.h` instead
    char type, rarity;
    // type: 'h' helmet, 'c' chestplate, 'g' greaves, 'b' boots, 's' shield, 'w' weapon
    // rarity: 'c' common, 'u' uncommon, 'r' rare, 'e' epic, 'l' legendary, 'a' artifact
    bool is_equipped;
    int hp = 0, attk = 0, def = 0, shield = 0, crit_chance = 0, crit_dmg = 0;
    unsigned int calibration = 0, uses = 0, enhancement = 0;
    double durability = 100.0;
    Item(std::string name, char type, char rarity, bool is_equipped, int hp, int attk, int def, int shield, int crit_chance, int crit_dmg, unsigned int calibration, unsigned int uses,double durability);
    void initialize_item();
    void reinitialize_item();
    void calculate_calibration();
    void recover_item();
    template<class Archive>void serialize(Archive &archive){archive(original,name,type,rarity,is_equipped,calibration,uses,durability,enhancement);}
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
struct Miscellaneous{
    struct Material_rarity{
        unsigned long long int common = 0;
        unsigned long long int uncommon = 0;
        unsigned long long int rare = 0;
        unsigned long long int epic = 0;
        unsigned long long int legendary = 0;
        unsigned long long int artifact = 0;
        template<class Archive> void serialize(Archive &archive){archive(common,uncommon,rare,epic,legendary,artifact);}
    };
    struct Material_type{
        unsigned long long int helmet = 0;
        unsigned long long int chestplate = 0;
        unsigned long long int greaves = 0;
        unsigned long long int boots = 0;
        unsigned long long int shield = 0;
        unsigned long long int weapon = 0;
        template<class Archive> void serialize(Archive &archive){archive(helmet,chestplate,greaves,boots,shield,weapon);}
    };
    unsigned long long int ancient_core = 0;
    unsigned long long int crystallium = 0;
    unsigned long long int crystal_cores = 0;
    unsigned long long int heal_amount=0;
    Material_rarity materials;
    Material_type blueprint;
    template<class Archive> void serialize(Archive &archive){archive(ancient_core,crystallium,materials,blueprint,crystal_cores,heal_amount);}
};
struct Inventory{
    std::vector<Item> item;
    Food food;
    Water water;
    Miscellaneous misc;
    template<class Archive>void serialize(Archive &archive){archive(food,water,item,misc);}
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
    int ori_hp, attk, def, ori_shield, crit_chance, crit_dmg;
    long long int gold = 0;
    int cur_hp = 500, cur_shield = 0;
    Inventory inv;
    Equipped equip;
    Player(int ori_hp = 500, int attk = 80, int def = 0, int ori_shield = 0, int crit_chance = 0, int crit_dmg = 0, long long int gold = 0);
    void init();
    void eat(int *food);
    void drink(int *water);
    void initialize_gear(Item *gear);
    void uninitialize_gear(Item *&gear);
    void initialize_stats();
    void uninitialize_stats();
    void add_item(Item input);
    void remove_item(Item *address);
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
struct Bartender{
    int relation = 50;
    template<class Archive>void serialize(Archive &archive){archive(relation);}
};
struct Farmer{
    int relation = 50;
    template<class Archive>void serialize(Archive &archive){archive(relation);}
};
struct Bank{
    // Bank Gold Storage
    long long int saved_gold = 0;
    double storage_interest = 1.02; // Compound Interest (Increases according to money saved)
    long long int storage_last_applied = 0; // Last time interest was applied
    template<class Archive>void serialize(Archive &archive){archive(saved_gold, storage_interest, storage_last_applied);}
};
struct Chest{
    std::vector<Item> gear_storage;
    Miscellaneous misc_storage;
    template<class Archive>void serialize(Archive &archive){archive(gear_storage, misc_storage);}
};
struct Merchant{
    int relation = 0;
    bool initial_refresh = true;
    std::vector<std::pair<Item, bool>>store; // bool is used to indicate whether to
    template<class Archive>void serialize(Archive &archive){archive(relation,initial_refresh,store);}
};
struct NPC{
    Bartender bartender;
    Farmer farmer;
    Bank bank;
    Chest chest;
    Merchant gear_merchant;
    Merchant mysterious_trader;
    template<class Archive>void serialize(Archive &archive){archive(bartender,farmer,bank,chest,gear_merchant,mysterious_trader);}
};
struct NoDelete{
    // Configurations
    // Stats? Maybe how many enemies killed, bosses killed and so on
    // Titles unlocked
    // Player EXP/Levels/System Control Authority
    char default_show_type_type = DEFAULT_SHOW_SELECTION;
    char default_show_rarity_type = DEFAULT_SHOW_SELECTION;
    char default_sort_rarity_method = DEFAULT_SHOW_SELECTION;
    bool show_current_item_compared_to_equipped = false;
    bool only_show_equipped = false;
    bool keep_changes_persistent = false;
    template<class Archive>void serialize(Archive &archive){
        archive(
            default_show_type_type,
            default_show_rarity_type,
            default_sort_rarity_method,
            show_current_item_compared_to_equipped,
            only_show_equipped,
            keep_changes_persistent
            );
    }
};


