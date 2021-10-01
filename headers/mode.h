#pragma once
#include <map>
#include "classes.h"
#include "main.h"
struct SortAscending{
    std::unordered_map<char,int> priority{
        {RARITY_ARTIFACT,0},
        {RARITY_LEGENDARY,1},
        {RARITY_EPIC,2},
        {RARITY_RARE,3},
        {RARITY_UNCOMMON,4},
        {RARITY_COMMON,5}
    };
    bool operator()(const Item *i, const Item *j){
        int priority_i=priority[i->rarity];
        int priority_j=priority[j->rarity];
        return priority_i<priority_j;
    }
    bool operator()(const std::pair<Item, bool> i, const std::pair<Item, bool> j){
        int priority_i=priority[i.first.rarity];
        int priority_j=priority[j.first.rarity];
        return priority_i<priority_j;
    }
}const sort_ascending;
struct SortDescending{
    std::unordered_map<char,int> priority{
        {RARITY_ARTIFACT,0},
        {RARITY_LEGENDARY,1},
        {RARITY_EPIC,2},
        {RARITY_RARE,3},
        {RARITY_UNCOMMON,4},
        {RARITY_COMMON,5}
    };
    bool operator()(const Item *i, const Item *j){
        int priority_i=priority[i->rarity];
        int priority_j=priority[j->rarity];
        return priority_i>priority_j;
    }
    bool operator()(const std::pair<Item, bool> i, const std::pair<Item, bool> j){
        int priority_i=priority[i.first.rarity];
        int priority_j=priority[j.first.rarity];
        return priority_i>priority_j;
    }
}const sort_descending;
static constexpr std::map<std::string, long long int> trader_prices{
    // Prices in Gold
    {"ancient_core",1000000},
    {"crystallium",150000},
    {"crystal_core",2500},
    {"common_material",150},
    {"uncommon_material",300},
    {"rare_material",600},
    {"epic_material",1000},
    {"legendary_material",10000},
    // Prices in Crystal Cores
    {"helmet_blueprint",3},
    {"chestplate_blueprint",4},
    {"greaves_blueprint",4},
    {"boots_blueprint",2},
    {"shield_blueprint",3},
    {"weapon_blueprint",5}
};
void draw_base(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, int y, unsigned int size, unsigned int page, bool is_blacksmith_mode, bool is_inventory_modifier_mode);
void print_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, bool is_selected);
void print_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line);
void inventory_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config);
void bar_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User,NPC &npc, NoDelete &perm_config);
void reforge_repair_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context,  Player &User, NoDelete &perm_config);
void eat_drink_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User);
bool decrease_amount(unsigned long long int &original, unsigned int amount);
bool remove_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount);
void add_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount);
void print_misc_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, unsigned int csr_pos);
void show_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User);
bool unequip_item(Player &User, Item *cur_item);
void help_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string mode);
void init_copy(std::vector<Item> &original, std::vector<Item*> &items_copy);
void process_copy(std::vector<Item> &original, std::vector<Item*> &items_copy, NoDelete &perm_config);
void inventory_storage(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config, Chest &chest);
void inventory_retrieve(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config, Chest &chest);
void inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config, std::vector<Item> original_copy, std::vector<Item*> items_copy);
void show_merchant_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &User);
void sell_items_to_merchant(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &User, NoDelete &perm_config);
void store_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, Chest &chest);
void retrieve_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, Chest &chest);
int calculate_price(const Item cur, int relationship, bool player_sell=false);
void trader_misc_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Merchant &gear_merchant);
