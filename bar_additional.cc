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
        return price*0.8;
    }
    return price;
}
void print_trader_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price, bool is_locked){
    if(is_locked){
        tcod::print(*main_win, {0,line+1}, "[Locked]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        line++;
    }
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
    tcod::print(*main_win, {0,line+1}, "Price: "+std::to_string(price), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
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
    context->present(*main_win);
}
void print_sell_item_description(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Item *cur_item, int line, int price){
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
    if(cur_item->is_equipped){
        tcod::print(*main_win, {10,line+1}, "true", &GREEN, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    if(!cur_item->is_equipped){
        tcod::print(*main_win, {10,line+1}, "false", &LIGHT_RED, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
    line++;
    tcod::print(*main_win, {0,line+1}, "Price: "+std::to_string(price), &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    line++;
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
    context->present(*main_win);
}
void draw_trader_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item*> items_copy, unsigned int page_num, unsigned int csr_pos, int price, bool is_locked=false){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, User);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator);
    }
    print_bold_item(main_win, context, items_copy[page_num*30+csr_pos], csr_pos);
    print_trader_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price, is_locked);
}
void draw_sell_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const Player &User, const std::vector<Item*> items_copy, unsigned int page_num, unsigned int csr_pos, int price){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_stats_bar(main_win, context);
    draw_stats(main_win, context, User);
    draw_base(main_win, context, 34, items_copy.size(), page_num, false, false);
    for(int i=page_num*30, iterator=0; i<items_copy.size()&&iterator<30; i++, iterator++){
        print_item(main_win, context, items_copy[i], iterator);
    }
    print_bold_item(main_win, context, items_copy[page_num*30+csr_pos], csr_pos);
    print_sell_item_description(main_win, context, items_copy[page_num*30+csr_pos], 35, price);
}

void store_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User){
    unsigned int csr_pos=0;
    clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
    while(true){
        print_misc_item(main_win, context, User, csr_pos);
        int ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<18){
            clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
            if(csr_pos==2||csr_pos==10){
                csr_pos+=3;
            }
            else{
                csr_pos++;
            }
        }
        if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "[Inventory] Miscallaneous items:");
            if(csr_pos==5||csr_pos==13){
                csr_pos-=3;
            }
            else{
                csr_pos--;
            }
        }
        if(ch=='S'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be stored: ");
            if(remove_misc_item(User, csr_pos, quantity)){
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
void retrieve_misc_items(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, Miscellaneous &User){
    unsigned int csr_pos=0;
    clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
    while(true){
        print_misc_item(main_win, context, User, csr_pos);
        int ch=SDL_getch(main_win, context);
        if((ch==SDLK_DOWN||ch=='s')&&csr_pos<18){
            clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
            if(csr_pos==2||csr_pos==10){
                csr_pos+=3;
            }
            else{
                csr_pos++;
            }
        }
        if((ch==SDLK_UP||ch=='w')&&csr_pos>0){
            clear_and_draw_dialog(main_win, context, "[Storage] Miscallaneous items:");
            if(csr_pos==5||csr_pos==13){
                csr_pos-=3;
            }
            else{
                csr_pos--;
            }
        }
        if(ch=='S'){
            unsigned int quantity=get_int(main_win, context, "([ESC] to cancel) Quantity to be retrieved: ");
            if(remove_misc_item(User, csr_pos, quantity)){
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
                if(csr_pos>0){
                    csr_pos--;
                }
                if(User.inv.item.empty()){
                    return;
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
            if(csr_pos>0){
                csr_pos--;
            }
            if(chest.gear_storage.empty()){
                return;
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
                    User.add_item(original_copy[csr_pos+page_num*30]);
                    gear_merchant.store.erase(gear_merchant.store.begin()+(csr_pos+page_num*30));
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
    init_copy(User.inv.item, items_copy);
    if(perm_config.keep_changes_persistent){
        process_copy(User.inv.item, items_copy, perm_config);
    }
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

        }
        if(ch=='q'){
            return;
        }
        if(ch=='-'){ // Inventory Modifier
            inventory_modifier_selection(main_win, context, perm_config, User.inv.item, items_copy);
            process_copy(User.inv.item, items_copy, perm_config);
            draw_inventory(main_win, context, User, items_copy, page_num, csr_pos);
        }
    }
}
