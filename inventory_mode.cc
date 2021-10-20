#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include "headers/classes.h"
#include "headers/mode.h"
#include "headers/draw.h"
#include "headers/generate.h"

void draw_base(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, int y, unsigned int size, unsigned int page, bool is_blacksmith_mode, bool is_inventory_modifier_mode){
    std::stringstream ss;
    SDL_wclear_dialog_bar(mainWin, context);
    if(size==0){
        ss << "[System] Inventory Empty";
    }
    else if(is_blacksmith_mode){
        ss << "[Blacksmith] How can I help you today? " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")";
    }
    else if(is_inventory_modifier_mode){
        ss << "Inventory " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")" << " [Inventory Modifier Mode]";
    }
    else{
        ss << "Inventory " << "(" << page+1 << "/" << ((size-1)/30)+1 << ")";
    }
    tcod::print(*mainWin, {0, 0}, ss.str(), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    for(int i=0; i<80; i++){
        TCOD_console_put_char_ex(mainWin.get(), i, y+1, '-', WHITE, BLACK);
    }
//    context->present(*mainWin);
}

void print_bold_with_condition(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, const std::string &output, const TCOD_ColorRGB &foreground, int line, bool is_bold){
    if(is_bold){
        tcod::print(*mainWin, {0, line}, output, &BLACK, &foreground, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, line}, output, &foreground, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
}

void print_item(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, const Item *cur_item, int line, bool is_selected){
    std::stringstream ss;
    if(cur_item->is_equipped){
        ss << "*";
    }
    switch(cur_item->type){
        case Type::HELMET:
            ss << "[Helmet] ";
            break;
        case Type::CHESTPLATE:
            ss << "[Chestplate] ";
            break;
        case Type::GREAVES:
            ss << "[Greaves] ";
            break;
        case Type::BOOTS:
            ss << "[Boots] ";
            break;
        case Type::SHIELD:
            ss << "[Shield] ";
            break;
        case Type::WEAPON:
            ss << "[Weapon] ";
            break;
        default:
            tcod::print(*mainWin, {0, line+1}, "Error", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    ss << cur_item->name;
    if(cur_item->calibration>0){
        ss << " [" << cur_item->calibration << "]";
    }
    switch(cur_item->rarity){
        case Rarity::COMMON:
            print_bold_with_condition(mainWin, context, ss.str(), WHITE, line+1, is_selected);
            break;
        case Rarity::UNCOMMON:
            print_bold_with_condition(mainWin, context, ss.str(), GREEN, line+1, is_selected);
            break;
        case Rarity::RARE:
            print_bold_with_condition(mainWin, context, ss.str(), BLUE, line+1, is_selected);
            break;
        case Rarity::EPIC:
            print_bold_with_condition(mainWin, context, ss.str(), PURPLE, line+1, is_selected);
            break;
        case Rarity::LEGENDARY:
            print_bold_with_condition(mainWin, context, ss.str(), YELLOW, line+1, is_selected);
            break;
        case Rarity::ARTIFACT:
            print_bold_with_condition(mainWin, context, ss.str(), DARK_RED, line+1, is_selected);
            break;
        default:
            break;
    }
}

void print_description(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, const Item *cur_item, int line){
    tcod::print(*mainWin, {0, line+1}, "Name: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*mainWin, {6, line+1}, cur_item->name, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*mainWin, {0, line+2}, "Rarity: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->rarity){
        case Rarity::COMMON:
            tcod::print(*mainWin, {8, line+2}, "Common", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Rarity::UNCOMMON:
            tcod::print(*mainWin, {8, line+2}, "Uncommon", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Rarity::RARE:
            tcod::print(*mainWin, {8, line+2}, "Rare", &BLUE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Rarity::EPIC:
            tcod::print(*mainWin, {8, line+2}, "Epic", &PURPLE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Rarity::LEGENDARY:
            tcod::print(*mainWin, {8, line+2}, "Legendary", &YELLOW, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Rarity::ARTIFACT:
            tcod::print(*mainWin, {8, line+2}, "Artifact", &DARK_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*mainWin, {8, line+2}, "-ERROR-", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
    }
    line+=2;
    tcod::print(*mainWin, {0, line+1}, "Type: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->type){
        case Type::HELMET:
            tcod::print(*mainWin, {6, line+1}, "Helmet", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Type::CHESTPLATE:
            tcod::print(*mainWin, {6, line+1}, "Chestplate", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Type::GREAVES:
            tcod::print(*mainWin, {6, line+1}, "Greaves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Type::BOOTS:
            tcod::print(*mainWin, {6, line+1}, "Boots", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Type::SHIELD:
            tcod::print(*mainWin, {6, line+1}, "Shield", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case Type::WEAPON:
            tcod::print(*mainWin, {6, line+1}, "Weapon", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*mainWin, {6, line+1}, "-ERROR-", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    line++;
    tcod::print(*mainWin, {0, line+1}, "Equipped: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(cur_item->is_equipped){
        tcod::print(*mainWin, {10, line+1}, "true", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(!cur_item->is_equipped){
        tcod::print(*mainWin, {10, line+1}, "false", &LIGHT_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    line++;
    std::stringstream ss;
    tcod::print(*mainWin, {0, line+1}, ("Durability: "+std::to_string(cur_item->durability)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    if(cur_item->uses>0){
        tcod::print(*mainWin, {0, line+1}, ("Uses: "+std::to_string(cur_item->uses)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->calibration>0){
        tcod::print(*mainWin, {0, line+1}, ("Calibration Level: "+std::to_string(cur_item->calibration)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->enhancement>0){
        tcod::print(*mainWin, {0, line+1}, ("Enhancement: "+std::to_string(cur_item->enhancement)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->hp>0){
        tcod::print(*mainWin, {0, line+1}, ("Health: "+std::to_string(cur_item->hp)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->attk>0){
        tcod::print(*mainWin, {0, line+1}, ("Damage: "+std::to_string(cur_item->attk)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->def>0){
        tcod::print(*mainWin, {0, line+1}, ("Defence: "+std::to_string(cur_item->def)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->shield>0){
        tcod::print(*mainWin, {0, line+1}, ("Shield: "+std::to_string(cur_item->shield)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_chance>0){
        tcod::print(*mainWin, {0, line+1}, ("Critical Chance: "+std::to_string(cur_item->crit_chance)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_dmg>0){
        tcod::print(*mainWin, {0, line+1}, ("Critical Damage: "+std::to_string(cur_item->crit_dmg)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    context->present(*mainWin);
}

void equip_item(Player &User, Item *cur_item){
    cur_item->is_equipped=true;
    if(cur_item->type==Type::HELMET){
        if(User.equip.helmet!=nullptr){
            User.uninitialize_gear(User.equip.helmet);
            User.equip.helmet->is_equipped=false;
        }
        User.equip.helmet=cur_item;
        User.initialize_gear(User.equip.helmet);
    }
    if(cur_item->type==Type::CHESTPLATE){
        if(User.equip.chestplate!=nullptr){
            User.uninitialize_gear(User.equip.chestplate);
            User.equip.chestplate->is_equipped=false;
        }
        User.equip.chestplate=cur_item;
        User.initialize_gear(User.equip.chestplate);
    }
    if(cur_item->type==Type::GREAVES){
        if(User.equip.greaves!=nullptr){
            User.uninitialize_gear(User.equip.greaves);
            User.equip.greaves->is_equipped=false;
        }
        User.equip.greaves=cur_item;
        User.initialize_gear(User.equip.greaves);
    }
    if(cur_item->type==Type::BOOTS){
        if(User.equip.boots!=nullptr){
            User.uninitialize_gear(User.equip.boots);
            User.equip.boots->is_equipped=false;
        }
        User.equip.boots=cur_item;
        User.initialize_gear(User.equip.boots);
    }
    if(cur_item->type==Type::SHIELD){
        if(User.equip.shield!=nullptr){
            User.uninitialize_gear(User.equip.shield);
            User.equip.shield->is_equipped=false;
        }
        User.equip.shield=cur_item;
        User.initialize_gear(User.equip.shield);
    }
    if(cur_item->type==Type::WEAPON){
        if(User.equip.weapon!=nullptr){
            User.uninitialize_gear(User.equip.weapon);
            User.equip.weapon->is_equipped=false;
        }
        User.equip.weapon=cur_item;
        User.initialize_gear(User.equip.weapon);
    }
}

bool unequip_item(Player &User, Item *cur_item){
    if(!cur_item->is_equipped){
        return true;
    }
    if(User.cur_hp<=cur_item->hp){
        return false;
    }
    if(cur_item->type==Type::HELMET&&User.cur_hp>cur_item->hp){
        User.uninitialize_gear(User.equip.helmet);
        User.equip.helmet=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::CHESTPLATE&&User.cur_hp>cur_item->hp){
        User.uninitialize_gear(User.equip.chestplate);
        User.equip.chestplate=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::GREAVES){
        User.uninitialize_gear(User.equip.greaves);
        User.equip.greaves=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::BOOTS){
        User.uninitialize_gear(User.equip.boots);
        User.equip.boots=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::SHIELD){
        User.uninitialize_gear(User.equip.shield);
        User.equip.shield=nullptr;
        cur_item->is_equipped=false;
    }
    if(cur_item->type==Type::WEAPON){
        User.uninitialize_gear(User.equip.weapon);
        User.equip.weapon=nullptr;
        cur_item->is_equipped=false;
    }
    return true;
}

void draw_inventory(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, const Player &User, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, bool is_blacksmith_mode, bool is_inventory_modifier_mode){
    SDL_wclear_mainWin(mainWin, context);
    SDL_wclear_stats_bar(mainWin, context);
    draw_stats(mainWin, context, User);
    if(!items_copy.empty()){
        draw_base(mainWin, context, 34, items_copy.size(), page_num, is_blacksmith_mode, is_inventory_modifier_mode);
        for(long long int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
            print_item(mainWin, context, items_copy[i], iterator, csr_pos==iterator);
        }
        print_description(mainWin, context, items_copy[page_num*30+csr_pos], 35);
    }
    else{
        draw_base(mainWin, context, 34, 0, page_num, is_blacksmith_mode, is_inventory_modifier_mode);
        context->present(*mainWin);
    }
}

bool decrease_amount(unsigned long long int &original, unsigned int amount){
    if(original>=amount){
        original-=amount;
        return true;
    }
    else{
        return false;
    }
}

bool remove_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount){
    if(amount==0){
        return false;
    }
    switch(csr_pos){
        case 0:
            return decrease_amount(User.cores.ancient_core, amount);
        case 1:
            return decrease_amount(User.cores.mysterious_shard, amount);
        case 2:
            return decrease_amount(User.cores.crystallium, amount);
        case 3:
            return decrease_amount(User.cores.crystal_core, amount);
        case 4:
            return decrease_amount(User.materials.common, amount);
        case 5:
            return decrease_amount(User.materials.uncommon, amount);
        case 6:
            return decrease_amount(User.materials.rare, amount);
        case 7:
            return decrease_amount(User.materials.epic, amount);
        case 8:
            return decrease_amount(User.materials.legendary, amount);
        case 9:
            return decrease_amount(User.materials.artifact, amount);
        case 10:
            return decrease_amount(User.blueprint.helmet, amount);
        case 11:
            return decrease_amount(User.blueprint.chestplate, amount);
        case 12:
            return decrease_amount(User.blueprint.greaves, amount);
        case 13:
            return decrease_amount(User.blueprint.boots, amount);
        case 14:
            return decrease_amount(User.blueprint.shield, amount);
        case 15:
            return decrease_amount(User.blueprint.weapon, amount);
        case 16:
            return decrease_amount(User.heal_amount, amount);
        default:
            return false;
    }
}

void add_misc_item(Miscellaneous &User, unsigned int csr_pos, unsigned int amount){
    switch(csr_pos){
        case 0:
            User.cores.ancient_core+=amount;
            break;
        case 1:
            User.cores.mysterious_shard+=amount;
            break;
        case 2:
            User.cores.crystallium+=amount;
            break;
        case 3:
            User.cores.crystal_core+=amount;
            break;
        case 4:
            User.materials.common+=amount;
            break;
        case 5:
            User.materials.uncommon+=amount;
            break;
        case 6:
            User.materials.rare+=amount;
            break;
        case 7:
            User.materials.epic+=amount;
            break;
        case 8:
            User.materials.legendary+=amount;
            break;
        case 9:
            User.materials.artifact+=amount;
            break;
        case 10:
            User.blueprint.helmet+=amount;
            break;
        case 11:
            User.blueprint.chestplate+=amount;
            break;
        case 12:
            User.blueprint.greaves+=amount;
            break;
        case 13:
            User.blueprint.boots+=amount;
            break;
        case 14:
            User.blueprint.shield+=amount;
            break;
        case 15:
            User.blueprint.weapon+=amount;
            break;
        case 16:
            User.heal_amount+=amount;
            break;
        default:
            break;
    }
}

void print_misc_item(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, Miscellaneous &User, unsigned int csr_pos){
    SDL_wclear_mainWin(mainWin, context);
    tcod::print(*mainWin, {0, 1}, "Cores:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(mainWin, context, "Ancient Cores: "+std::to_string(User.cores.ancient_core), DARK_RED, 2, csr_pos==0);
    print_bold_with_condition(mainWin, context, "Mysterious Shard: "+std::to_string(User.cores.mysterious_shard), DARK_RED, 3, csr_pos==1);
    print_bold_with_condition(mainWin, context, "Crystallium: "+std::to_string(User.cores.crystallium), YELLOW, 4, csr_pos==2);
    print_bold_with_condition(mainWin, context, "Crystal Cores: "+std::to_string(User.cores.crystal_core), PURPLE, 5, csr_pos==3);
    tcod::print(*mainWin, {0, 7}, "Materials:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(mainWin, context, "Common: "+std::to_string(User.materials.common), WHITE, 8, csr_pos==4);
    print_bold_with_condition(mainWin, context, "Uncommon: "+std::to_string(User.materials.uncommon), GREEN, 9, csr_pos==5);
    print_bold_with_condition(mainWin, context, "Rare: "+std::to_string(User.materials.rare), BLUE, 10, csr_pos==6);
    print_bold_with_condition(mainWin, context, "Epic: "+std::to_string(User.materials.epic), PURPLE, 11, csr_pos==7);
    print_bold_with_condition(mainWin, context, "Legendary: "+std::to_string(User.materials.legendary), YELLOW, 12, csr_pos==8);
    print_bold_with_condition(mainWin, context, "Artifact: "+std::to_string(User.materials.artifact), DARK_RED, 13, csr_pos==9);
    tcod::print(*mainWin, {0, 15}, "Blueprints:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(mainWin, context, "Helmet: "+std::to_string(User.blueprint.helmet), WHITE, 16, csr_pos==10);
    print_bold_with_condition(mainWin, context, "Chestplate: "+std::to_string(User.blueprint.chestplate), WHITE, 17, csr_pos==11);
    print_bold_with_condition(mainWin, context, "Greaves: "+std::to_string(User.blueprint.greaves), WHITE, 18, csr_pos==12);
    print_bold_with_condition(mainWin, context, "Boots: "+std::to_string(User.blueprint.boots), WHITE, 19, csr_pos==13);
    print_bold_with_condition(mainWin, context, "Shield: "+std::to_string(User.blueprint.shield), WHITE, 20, csr_pos==14);
    print_bold_with_condition(mainWin, context, "Weapon: "+std::to_string(User.blueprint.weapon), WHITE, 21, csr_pos==15);
    tcod::print(*mainWin, {0, 23}, "Others:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(mainWin, context, "First-Aid Kits: "+std::to_string(User.heal_amount), WHITE, 24, csr_pos==16);
    context->present(*mainWin);
}

void show_misc_items(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, Miscellaneous &User){
    unsigned int csr_pos=0;
    int ch;
    clear_and_draw_dialog(mainWin, context, "Miscallaneous items:");
    while(true){
        print_misc_item(mainWin, context, User, csr_pos);
        ch=SDL_getch(mainWin, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<16){
            clear_and_draw_dialog(mainWin, context, "Miscallaneous items:");
            csr_pos++;
        }
        else if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(mainWin, context, "Miscallaneous items:");
            csr_pos--;
        }
        else if(ch=='r'){
            unsigned int quantity=get_int(mainWin, context, "([ESC] to cancel) Quantity to be trashed: ");
            if(!remove_misc_item(User, csr_pos, quantity)){
                clear_and_draw_dialog(mainWin, context, "[System] Invalid amount");
            }
            else{
                clear_and_draw_dialog(mainWin, context, "[System] Success");
            }
        }
        else if(ch=='q'||ch=='m'){
            return;
        }
    }
}

bool decrease_materials(Rarity rarity, Miscellaneous &misc, unsigned int amount){
    if(rarity==Rarity::COMMON&&misc.materials.common>=amount){
        misc.materials.common-=amount;
        return true;
    }
    else if(rarity==Rarity::UNCOMMON&&misc.materials.uncommon>=amount){
        misc.materials.uncommon-=amount;
        return true;
    }
    else if(rarity==Rarity::RARE&&misc.materials.rare>=amount){
        misc.materials.rare-=amount;
        return true;
    }
    else if(rarity==Rarity::EPIC&&misc.materials.epic>=amount){
        misc.materials.epic-=amount;
        return true;
    }
    else if(rarity==Rarity::LEGENDARY&&misc.materials.legendary>=amount){
        misc.materials.legendary-=amount;
        return true;
    }
    else if(rarity==Rarity::ARTIFACT&&misc.materials.artifact>=amount){
        misc.materials.artifact-=amount;
        return true;
    }
    return false;
}

bool enhance_item(Item *cur_item, Miscellaneous &misc, unsigned int amount){
    bool require_enhance=false;
    if(decrease_materials(cur_item->rarity, misc, amount)){
        cur_item->enhancement+=amount;
        cur_item->reinitialize_item();
        return true;
    }
    return false;
}

void draw_crafting_selection(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, const Miscellaneous &misc, unsigned int csr_pos, int blueprint_selection, int material_selection){
    SDL_wclear_mainWin(mainWin, context);
    tcod::print(*mainWin, {0, 1}, "Select a blueprint:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(csr_pos==1||blueprint_selection==1){
        tcod::print(*mainWin, {0, 2}, "Helmet ["+std::to_string(misc.blueprint.helmet)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 2}, "Helmet ["+std::to_string(misc.blueprint.helmet)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==2||blueprint_selection==2){
        tcod::print(*mainWin, {0, 3}, "Chestplate ["+std::to_string(misc.blueprint.chestplate)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 3}, "Chestplate ["+std::to_string(misc.blueprint.chestplate)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==3||blueprint_selection==3){
        tcod::print(*mainWin, {0, 4}, "Greaves ["+std::to_string(misc.blueprint.greaves)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 4}, "Greaves ["+std::to_string(misc.blueprint.greaves)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==4||blueprint_selection==4){
        tcod::print(*mainWin, {0, 5}, "Boots ["+std::to_string(misc.blueprint.boots)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 5}, "Boots ["+std::to_string(misc.blueprint.boots)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==5||blueprint_selection==5){
        tcod::print(*mainWin, {0, 6}, "Shield ["+std::to_string(misc.blueprint.shield)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 6}, "Shield ["+std::to_string(misc.blueprint.shield)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==6||blueprint_selection==6){
        tcod::print(*mainWin, {0, 7}, "Weapon ["+std::to_string(misc.blueprint.weapon)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 7}, "Weapon ["+std::to_string(misc.blueprint.weapon)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    tcod::print(*mainWin, {0, 9}, "Select the material:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(csr_pos==9||material_selection==9){
        tcod::print(*mainWin, {0, 10}, "Common ["+std::to_string(misc.materials.common)+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 10}, "Common ["+std::to_string(misc.materials.common)+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==10||material_selection==10){
        tcod::print(*mainWin, {0, 11}, "Uncommon ["+std::to_string(misc.materials.uncommon)+"]", &BLACK, &GREEN, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 11}, "Uncommon ["+std::to_string(misc.materials.uncommon)+"]", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==11||material_selection==11){
        tcod::print(*mainWin, {0, 12}, "Rare ["+std::to_string(misc.materials.rare)+"]", &BLACK, &BLUE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 12}, "Rare ["+std::to_string(misc.materials.rare)+"]", &BLUE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==12||material_selection==12){
        tcod::print(*mainWin, {0, 13}, "Epic ["+std::to_string(misc.materials.epic)+"]", &BLACK, &PURPLE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 13}, "Epic ["+std::to_string(misc.materials.epic)+"]", &PURPLE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==13||material_selection==13){
        tcod::print(*mainWin, {0, 14}, "Legendary ["+std::to_string(misc.materials.legendary)+"]", &BLACK, &YELLOW, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 14}, "Legendary ["+std::to_string(misc.materials.legendary)+"]", &YELLOW, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==14||material_selection==14){
        tcod::print(*mainWin, {0, 15}, "Artifact ["+std::to_string(misc.materials.artifact)+"]", &BLACK, &DARK_RED, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 15}, "Artifact ["+std::to_string(misc.materials.artifact)+"]", &DARK_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(csr_pos==16){
        tcod::print(*mainWin, {0, 17}, "[Done]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
    }
    else{
        tcod::print(*mainWin, {0, 17}, "[Done]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    clear_and_draw_dialog(mainWin, context, "[Blacksmith] What do you want to craft today?");
}

bool check_if_sufficient_materials(Miscellaneous &misc, int material_selection, int required_material, Rarity &rarity){
    switch(material_selection){
        case 9: // common
            if(misc.materials.common>=required_material){
                misc.materials.common-=required_material;
                rarity=Rarity::COMMON;
                return true;
            }
            else{
                return false;
            }
            break;
        case 10: // uncommon
            if(misc.materials.uncommon>=required_material){
                misc.materials.uncommon-=required_material;
                rarity=Rarity::UNCOMMON;
                return true;
            }
            else{
                return false;
            }
            break;
        case 11: // rare
            if(misc.materials.rare>=required_material){
                misc.materials.rare-=required_material;
                rarity=Rarity::RARE;
                return true;
            }
            else{
                return false;
            }
            break;
        case 12: // epic
            if(misc.materials.epic>=required_material){
                misc.materials.epic-=required_material;
                rarity=Rarity::EPIC;
                return true;
            }
            else{
                return false;
            }
            break;
        case 13: // legendary
            if(misc.materials.legendary>=required_material){
                misc.materials.legendary-=required_material;
                rarity=Rarity::LEGENDARY;
                return true;
            }
            else{
                return false;
            }
            break;
        case 14: // artifact
            if(misc.materials.artifact>=required_material){
                misc.materials.artifact-=required_material;
                rarity=Rarity::ARTIFACT;
                return true;
            }
            else{
                return false;
            }
            break;
        default:
            return false;
    }
}

bool check_if_craft_valid(int blueprint_selection, int material_selection, Miscellaneous &misc, Type &gear_type, Rarity &rarity){
    switch(blueprint_selection){
        case 1: // helmet
            if(misc.blueprint.helmet>0){
                if(check_if_sufficient_materials(misc, material_selection, 3, rarity)){
                    misc.blueprint.helmet--;
                    gear_type=Type::HELMET;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 2: // chestplate
            if(misc.blueprint.chestplate>0){
                if(check_if_sufficient_materials(misc, material_selection, 5, rarity)){
                    misc.blueprint.chestplate--;
                    gear_type=Type::CHESTPLATE;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 3: // greaves
            if(misc.blueprint.greaves>0){
                if(check_if_sufficient_materials(misc, material_selection, 3, rarity)){
                    misc.blueprint.greaves--;
                    gear_type=Type::GREAVES;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 4: // boots
            if(misc.blueprint.boots>0){
                if(check_if_sufficient_materials(misc, material_selection, 2, rarity)){
                    misc.blueprint.boots--;
                    gear_type=Type::BOOTS;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 5: // shield
            if(misc.blueprint.shield>0){
                if(check_if_sufficient_materials(misc, material_selection, 4, rarity)){
                    misc.blueprint.shield--;
                    gear_type=Type::SHIELD;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 6: // weapon
            if(misc.blueprint.weapon>0){
                if(check_if_sufficient_materials(misc, material_selection, 6, rarity)){
                    misc.blueprint.weapon--;
                    gear_type=Type::WEAPON;
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        default:
            return false;
    }
    return false;
}

void crafting_mode(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, Player &User){
    unsigned int csr_pos=1;
    int blueprint_selection=0, material_selection=0;
    int ch;
    while(true){
        draw_crafting_selection(mainWin, context, User.inv.misc, csr_pos, blueprint_selection, material_selection);
        ch=SDL_getch(mainWin, context);
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<15){
            if(csr_pos==6){
                csr_pos=9;
            }
            else if(csr_pos==14){
                csr_pos=16;
            }
            else{
                csr_pos++;
            }
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>1){
            if(csr_pos==9){
                csr_pos=6;
            }
            else if(csr_pos==16){
                csr_pos=14;
            }
            else{
                csr_pos--;
            }
        }
        else if(ch==SDLK_TAB){ // KEY_TAB
            if(csr_pos<7){
                csr_pos+=8;
            }
            else if(csr_pos<15){
                csr_pos-=8;
            }
        }
        else if(ch==SDLK_RETURN){ // KEY_ENTER
            if(csr_pos<7){
                blueprint_selection=csr_pos;
            }
            else if(csr_pos<15){
                material_selection=csr_pos;
            }
            if(csr_pos==16&&blueprint_selection!=0&&material_selection!=0){
                Type gear_type;
                Rarity rarity;
                if(check_if_craft_valid(blueprint_selection, material_selection, User.inv.misc, gear_type, rarity)){
                    User.add_item(craft_item(rarity, gear_type));
                    return;
                }
            }
        }
        else if(ch=='q'){
            return;
        }
    }
}

void salvage_item(Player &User, Item *item){
    switch(item->rarity){
        case Rarity::COMMON:
            User.inv.misc.materials.common+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::UNCOMMON:
            User.inv.misc.materials.uncommon+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::RARE:
            User.inv.misc.materials.rare+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::EPIC:
            User.inv.misc.materials.epic+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::LEGENDARY:
            User.inv.misc.materials.legendary+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        case Rarity::ARTIFACT:
            User.inv.misc.materials.artifact+=((item->enhancement/2.0)+1.0)*(item->durability/50.0);
            break;
        default:
            break;
    }
    User.uninitialize_stats();
    User.remove_item(item);
    User.initialize_stats();
}

void inventory_mode(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, Player &User, NoDelete &perm_config){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    init_copy(User.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(User.inv.item, items_copy, perm_config);
    }
    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
    int ch;
    while(true){
        ch=SDL_getch(mainWin, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if((ch=='a'||ch==SDLK_LEFT)&&page_num>0){
            page_num--;
            csr_pos=0;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if((ch=='d'||ch==SDLK_RIGHT)&&((page_num+1)*30<items_copy.size())){
            page_num++;
            csr_pos=0;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='e'){
            if(!items_copy[csr_pos+(page_num*30)]->is_equipped&&items_copy[csr_pos+(page_num*30)]->durability>0){ // if the current item is unequipped
                equip_item(User, items_copy[csr_pos+(page_num*30)]);
                draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
            }
            else if(items_copy[csr_pos+(page_num*30)]->is_equipped){
                unequip_item(User, items_copy[csr_pos+(page_num*30)]);
                draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
            }
        }
        else if(ch=='r'){
            items_copy[csr_pos+(page_num*30)]->name=get_string(mainWin, context, "([ESC] to cancel) Rename item to: ", items_copy[csr_pos+(page_num*30)]->name);
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='m'){
            show_misc_items(mainWin, context, User.inv.misc);
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='H'){
            help_mode(mainWin, context, "inventory_mode");
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='R'){
            if(SDL_getch_y_or_n(mainWin, context, "[System] Delete item? (You can salvage instead at the blacksmith) [y/n]")){
                if(unequip_item(User, items_copy[csr_pos+(page_num*30)])){
                    User.remove_item(items_copy[csr_pos+(page_num*30)]);
                    if(User.inv.item.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    process_copy(User.inv.item, items_copy, perm_config);
                }
            }
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='-'){ // Inventory Modifier
            inventory_modifier_selection(mainWin, context, perm_config, User.inv.item, items_copy);
            process_copy(User.inv.item, items_copy, perm_config);
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
    }
}

void reforge_repair_mode(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, Player &User, NoDelete &perm_config){
    std::vector<Item *> items_copy;
    init_copy(User.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(User.inv.item, items_copy, perm_config);
    }
    bool safety_mode=true;
    unsigned int page_num=0;
    int csr_pos=0;
    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
    int ch;
    while(true){
        ch=SDL_getch(mainWin, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if((ch=='a'||ch==SDLK_LEFT)&&(page_num>0)){
            page_num--;
            csr_pos=0;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if((ch=='d'||ch==SDLK_RIGHT)&&((page_num+1)*30<items_copy.size())){
            page_num++;
            csr_pos=0;
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='q'){
            return;
        }
        else if(ch=='m'){
            show_misc_items(mainWin, context, User.inv.misc);
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
        }
        else if(ch=='-'){ // Inventory Modifier
            inventory_modifier_selection(mainWin, context, perm_config, User.inv.item, items_copy);
            process_copy(User.inv.item, items_copy, perm_config);
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos);
        }
        else if(ch=='M'){ // Multiselect salvage (std::queue)
            if(!safety_mode){
                safety_mode=true;
                clear_and_draw_dialog(mainWin, context, "Safety Mode ON");
            }
            else{
                safety_mode=false;
                clear_and_draw_dialog(mainWin, context, "Safety Mode OFF [Salvaging will now not require confirmation]");
            }
        }
        else if(ch=='r'){ // Repair
            if(items_copy[csr_pos+(page_num*30)]->durability<100.0){
                const unsigned int cost=(100.0-items_copy[csr_pos+(page_num*30)]->durability)*rarity_value(items_copy[csr_pos+(page_num*30)]->rarity)*15;
                const unsigned int material_cost=2*(1+((100-items_copy[csr_pos+(page_num*30)]->durability)/100));
                std::stringstream ss;
                ss << "[System] Attempt repair? Gold:" << cost << " Material:" << material_cost << " [y/n]";
                if(SDL_getch_y_or_n(mainWin, context, ss.str())){
                    if(User.gold>=cost&&decrease_materials(items_copy[csr_pos+(page_num*30)]->rarity, User.inv.misc, material_cost)){
                        User.gold-=cost;
                        draw_stats(mainWin, context, User);
                        if(return_chance(items_copy[csr_pos+(page_num*30)]->durability*2)){
                            items_copy[csr_pos+(page_num*30)]->durability=100.0;
                            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                            clear_and_draw_dialog(mainWin, context, "[System] Success: Item Repaired.");
                        }
                        else{
                            clear_and_draw_dialog(mainWin, context, "[System] Error: Repair failed.");
                        }
                    }
                    else{
                        clear_and_draw_dialog(mainWin, context, "[System] Error: Insufficient gold/materials");
                    }
                }
                else{
                    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                }
            }
        }
        else if(ch=='e'){ // Enhance
            unsigned int amount=get_int(mainWin, context, "[System] Amount of materials you want to use in this enhancement: ");
            if(amount>0){
                User.uninitialize_stats();
                if(enhance_item(items_copy[csr_pos+(page_num*30)], User.inv.misc, amount)){
                    User.initialize_stats();
                    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                    clear_and_draw_dialog(mainWin, context, "[System] Success");
                }
                else{
                    User.initialize_stats();
                    clear_and_draw_dialog(mainWin, context, "[System] Failure");
                }
            }
            else{
                clear_and_draw_dialog(mainWin, context, "[System] Failure");
            }
        }
        else if(ch=='R'){ // Reforge
            if(SDL_getch_y_or_n(mainWin, context, "[System] Reforge item? (This action is irreversible) [y/n]")){
                unsigned int gold_cost=rarity_value(items_copy[csr_pos+(page_num*30)]->rarity)*200;
                unsigned int material_cost=2*(1+((100-items_copy[csr_pos+(page_num*30)]->durability)/100.0));
                std::stringstream ss;
                ss << "Proceed? Cost of reforge: " << gold_cost << " Gold, " << material_cost << " Materials [y/n]";
                if(SDL_getch_y_or_n(mainWin, context, ss.str())&&decrease_materials(items_copy[csr_pos+(page_num*30)]->rarity, User.inv.misc, material_cost)&&User.gold>=gold_cost){
                    User.gold-=gold_cost;
                    bool use_crystallium=false, use_ancient_core=false;
                    unsigned int crystallium_amount=get_int(mainWin, context, "Use Crystallium? [0 to not use any]: ");
                    unsigned int ancient_core_amount=0;
                    if(crystallium_amount==0){
                        ancient_core_amount=get_int(mainWin, context, "Use Ancient Cores? [0 to not use any]: ");
                        if(ancient_core_amount>0){
                            use_ancient_core=true;
                        }
                    }
                    else if(crystallium_amount>0){
                        use_crystallium=true;
                    }
                    if(User.inv.misc.cores.ancient_core>=ancient_core_amount&&User.inv.misc.cores.crystallium>=crystallium_amount){
                        User.inv.misc.cores.ancient_core-=ancient_core_amount;
                        User.inv.misc.cores.crystallium-=crystallium_amount;
                        User.uninitialize_stats();
                        reforge_item(ancient_core_amount, crystallium_amount, items_copy[csr_pos+(page_num*30)]);
                        items_copy[csr_pos+(page_num*30)]->reinitialize_item();
                        User.initialize_stats();
                        draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                        draw_stats(mainWin, context, User);
                    }
                    else{
                        clear_and_draw_dialog(mainWin, context, "[System] Insufficient materials");
                    }
                }
                else{
                    clear_and_draw_dialog(mainWin, context, "[System] Aborted");
                }
            }
            else{
                draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
            }
        }
        else if(ch=='c'){
            crafting_mode(mainWin, context, User);
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
        }
        else if(ch=='S'){
            if(safety_mode){
                if(SDL_getch_y_or_n(mainWin, context, "Salvage item? (This action is irreversible) [y/n]")&&unequip_item(User, items_copy[csr_pos+(page_num*30)])){
                    salvage_item(User, items_copy[csr_pos+(page_num*30)]);
                    if(User.inv.item.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    process_copy(User.inv.item, items_copy, perm_config);
                    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                }
                else{
                    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                }
            }
            else{
                if(unequip_item(User, items_copy[csr_pos+(page_num*30)])){
                    salvage_item(User, items_copy[csr_pos+(page_num*30)]);
                    if(User.inv.item.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    process_copy(User.inv.item, items_copy, perm_config);
                    draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
                    clear_and_draw_dialog(mainWin, context, "[System] Salvage success");
                }
                else{
                    clear_and_draw_dialog(mainWin, context, "[System] Salvage Failure");
                }
            }
        }
        else if(ch=='H'){
            help_mode(mainWin, context, "blacksmith_mode");
            draw_inventory(mainWin, context, User, items_copy, page_num, csr_pos, true);
        }
    }
}

