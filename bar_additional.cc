#include <iomanip>
#include "headers/main.h"
#include "headers/mode.h"
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"

int calculate_price(const Item cur, int relationship, bool player_sell){
    int price=100*rarity_value(cur.rarity)*rarity_value(cur.rarity);
    if(cur.def>0&&cur.type!=TYPE_GREAVES){
        price*=1.5+(cur.def/100.0);
    }
    else{
        price*=1.5+(cur.def/500.0);
    }
    if(cur.crit_dmg>0){
        price*=1.5+(cur.crit_dmg/25.0);
    }
    if(cur.crit_chance>0){
        price*=1.5+(cur.crit_chance/25.0);
    }
    switch(cur.type){
        case TYPE_HELMET:
            price*=cur.hp/(40*rarity_value(cur.rarity))*2;
            break;
        case TYPE_CHESTPLATE:
            price*=cur.hp/(100*rarity_value(cur.rarity))*2;
            break;
        case TYPE_GREAVES:
            price*=cur.def/(50*rarity_value(cur.rarity))*2;
            break;
        case TYPE_BOOTS:
            price*=cur.def/(30*rarity_value(cur.rarity))*2;
            break;
        case TYPE_SHIELD:
            price*=cur.shield/(100*rarity_value(cur.rarity))*2;
            break;
        case TYPE_WEAPON:
            price*=cur.attk/(200*rarity_value(cur.rarity))*4;
            break;
        default:
            break;
    }
    price*=1+((100-relationship)/100);
    if(player_sell){
        return (price*0.8)*(cur.durability/100.0);
    }
    return price;
}

void print_item_description_1(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int &line){
    tcod::print(*main_win, {0, line+1}, "Name: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {6, line+1}, cur_item->name, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, line+2}, "Rarity: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->rarity){
        case RARITY_COMMON:
            tcod::print(*main_win, {8, line+2}, "Common", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_UNCOMMON:
            tcod::print(*main_win, {8, line+2}, "Uncommon", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_RARE:
            tcod::print(*main_win, {8, line+2}, "Rare", &BLUE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_EPIC:
            tcod::print(*main_win, {8, line+2}, "Epic", &PURPLE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_LEGENDARY:
            tcod::print(*main_win, {8, line+2}, "Legendary", &YELLOW, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_ARTIFACT:
            tcod::print(*main_win, {8, line+2}, "Artifact", &DARK_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {8, line+2}, "-ERROR-", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
    }
    line+=2;
    tcod::print(*main_win, {0, line+1}, "Type: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->type){
        case TYPE_HELMET:
            tcod::print(*main_win, {6, line+1}, "Helmet", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_CHESTPLATE:
            tcod::print(*main_win, {6, line+1}, "Chestplate", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_GREAVES:
            tcod::print(*main_win, {6, line+1}, "Greaves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_BOOTS:
            tcod::print(*main_win, {6, line+1}, "Boots", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_SHIELD:
            tcod::print(*main_win, {6, line+1}, "Shield", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_WEAPON:
            tcod::print(*main_win, {6, line+1}, "Weapon", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {6, line+1}, "-ERROR-", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    line++;
}

void print_item_description_2(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int &line){
    std::stringstream ss;
    tcod::print(*main_win, {0, line+1}, ("Durability: "+std::to_string(cur_item->durability)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    if(cur_item->uses>0){
        tcod::print(*main_win, {0, line+1}, ("Uses: "+std::to_string(cur_item->uses)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->calibration>0){
        tcod::print(*main_win, {0, line+1}, ("Calibration Level: "+std::to_string(cur_item->calibration)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->enhancement>0){
        tcod::print(*main_win, {0, line+1}, ("Enhancement: "+std::to_string(cur_item->enhancement)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->hp>0){
        tcod::print(*main_win, {0, line+1}, ("Health: "+std::to_string(cur_item->hp)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->attk>0){
        tcod::print(*main_win, {0, line+1}, ("Damage: "+std::to_string(cur_item->attk)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->def>0){
        tcod::print(*main_win, {0, line+1}, ("Defence: "+std::to_string(cur_item->def)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->shield>0){
        tcod::print(*main_win, {0, line+1}, ("Shield: "+std::to_string(cur_item->shield)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_chance>0){
        tcod::print(*main_win, {0, line+1}, ("Critical Chance: "+std::to_string(cur_item->crit_chance)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_dmg>0){
        tcod::print(*main_win, {0, line+1}, ("Critical Damage: "+std::to_string(cur_item->crit_dmg)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
}

void print_trader_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price, bool is_locked){
    if(is_locked){
        tcod::print(*main_win, {0, line+1}, "[Locked]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(*main_win, {0, line+1}, "Price: "+std::to_string(price), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(*main_win);
}

void print_mysterious_trader_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, double quality_points){
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(*main_win, {0, line+1}, "Quality Points: "+std::to_string(quality_points), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(*main_win);
}

void print_sell_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price){
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(*main_win, {0, line+1}, "Equipped: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(cur_item->is_equipped){
        tcod::print(*main_win, {10, line+1}, "true", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(!cur_item->is_equipped){
        tcod::print(*main_win, {10, line+1}, "false", &LIGHT_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    line++;
    tcod::print(*main_win, {0, line+1}, "Price: "+std::to_string(price), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(*main_win);
}

void draw_trader_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, int price, bool is_locked=false){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, User);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_trader_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price, is_locked);
}

void draw_mysterious_trader_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<Item *> &items_copy, unsigned int csr_pos, double quality_points){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_base(main_win, context, 34, items_copy.size(), 0, false, false);
    for(int i=0, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_mysterious_trader_item_description(main_win, context, items_copy[csr_pos], 35, quality_points);
}

void draw_sell_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item *> &items_copy, unsigned int page_num, unsigned int csr_pos, int price){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, User);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_sell_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price);
}

void store_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, Chest &chest){
    unsigned int csr_pos=0;
    clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
    while(true){
        print_misc_item(main_win, context, User, csr_pos);
        int ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<16){
            clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
            csr_pos++;
        }
        if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
            csr_pos--;
        }
        if(ch=='S'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be stored: ");
            if(remove_misc_item(User, csr_pos, quantity)){
                add_misc_item(chest.misc_storage, csr_pos, quantity);
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Invalid amount");
            }
        }
        if(ch=='q'){
            return;
        }
    }
}

void retrieve_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User, Chest &chest){
    unsigned int csr_pos=0;
    clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
    while(true){
        print_misc_item(main_win, context, chest.misc_storage, csr_pos);
        int ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<16){
            clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
            csr_pos++;
        }
        if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
            csr_pos--;
        }
        if(ch=='R'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be retrieved: ");
            if(remove_misc_item(chest.misc_storage, csr_pos, quantity)){
                add_misc_item(User, csr_pos, quantity);
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Invalid amount");
            }
        }
        if(ch=='q'){
            return;
        }
    }
}

void inventory_storage(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config, Chest &chest){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_called=false;
    init_copy(User.inv.item, items_copy);
    draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
    while(true){
        int ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
        if(ch=='a'||ch==SDLK_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
            }
        }
        if(ch=='d'||ch==SDLK_RIGHT){
            if((page_num+1)*30<items_copy.size()){
                page_num++;
                csr_pos=0;
                draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
            }
        }
        if(ch=='S'){
            if(unequip_item(User, items_copy[csr_pos+page_num*30])){
                chest.gear_storage.insert(chest.gear_storage.begin(), *items_copy[csr_pos+page_num*30]);
                User.remove_item(items_copy[csr_pos+page_num*30]);
                if(User.inv.item.empty()){
                    return;
                }
                else if(csr_pos>0){
                    csr_pos--;
                }
                else if(csr_pos==0&&page_num>0){
                    page_num--;
                }
                if(is_modifier_called){
                    process_copy(User.inv.item, items_copy, perm_config);
                }
                else{
                    init_copy(User.inv.item, items_copy);
                }
                draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Error Storing Item");
            }
        }
        if(ch=='q'){
            return;
        }
        if(ch=='-'){ // Inventory Modifier
            is_modifier_called=true;
            inventory_modifier_selection(main_win, context, perm_config, User.inv.item, items_copy);
            process_copy(User.inv.item, items_copy, perm_config);
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
    }
}

void inventory_retrieve(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, NoDelete &perm_config, Chest &chest){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_called=false;
    init_copy(chest.gear_storage, items_copy);
    draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
    while(true){
        int ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
        if(ch=='a'||ch==SDLK_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
            }
        }
        if(ch=='d'||ch==SDLK_RIGHT){
            if((page_num+1)*30<items_copy.size()){
                page_num++;
                csr_pos=0;
                draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
            }
        }
        if(ch=='R'){
            User.add_item(*items_copy[csr_pos+page_num*30]);
            chest.gear_storage.erase(chest.gear_storage.begin()+(csr_pos+page_num*30));
            if(chest.gear_storage.empty()){
                return;
            }
            else if(csr_pos>0){
                csr_pos--;
            }
            else if(csr_pos==0&&page_num>0){
                page_num--;
            }
            if(is_modifier_called){
                process_copy(chest.gear_storage, items_copy, perm_config);
            }
            else{
                init_copy(chest.gear_storage, items_copy);
            }
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
        if(ch=='q'){
            return;
        }
        if(ch=='-'){ // Inventory Modifier
            is_modifier_called=true;
            inventory_modifier_selection(main_win, context, perm_config, chest.gear_storage, items_copy);
            process_copy(chest.gear_storage, items_copy, perm_config);
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
    }
}

void show_merchant_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &User){
    unsigned int page_num=0;
    int csr_pos=0;
    std::vector<Item> original_copy;
    for(auto i: gear_merchant.store){
        original_copy.push_back(i.first);
    }
    std::vector<Item *> items_copy;
    init_copy(original_copy, items_copy);
    draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation));
    while(true){
        int ch=SDL_getch(main_win, context);
        if(ch=='q'){
            return;
        }
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            tcod::print(*main_win, {0, 39}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            tcod::print(*main_win, {0, 39}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
        }
        if(ch=='L'){
            if(!gear_merchant.store[csr_pos+page_num*30].second){
                gear_merchant.store[csr_pos+page_num*30].second=true;
                draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                clear_and_draw_dialog(main_win, context, "Item Locked!");
            }
            else{
                gear_merchant.store[csr_pos+page_num*30].second=false;
                draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                clear_and_draw_dialog(main_win, context, "Item Unlocked!");
            }
        }
        if(ch=='B'){
            clear_and_draw_dialog(main_win, context, "Confirm purchase of item? [y/n]");
            while(true){
                ch=SDL_getch(main_win, context);
                if(ch>0&&ch<128){
                    break;
                }
            }
            if(ch=='y'){
                unsigned int amount=calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation);
                if(amount<=User.gold){
                    User.gold-=amount;
                    draw_stats(main_win, context, User);
                    User.add_item(original_copy[csr_pos+page_num*30]);
                    gear_merchant.store.erase(gear_merchant.store.begin()+(csr_pos+page_num*30));
                    if(gear_merchant.store.empty()){
                        return;
                    }
                    else if(csr_pos>0){
                        csr_pos--;
                    }
                    else if(csr_pos==0&&page_num>0){
                        page_num--;
                    }
                    original_copy.clear();
                    for(auto i: gear_merchant.store){
                        original_copy.push_back(i.first);
                    }
                    init_copy(original_copy, items_copy);
                    draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30], gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                    clear_and_draw_dialog(main_win, context, "[System] Item added to inventory");
                }
                else{
                    clear_and_draw_dialog(main_win, context, "[System] Insufficient gold");
                }
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Transaction aborted");
            }
        }
    }
}

void sell_items_to_merchant(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Merchant &gear_merchant, Player &User, NoDelete &perm_config){
    std::vector<Item *> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_active=false;
    init_copy(User.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(User.inv.item, items_copy, perm_config);
    }
    draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(User.inv.item[csr_pos+page_num*30], gear_merchant.relation, true));
    while(true){
        int ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
        if(ch=='a'||ch==SDLK_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
            }
        }
        if(ch=='d'||ch==SDLK_RIGHT){
            if((page_num+1)*30<items_copy.size()){
                page_num++;
                csr_pos=0;
                draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
            }
        }
        if(ch=='S'){
            clear_and_draw_dialog(main_win, context, "Sell item? [y/n]");
            while(true){
                ch=SDL_getch(main_win, context);
                if(ch>0&&ch<128){
                    break;
                }
            }
            if(ch=='y'&&unequip_item(User, items_copy[csr_pos+page_num*30])){
                User.gold+=calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true);
                User.remove_item(items_copy[csr_pos+page_num*30]);
                if(User.inv.item.empty()){
                    return;
                }
                else if(csr_pos>0){
                    csr_pos--;
                }
                else if(csr_pos==0&&page_num>0){
                    page_num--;
                }
                if(is_modifier_active){
                    process_copy(User.inv.item, items_copy, perm_config);
                }
                else{
                    init_copy(User.inv.item, items_copy);
                }
                draw_stats(main_win, context, User);
                draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
            }
        }
        if(ch=='q'){
            return;
        }
        if(ch=='-'){ // Inventory Modifier
            is_modifier_active=true;
            inventory_modifier_selection(main_win, context, perm_config, User.inv.item, items_copy);
            process_copy(User.inv.item, items_copy, perm_config);
            draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30], gear_merchant.relation, true));
        }
    }
}

void print_trader_misc_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    // Buy
    tcod::print(*main_win, {0, 1}, "Buy Materials/Blueprints:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[Common] "+std::to_string(trader_prices.at("common_material"))+" Gold each", WHITE, 2, csr_pos==0);
    print_bold_with_condition(main_win, context, "[Uncommon] "+std::to_string(trader_prices.at("uncommon_material"))+" Gold each", GREEN, 3, csr_pos==1);
    print_bold_with_condition(main_win, context, "[Rare] "+std::to_string(trader_prices.at("rare_material"))+" Gold each", BLUE, 4, csr_pos==2);
    print_bold_with_condition(main_win, context, "[Epic] "+std::to_string(trader_prices.at("epic_material"))+" Gold each", PURPLE, 5, csr_pos==3);
    print_bold_with_condition(main_win, context, "[Legendary] "+std::to_string(trader_prices.at("legendary_material"))+" Gold each", YELLOW, 6, csr_pos==4);
    tcod::print(*main_win, {0, 8}, "Blueprints:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[Helmet] "+std::to_string(trader_prices.at("helmet_blueprint"))+" Crystal Cores each", WHITE, 9, csr_pos==5);
    print_bold_with_condition(main_win, context, "[Chestplate] "+std::to_string(trader_prices.at("chestplate_blueprint"))+" Crystal Cores each", WHITE, 10, csr_pos==6);
    print_bold_with_condition(main_win, context, "[Greaves] "+std::to_string(trader_prices.at("greaves_blueprint"))+" Crystal Cores each", WHITE, 11, csr_pos==7);
    print_bold_with_condition(main_win, context, "[Boots] "+std::to_string(trader_prices.at("boots_blueprint"))+" Crystal Cores each", WHITE, 12, csr_pos==8);
    print_bold_with_condition(main_win, context, "[Shield] "+std::to_string(trader_prices.at("shield_blueprint"))+" Crystal Cores each", WHITE, 13, csr_pos==9);
    print_bold_with_condition(main_win, context, "[Weapon] "+std::to_string(trader_prices.at("weapon_blueprint"))+" Crystal Cores each", WHITE, 14, csr_pos==10);
    tcod::print(*main_win, {0, 16}, "Others:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[First-Aid Kits] "+std::to_string(trader_prices.at("first_aid_kit"))+" Crystal Cores each", PURPLE, 17, csr_pos==11);
    // Sell
    tcod::print(*main_win, {0, 21}, "--------------------------------------------------------------------------------", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 25}, "Sell Cores/Materials:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0, 26}, "Cores:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[Crystal Cores] "+std::to_string((unsigned long long int) (trader_prices.at("crystal_core")*0.8))+" Gold each", BLUE, 27, csr_pos==12);
    tcod::print(*main_win, {0, 29}, "Materials:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[Common] "+std::to_string((unsigned long long int) (trader_prices.at("common_material")*0.8))+" Gold each", WHITE, 30, csr_pos==13);
    print_bold_with_condition(main_win, context, "[Uncommon] "+std::to_string((unsigned long long int) (trader_prices.at("uncommon_material")*0.8))+" Gold each", GREEN, 31, csr_pos==14);
    print_bold_with_condition(main_win, context, "[Rare] "+std::to_string((unsigned long long int) (trader_prices.at("rare_material")*0.8))+" Gold each", BLUE, 32, csr_pos==15);
    print_bold_with_condition(main_win, context, "[Epic] "+std::to_string((unsigned long long int) (trader_prices.at("epic_material")*0.8))+" Gold each", PURPLE, 33, csr_pos==16);
    print_bold_with_condition(main_win, context, "[Legendary] "+std::to_string((unsigned long long int) (trader_prices.at("legendary_material")*0.8))+" Gold each", YELLOW, 34, csr_pos==17);
    clear_and_draw_dialog(main_win, context, "Material Shop:");
}

bool trader_check_if_payment_valid(std::string type, unsigned long long int &payment, unsigned long long int amount){
    if(payment>=(trader_prices.at(type)*amount)){
        payment-=(trader_prices.at(type)*amount);
        return true;
    }
    else{
        return false;
    }
}

bool trader_process_payment(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Merchant &gear_merchant, unsigned int csr_pos){
    unsigned long long int amount=0;
    if(csr_pos<12){
        amount=get_ullint(main_win, context, "Please enter the amount of items you wish to purchase: ");
    }
    else if(csr_pos<18){
        amount=get_ullint(main_win, context, "Please enter the amount of items you wish to sell: ");
    }
    if(amount<=0){
        return false;
    }
    switch(csr_pos){
        case 0:
            if(trader_check_if_payment_valid("common_material", User.gold, amount)){
                User.inv.misc.materials.common+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 1:
            if(trader_check_if_payment_valid("uncommon_material", User.gold, amount)){
                User.inv.misc.materials.uncommon+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 2:
            if(trader_check_if_payment_valid("rare_material", User.gold, amount)){
                User.inv.misc.materials.rare+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 3:
            if(trader_check_if_payment_valid("epic_material", User.gold, amount)){
                User.inv.misc.materials.epic+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 4:
            if(trader_check_if_payment_valid("legendary_material", User.gold, amount)){
                User.inv.misc.materials.legendary+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 5:
            if(trader_check_if_payment_valid("helmet_blueprint", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.blueprint.helmet+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 6:
            if(trader_check_if_payment_valid("chestplate_blueprint", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.blueprint.chestplate+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 7:
            if(trader_check_if_payment_valid("greaves_blueprint", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.blueprint.greaves+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 8:
            if(trader_check_if_payment_valid("boots_blueprint", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.blueprint.boots+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 9:
            if(trader_check_if_payment_valid("shield_blueprint", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.blueprint.shield+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 10:
            if(trader_check_if_payment_valid("weapon_blueprint", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.blueprint.weapon+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 11:
            if(trader_check_if_payment_valid("first_aid_kit", User.inv.misc.cores.crystal_core, amount)){
                User.inv.misc.heal_amount+=amount;
                return true;
            }
            else{
                return false;
            }
            break;
        case 12:
            if(User.inv.misc.cores.crystal_core>=amount){
                User.inv.misc.cores.crystal_core-=amount;
                User.gold+=(trader_prices.at("crystal_core")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 13:
            if(User.inv.misc.materials.common>=amount){
                User.inv.misc.materials.common-=amount;
                User.gold+=(trader_prices.at("common_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 14:
            if(User.inv.misc.materials.uncommon>=amount){
                User.inv.misc.materials.uncommon-=amount;
                User.gold+=(trader_prices.at("uncommon_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 15:
            if(User.inv.misc.materials.rare>=amount){
                User.inv.misc.materials.rare-=amount;
                User.gold+=(trader_prices.at("rare_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 16:
            if(User.inv.misc.materials.epic>=amount){
                User.inv.misc.materials.epic-=amount;
                User.gold+=(trader_prices.at("epic_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        case 17:
            if(User.inv.misc.materials.legendary>=amount){
                User.inv.misc.materials.legendary-=amount;
                User.gold+=(trader_prices.at("legendary_material")*amount)*0.8;
                return true;
            }
            else{
                return false;
            }
        default:
            break;
    }
    return false;
}

void trader_misc_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Merchant &gear_merchant){
    unsigned int csr_pos=0;
    int ch;
    print_trader_misc_menu(main_win, context, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<17){
            csr_pos++;
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        if(ch=='q'){
            return;
        }
        if(ch=='m'){
            show_misc_items(main_win, context, User.inv.misc);
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        if(ch==SDLK_RETURN){
            if(trader_process_payment(main_win, context, User, gear_merchant, csr_pos)){
                print_trader_misc_menu(main_win, context, csr_pos);
                draw_stats(main_win, context, User);
                clear_and_draw_dialog(main_win, context, "[System] Success");
            }
            else{
                clear_and_draw_dialog(main_win, context, "[System] Failure");
            }
        }
    }
}

double calculate_quality_points(const Item &cur){
    double quality_points=rarity_value(cur.rarity)*rarity_value(cur.rarity);
    quality_points+=cur.def/10;
    quality_points+=cur.crit_chance/5;
    quality_points+=cur.crit_dmg/5;
    switch(cur.type){
        case TYPE_HELMET:
            quality_points*=cur.hp/(40*rarity_value(cur.rarity));
            break;
        case TYPE_CHESTPLATE:
            quality_points*=cur.hp/(100*rarity_value(cur.rarity));
            break;
        case TYPE_GREAVES:
            quality_points*=cur.def/(50*rarity_value(cur.rarity));
            break;
        case TYPE_BOOTS:
            quality_points*=cur.def/(30*rarity_value(cur.rarity));
            break;
        case TYPE_SHIELD:
            quality_points*=cur.shield/(100*rarity_value(cur.rarity));
            break;
        case TYPE_WEAPON:
            quality_points*=cur.attk/(200*rarity_value(cur.rarity));
            break;
        default:
            break;
    }
    return quality_points;
}

void print_inventory_with_quality_points_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, double quality_points){
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(*main_win, {0, line+1}, "Equipped: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(cur_item->is_equipped){
        tcod::print(*main_win, {10, line+1}, "true", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(!cur_item->is_equipped){
        tcod::print(*main_win, {10, line+1}, "false", &LIGHT_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    line++;
    tcod::print(*main_win, {0, line+1}, "Quality Points: "+std::to_string(quality_points), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(*main_win);
}

void print_inventory_with_quality_points(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, unsigned int csr_pos, const std::vector<Item *> &items_copy, double quality_points){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_base(main_win, context, 34, items_copy.size(), 0, false, false);
    for(int i=0, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_mysterious_trader_item_description(main_win, context, items_copy[csr_pos], 35, quality_points);
}

void select_inventory_for_exchange(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, const Item &target){

}

void mysterious_trader_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Player &User, Merchant &mysterious_merchant){
    unsigned int page_num=0;
    int csr_pos=0;
    std::vector<Item *> selected_for_trade;
    std::vector<Item> original_copy;
    for(auto i: mysterious_merchant.store){
        original_copy.push_back(i.first);
    }
    std::vector<Item *> items_copy;
    init_copy(original_copy, items_copy);
    draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
    while(true){
        int ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_mysterious_trader_items(main_win, context, items_copy, csr_pos, calculate_quality_points(*items_copy[csr_pos]));
        }
        if(ch=='q'){
            return;
        }
        if(ch==SDLK_RETURN){

        }
    }
}
