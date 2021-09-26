#include "headers/main.h"
#include "headers/mode.h"
#include "headers/classes.h"
#include "headers/draw.h"
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
                chest.gear_storage.push_back(*items_copy[csr_pos+page_num*30]);
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
