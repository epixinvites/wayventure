#include "headers/main.h"
#include "headers/mode.h"
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"
int calculate_price(const Item cur, int relationship, bool player_sell){
    int price = 100*rarity_value(cur.rarity)*rarity_value(cur.rarity);
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
    tcod::print(*main_win, {0,line+1}, "Name: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {6,line+1}, cur_item->name, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,line+2}, "Rarity: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->rarity){
        case RARITY_COMMON:
            tcod::print(*main_win, {8,line+2}, "Common", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_UNCOMMON:
            tcod::print(*main_win, {8,line+2}, "Uncommon", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_RARE:
            tcod::print(*main_win, {8,line+2}, "Rare", &BLUE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_EPIC:
            tcod::print(*main_win, {8,line+2}, "Epic", &PURPLE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_LEGENDARY:
            tcod::print(*main_win, {8,line+2}, "Legendary", &YELLOW, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case RARITY_ARTIFACT:
            tcod::print(*main_win, {8,line+2}, "Artifact", &DARK_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {8,line+2}, "-ERROR-", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
    }
    line+=2;
    tcod::print(*main_win, {0,line+1}, "Type: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    switch(cur_item->type){
        case TYPE_HELMET:
            tcod::print(*main_win, {6,line+1}, "Helmet", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_CHESTPLATE:
            tcod::print(*main_win, {6,line+1}, "Chestplate", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_GREAVES:
            tcod::print(*main_win, {6,line+1}, "Greaves", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_BOOTS:
            tcod::print(*main_win, {6,line+1}, "Boots", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_SHIELD:
            tcod::print(*main_win, {6,line+1}, "Shield", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        case TYPE_WEAPON:
            tcod::print(*main_win, {6,line+1}, "Weapon", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            break;
        default:
            tcod::print(*main_win, {6,line+1}, "-ERROR-", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            return;
    }
    line++;
}
void print_item_description_2(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int &line){
    std::stringstream ss;
    tcod::print(*main_win, {0,line+1}, ("Durability: "+std::to_string(cur_item->durability)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    if(cur_item->uses>0){
        tcod::print(*main_win, {0,line+1}, ("Uses: "+std::to_string(cur_item->uses)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->calibration>0){
        tcod::print(*main_win, {0,line+1}, ("Calibration Level: "+std::to_string(cur_item->calibration)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->enhancement>0){
        tcod::print(*main_win, {0,line+1}, ("Enhancement: "+std::to_string(cur_item->enhancement)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->hp>0){
        tcod::print(*main_win, {0,line+1}, ("Health: "+std::to_string(cur_item->hp)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->attk>0){
        tcod::print(*main_win, {0,line+1}, ("Damage: "+std::to_string(cur_item->attk)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->def>0){
        tcod::print(*main_win, {0,line+1}, ("Defence: "+std::to_string(cur_item->def)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->shield>0){
        tcod::print(*main_win, {0,line+1}, ("Shield: "+std::to_string(cur_item->shield)), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_chance>0){
        tcod::print(*main_win, {0,line+1}, ("Critical Chance: "+std::to_string(cur_item->crit_chance)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    if(cur_item->crit_dmg>0){
        tcod::print(*main_win, {0,line+1}, ("Critical Damage: "+std::to_string(cur_item->crit_dmg)+"%"), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
}
void print_trader_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price, bool is_locked){
    if(is_locked){
        tcod::print(*main_win, {0,line+1}, "[Locked]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(*main_win, {0,line+1}, "Price: "+std::to_string(price), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(*main_win);
}
void print_sell_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price){
    print_item_description_1(main_win, context, cur_item, line);
    tcod::print(*main_win, {0,line+1}, "Equipped: ", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(cur_item->is_equipped){
        tcod::print(*main_win, {10,line+1}, "true", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(!cur_item->is_equipped){
        tcod::print(*main_win, {10,line+1}, "false", &LIGHT_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    line++;
    tcod::print(*main_win, {0,line+1}, "Price: "+std::to_string(price), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
    print_item_description_2(main_win, context, cur_item, line);
    context->present(*main_win);
}
void draw_trader_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item*> items_copy, unsigned int page_num, unsigned int csr_pos, int price, bool is_locked=false){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, User);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator, csr_pos==iterator);
    }
    print_trader_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price, is_locked);
}
void draw_sell_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item*> items_copy, unsigned int page_num, unsigned int csr_pos, int price){
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
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<15){
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
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<15){
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
    std::vector<Item*> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_called = false;
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
                chest.gear_storage.insert(chest.gear_storage.begin(),*items_copy[csr_pos+page_num*30]);
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
    std::vector<Item*> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_called = false;
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
            is_modifier_called = true;
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
    for(auto i:gear_merchant.store){
        original_copy.push_back(i.first);
    }
    std::vector<Item*> items_copy;
    init_copy(original_copy, items_copy);
    draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation));
    while(true){
        int ch=SDL_getch(main_win, context);
        if(ch=='q'){
            return;
        }
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            tcod::print(*main_win, {0,39}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            tcod::print(*main_win, {0,39}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
            draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
        }
        if(ch=='L'){
            if(!gear_merchant.store[csr_pos+page_num*30].second){
                gear_merchant.store[csr_pos+page_num*30].second=true;
                draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
                clear_and_draw_dialog(main_win, context, "Item Locked!");
            }
            else{
                gear_merchant.store[csr_pos+page_num*30].second=false;
                draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
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
                unsigned int amount = calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation);
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
                    for(auto i:gear_merchant.store){
                        original_copy.push_back(i.first);
                    }
                    init_copy(original_copy, items_copy);
                    draw_trader_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(original_copy[csr_pos+page_num*30],gear_merchant.relation), gear_merchant.store[csr_pos+page_num*30].second);
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
    std::vector<Item*> items_copy;
    unsigned int page_num=0;
    int csr_pos=0;
    bool is_modifier_active = false;
    init_copy(User.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(User.inv.item, items_copy, perm_config);
    }
    draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(User.inv.item[csr_pos+page_num*30],gear_merchant.relation,true));
    while(true){
        int ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&((csr_pos+page_num*30)<items_copy.size()-1&&csr_pos<29)){
            csr_pos++;
            draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true));
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true));
        }
        if(ch=='a'||ch==SDLK_LEFT){
            if(page_num>0){
                page_num--;
                csr_pos=0;
                draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true));
            }
        }
        if(ch=='d'||ch==SDLK_RIGHT){
            if((page_num+1)*30<items_copy.size()){
                page_num++;
                csr_pos=0;
                draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true));
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
                User.gold+=calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true);
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
                draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true));
            }
        }
        if(ch=='q'){
            return;
        }
        if(ch=='-'){ // Inventory Modifier
            is_modifier_active = true;
            inventory_modifier_selection(main_win, context, perm_config, User.inv.item, items_copy);
            process_copy(User.inv.item, items_copy, perm_config);
            draw_sell_items(main_win, context, User, items_copy, page_num, csr_pos, calculate_price(*items_copy[csr_pos+page_num*30],gear_merchant.relation,true));
        }
    }
}
void print_trader_misc_menu(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    tcod::print(*main_win, {0,1}, "Materials:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[Common]", WHITE, 2, csr_pos==0);
    print_bold_with_condition(main_win, context, "[Uncommon]", GREEN, 3, csr_pos==1);
    print_bold_with_condition(main_win, context, "[Rare]", BLUE, 4, csr_pos==2);
    print_bold_with_condition(main_win, context, "[Epic]", PURPLE, 5, csr_pos==3);
    print_bold_with_condition(main_win, context, "[Legendary]", YELLOW, 6, csr_pos==4);
    tcod::print(*main_win, {0,8}, "Blueprints:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[Helmet]", WHITE, 9, csr_pos==5);
    print_bold_with_condition(main_win, context, "[Chestplate]", WHITE, 10, csr_pos==6);
    print_bold_with_condition(main_win, context, "[Greaves]", WHITE, 11, csr_pos==7);
    print_bold_with_condition(main_win, context, "[Boots]", WHITE, 12, csr_pos==8);
    print_bold_with_condition(main_win, context, "[Shield]", WHITE, 13, csr_pos==9);
    print_bold_with_condition(main_win, context, "[Weapon]", WHITE, 14, csr_pos==10);
    tcod::print(*main_win, {0,16}, "Others:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_bold_with_condition(main_win, context, "[First-Aid Kits]", WHITE, 17, csr_pos==11);
    clear_and_draw_dialog(main_win, context, "Material Shop:");
}
bool trader_check_if_payment_valid(unsigned long long int &gold, unsigned long long int amount){
    if(original>=amount){
        original-=amount;
        return true;
    }
    else{
        return false;
    }
}
bool trader_process_payment(Player &User, Merchant &gear_merchant, unsigned int csr_pos, unsigned long long int amount){
    switch(csr_pos){
        case 0:
            return trader_check_if_payment_valid(User.inv.misc.materials.common, amount);
        case 1:
            return trader_check_if_payment_valid(User.inv.misc.materials.uncommon, amount);
        case 2:
            return trader_check_if_payment_valid(User.inv.misc.materials.rare, amount);
    }
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
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<11){
            csr_pos++;
            print_trader_misc_menu(main_win, context, csr_pos);
        }
        if(ch=='q'){
            return;
        }
        if(ch==SDLK_RETURN){
            unsigned long long int amount = get_ullint(main_win, context, "Please enter the amount of items you wish to purchase: ");
            if(amount>0&&trader_process_payment(User, gear_merchant, csr_pos, amount)){

            }
        }
    }
}
