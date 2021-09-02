#pragma once
#include <vector>
#include "classes.h"
struct Bartender{
    int relation = 50;
};
struct Farmer{
    int relation = 50;
};
struct Bank{
    unsigned int saved_gold = 0;
    unsigned int total_gold = 0;
    double interest = 0.01;
    unsigned long long last_applied = 0;
};
struct Chest{
    std::vector<Item> storage;
};
struct Merchant{
    int relation = 50;
    unsigned long long last_refresh = 0;
    std::vector<Item> store;
};
struct NPC{
    Bartender bartender;
    Farmer farmer;
    Bank bank;
    Chest chest;
    Merchant gear_merchant;
    Merchant mysterious_trader;
};
