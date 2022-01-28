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

#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <atomic>
#include <libtcod.h>
#include <SDL2/SDL.h>

enum class Rarity{
    NONE, ALL, COMMON, UNCOMMON, RARE, EPIC, LEGENDARY, ARTIFACT,
};

enum class Type{
    NONE, ALL, HELMET, CHESTPLATE, GREAVES, BOOTS, SHIELD, WEAPON
};

enum class Enemy_Type{
    NONE, ENEMY, ROOM_BOSS, LEVEL_BOSS, MINI_BOSS, FINAL_BOSS
};

constexpr char DEFAULT_SHOW_SELECTION='!';
constexpr char SORT_TYPE_RARITY_ASCENDING='a';
constexpr char SORT_TYPE_RARITY_DESCENDING='d';

struct Time{
    long hours;
    unsigned int minutes;
    unsigned int seconds;

    template<class Archive>
    void serialize(Archive &archive){archive(hours, minutes, seconds);}

    Time(long total_seconds);

    Time(long hours, unsigned int minutes, unsigned int seconds);

    bool operator>(const Time &);

    bool operator>=(const Time &);

    long time_to_seconds() const;
};

struct Level{
    int lvl=1, x=1, y=1;

    Level()=default;

    Level(int lvl, int x, int y):lvl{lvl}, x{x}, y{y}{};

    bool operator==(const Level &other) const;

    bool operator!=(const Level &other) const;

    bool operator<=(const Level &other) const;

    bool operator>=(const Level &other) const;

    template<class Archive>
    void serialize(Archive &archive){archive(lvl, x, y);}
};

struct Monster_Stats{ // base stats
    int hp=150, attk=60, def=0;

    template<class Archive>
    void serialize(Archive &archive){archive(hp, attk, def);}
};

struct Monster{
    unsigned int id;
    int x, y; // x cords and y cords
    Enemy_Type type; // 'e' for enemy, 'b' for boss
    Monster_Stats stats;

    template<class Archive>
    void serialize(Archive &archive){archive(id, x, y, type, stats);}
};

struct Csr{
    int x=1, y=1; // csr_pos starts at {0, 1} until {49, 50}

    Csr convert_to_screen_pos(int input_x, int input_y);

    template<class Archive>
    void serialize(Archive &archive){archive(x, y);}
};

struct LootData{
    unsigned int id=0;
    Level dungeon_position={1,1,1};
    Csr room_position={1,1};

    template<class Archive>
    void serialize(Archive &archive){archive(id, dungeon_position, room_position);}
};

struct TrapData{
    int x=0, y=0;
    bool is_activated=false;
    int behaviour=0; // 0: Default

    template<class Archive>
    void serialize(Archive &archive){archive(x, y, is_activated, behaviour);}
};

struct StaircaseData{
    enum class Behaviour{
        NONE, DOWN, UP
    };
    unsigned int id=0;
    int x=0, y=0;
    Behaviour behaviour=Behaviour::NONE; // 0: None, 1: Down, 2: Up

    template<class Archive>
    void serialize(Archive &archive){archive(id, x, y, behaviour);}
};

struct DoorData{
    enum class Behaviour{
        NONE, UP, DOWN, LEFT, RIGHT
    };
    unsigned int id=0;
    int x=0, y=0;
    Behaviour behaviour=Behaviour::NONE; // 0: None, 1: Up, 2: Down, 3: Left, 4: Right

    template<class Archive>
    void serialize(Archive &archive){archive(id, x, y, behaviour);}
};

struct RoomData{
    Level id;
    std::vector<Monster> enemy_data;// Completed
    std::vector<TrapData> trap_data;
    std::vector<StaircaseData> staircase_data; // Completed
    std::vector<LootData> loot_in_room; // Completed
    std::vector<DoorData> door_data; // Completed

    RoomData()=default;

    RoomData(const Csr csr_pos, const Level &id);

    template<class Archive>
    void serialize(Archive &archive){archive(id, enemy_data, trap_data, staircase_data, loot_in_room, door_data);}
};

struct Item{
    struct Original{
        int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;

        template<class Archive>
        void serialize(Archive &archive){archive(hp, attk, def, shield, crit_chance, crit_dmg);}
    };

    Item() = default;

    Original original;
    unsigned long long int id=0;
    std::string name; // Depreciated, change your username via `main.h` instead
    Type type;
    Rarity rarity;
    // type: 'h' helmet, 'c' chestplate, 'g' greaves, 'b' boots, 's' shield, 'w' weapon
    // rarity: 'c' common, 'u' uncommon, 'r' rare, 'e' epic, 'l' legendary, 'a' artifact
    bool is_equipped;
    int hp=0, attk=0, def=0, shield=0, crit_chance=0, crit_dmg=0;
    unsigned int calibration=0, uses=0, enhancement=0;
    double durability=100.0;

    Item(std::string name, Type type, Rarity rarity, bool is_equipped, int hp, int attk, int def, int shield, int crit_chance, int crit_dmg, unsigned int calibration, unsigned int uses, double durability);

    void initialize_item();

    void reinitialize_item();

    void calculate_calibration();

    void recover_item();

    template<class Archive>
    void serialize(Archive &archive){archive(original, name, type, rarity, is_equipped, calibration, uses, durability, enhancement);}
};

struct Food{
    int bread=5; // 30 points of saturation
    int waffle=0; // 50 points of saturation
    int energy_bar=0; // Fill up entire saturation
    template<class Archive>
    void serialize(Archive &archive){archive(bread, waffle, energy_bar);}
};

struct Water{
    int water=3; // 50 points of hydration
    int sparkling_juice=0; // Fill up entire hydration
    template<class Archive>
    void serialize(Archive &archive){archive(water, sparkling_juice);}
};

struct Miscellaneous{
    struct Cores{
        unsigned long long int ancient_core=0;
        unsigned long long int crystallium=0;
        unsigned long long int crystal_core=0;
        unsigned long long int mysterious_shard=0;

        Cores operator+=(const Cores &other);

        template<class Archive>
        void serialize(Archive &archive){archive(ancient_core, crystallium, crystal_core, mysterious_shard);}
    };

    struct Material_Rarity{
        unsigned long long int common=0;
        unsigned long long int uncommon=0;
        unsigned long long int rare=0;
        unsigned long long int epic=0;
        unsigned long long int legendary=0;
        unsigned long long int artifact=0;

        Material_Rarity operator+=(const Material_Rarity &other);

        template<class Archive>
        void serialize(Archive &archive){archive(common, uncommon, rare, epic, legendary, artifact);}
    };

    struct Material_Type{
        unsigned long long int helmet=0;
        unsigned long long int chestplate=0;
        unsigned long long int greaves=0;
        unsigned long long int boots=0;
        unsigned long long int shield=0;
        unsigned long long int weapon=0;

        template<class Archive>
        void serialize(Archive &archive){archive(helmet, chestplate, greaves, boots, shield, weapon);}
    };

    unsigned long long int heal_amount=0;
    Material_Rarity materials;
    Material_Type blueprint;
    Cores cores;

    template<class Archive>
    void serialize(Archive &archive){archive(cores, materials, blueprint, heal_amount);}
};

struct Inventory{
    std::vector<Item> item;
    Food food;
    Water water;
    Miscellaneous misc;

    Item *get_pointer_to_item_with_id(unsigned long long int id);

    template<class Archive>
    void serialize(Archive &archive){archive(food, water, item, misc);}
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
    std::string name="Wayfarrer";
    unsigned long long int steps=0;
    int saturation=100, hydration=100;
    int ori_hp, attk, def, ori_shield, crit_chance, crit_dmg;
    unsigned long long int gold=0;
    int cur_hp=500, cur_shield=0;
    Inventory inv;
    Equipped equip;

    Player(int ori_hp=500, int attk=80, int def=0, int ori_shield=0, int crit_chance=0, int crit_dmg=0, unsigned long long int gold=0);

    void init();

    void eat(int *food);

    void drink(int *water);

    void initialize_gear(Item *gear);

    void uninitialize_gear(Item *&gear);

    void initialize_stats();

    void uninitialize_stats();

    void delete_item_with_id(unsigned long long int id);

    void add_item(Item input);

    void remove_item(Item *address);

    template<class Archive>
    void serialize(Archive &archive){archive(steps, saturation, hydration, gold, cur_hp, cur_shield, inv);}
};

struct Bartender{
    double relation=50;

    template<class Archive>
    void serialize(Archive &archive){archive(relation);}
};

struct Farmer{
    double relation=50;

    template<class Archive>
    void serialize(Archive &archive){archive(relation);}
};

struct Bank{
    // Bank Gold Storage
    unsigned long long int saved_gold=0;
    double storage_interest=1.02; // Compound Interest (Increases according to money saved)
    unsigned long long int interest_next_applied=0; // Last time interest was applied
    unsigned long long int initial_gold_store_time=0;
    template<class Archive>
    void serialize(Archive &archive){archive(saved_gold, storage_interest, interest_next_applied);}
};

struct Chest{
    std::vector<Item> gear_storage;
    Miscellaneous misc_storage;

    template<class Archive>
    void serialize(Archive &archive){archive(gear_storage, misc_storage);}
};

struct Merchant{
    double relation=0;
    bool initial_refresh=true;
    std::vector<std::pair<Item, bool>> store;

    template<class Archive>
    void serialize(Archive &archive){archive(relation, initial_refresh, store);}
};

struct Raw_Loot{
    long long int mysterious_piece=0;
    long long int mysterious_artifact=0;

    template<class Archive>
    void serialize(Archive &archive){archive(mysterious_piece, mysterious_artifact);}
};

struct Miner{
    struct Job_Details{
        Time original_job_duration={48, 0, 0};
        Time total_job_duration={0, 0, 0};
        bool has_active_job=false;
        std::chrono::time_point<std::chrono::steady_clock> job_start;
        int number_of_miners=0;
        double loot_multiplier=0;

        [[nodiscard]] bool is_job_finished() const;

        template<class Archive>
        void serialize(Archive &archive){archive(total_job_duration, has_active_job, job_start, number_of_miners, loot_multiplier);}
    };

    double relation=0;
    double skill_level=0;
    Raw_Loot loot;
    Job_Details job;

    template<class Archive>
    void serialize(Archive &archive){archive(relation, skill_level, loot, job);}
};

struct Archaeologist{
    struct Job_Details{
        Time decrypt_artifact_duration={2, 0, 0};
        Time decrypt_piece_duration={0, 20, 0};
        Time total_job_duration={0, 0, 0};
        Raw_Loot decryption_amount;
        bool has_active_job=false;
        std::chrono::time_point<std::chrono::steady_clock> job_start;

        [[nodiscard]] bool is_job_finished() const;

        template<class Archive>
        void serialize(Archive &archive){archive(total_job_duration, decryption_amount, has_active_job, job_start);}
    };

    double relation=0;
    double skill_level=0;
    Raw_Loot raw_storage;
    Miscellaneous loot_storage;
    Job_Details job;

    template<class Archive>
    void serialize(Archive &archive){archive(skill_level, loot_storage, raw_storage, job);}
};

struct Npc{
    Bartender bartender;
    Farmer farmer;
    Bank bank;
    Chest chest;
    Merchant gear_merchant;
    Merchant mysterious_trader;
    Miner miner;
    Archaeologist archaeologist;

    template<class Archive>
    void serialize(Archive &archive){archive(bartender, farmer, bank, chest, gear_merchant, mysterious_trader, miner, archaeologist);}
};

class Dungeon{
public:
    Csr csr_pos{1, 1};
    Csr old_pos{0, 0};
    Level current{1, 1, 1};
    Npc npc;
    std::vector<RoomData> room_data;
    std::vector<LootData> loot_data;

    Dungeon()=default;

    void init_rooms();

    void get_loot_in_room(const Level &current, std::vector<LootData> &loot_in_room);

    RoomData* get_pointer_of_room(const Level &current);

    template<class Archive>
    void serialize(Archive &archive){archive(csr_pos, old_pos, current, npc, room_data, loot_data);}
};

struct No_Delete{
    // Configurations
    // Stats? Maybe how many enemies killed, bosses killed and so on
    // Titles unlocked
    // Player EXP/Levels/System Control Authority
    Type default_show_type_type=Type::ALL;
    Rarity default_show_rarity_type=Rarity::ALL;
    char default_sort_rarity_method=DEFAULT_SHOW_SELECTION;
    bool show_current_item_compared_to_equipped=false;
    bool only_show_equipped=false;
    bool keep_changes_persistent=false;

    template<class Archive>
    void serialize(Archive &archive){
        archive(default_show_type_type, default_show_rarity_type, default_sort_rarity_method, show_current_item_compared_to_equipped, only_show_equipped, keep_changes_persistent);
    }
};

class Thread_Flags{
public:
    std::atomic<bool> terminate = false;
    std::atomic<bool> main_dungeon_require_update = false;

    void update_flag(std::atomic<bool> &flag, bool value);

    bool get_flag(std::atomic<bool> &flag);
};

class Screen_Char_Data{
private:
    int char_code;
    TCOD_ColorRGB foreground;
    TCOD_ColorRGB background;

public:
    void get_char_at(tcod::Console &main_win, int x, int y);
};

std::ostream &operator<<(std::ostream &os, const Time &time);

template <typename T> unsigned long long int get_largest_id(const std::vector<T> &data){
    unsigned long long int id=0;
    for(const auto &i:data){
        if(i.id>id){
            id=i.id;
        }
    }
    return id;
}

template <typename T> bool delete_item_with_id(std::vector<T> &data, unsigned long long int id){
    for(auto it = data.begin(); it != data.end(); it++){
        if(it->id==id){
            data.erase(it);
            return true;
        }
    }
    return false;
}

template <typename T> T* get_pointer_with_id(std::vector<T> &data, unsigned long long int id){
    for(auto &i:data){
        if(i.id==id){
            return &i;
        }
    }
    return nullptr;
}