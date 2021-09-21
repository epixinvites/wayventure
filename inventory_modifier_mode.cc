#include "headers/main.h"
#include "headers/classes.h"
#include "headers/mode.h"
void print_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, bool curr_pos_matches_condition, bool curr_config_matches_condition, int line, std::string output){
    if(curr_pos_matches_condition){
        if(curr_config_matches_condition){
            tcod::print(*main_win, {0,line}, "->["+output+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,line}, "["+output+"]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(curr_config_matches_condition){
            tcod::print(*main_win, {0,line}, "->["+output+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,line}, "["+output+"]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
}
void draw_inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const NoDelete &perm_config, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0,0}, "Inventory Modifier Selection", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,1}, "Show only [Rarity]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==0, perm_config.default_show_rarity_type==DEFAULT_SHOW_SELECTION, 2, "All");
    print_item(main_win, context, csr_pos==1, perm_config.default_show_rarity_type==RARITY_COMMON, 3, "Common");
    print_item(main_win, context, csr_pos==2, perm_config.default_show_rarity_type==RARITY_UNCOMMON, 4, "Uncommon");
    print_item(main_win, context, csr_pos==3, perm_config.default_show_rarity_type==RARITY_RARE, 5, "Rare");
    print_item(main_win, context, csr_pos==4, perm_config.default_show_rarity_type==RARITY_EPIC, 6, "Epic");
    print_item(main_win, context, csr_pos==5, perm_config.default_show_rarity_type==RARITY_LEGENDARY, 7, "Legendary");
    print_item(main_win, context, csr_pos==6, perm_config.default_show_rarity_type==RARITY_ARTIFACT, 8, "Artifact");
    tcod::print(*main_win, {0,10}, "Show only [Type]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==7, perm_config.default_show_type_type==DEFAULT_SHOW_SELECTION, 11, "All");
    print_item(main_win, context, csr_pos==8, perm_config.default_show_type_type==TYPE_HELMET, 12, "Helmet");
    print_item(main_win, context, csr_pos==9, perm_config.default_show_type_type==TYPE_CHESTPLATE, 13, "Chestplate");
    print_item(main_win, context, csr_pos==10, perm_config.default_show_type_type==TYPE_GREAVES, 14, "Greaves");
    print_item(main_win, context, csr_pos==11, perm_config.default_show_type_type==TYPE_BOOTS, 15, "Boots");
    print_item(main_win, context, csr_pos==12, perm_config.default_show_type_type==TYPE_SHIELD, 16, "Shield");
    print_item(main_win, context, csr_pos==13, perm_config.default_show_type_type==TYPE_WEAPON, 17, "Weapon");
    tcod::print(*main_win, {0,19}, "Show only [Equipped]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==14, !perm_config.only_show_equipped, 20, "All");
    print_item(main_win, context, csr_pos==15, perm_config.only_show_equipped, 21, "Equipped Only");
    tcod::print(*main_win, {0,23}, "Sort by [Rarity]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==16, perm_config.default_sort_rarity_method==DEFAULT_SHOW_SELECTION, 24, "None");
    print_item(main_win, context, csr_pos==17, perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_ASCENDING, 25, "Ascending");
    print_item(main_win, context, csr_pos==18, perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_DESCENDING, 26, "Descending");
    tcod::print(*main_win, {0,28}, "Show difference between current item and equipped item:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==19, perm_config.show_current_item_compared_to_equipped, 29, "True");
    print_item(main_win, context, csr_pos==20, !perm_config.show_current_item_compared_to_equipped, 30, "False");
    context->present(*main_win);
}
void inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config){
    unsigned int csr_pos = 0;
    int ch;
    while(true){
        draw_inventory_modifier_selection(main_win, context, perm_config, csr_pos);
        ch=SDL_getch(main_win, context);
        if(ch=='s'||ch==SDLK_DOWN){
            csr_pos++;
        }
        if(ch=='w'||ch==SDLK_UP){
            csr_pos--;
        }
        if(ch=='q'){
            return;
        }
    }
}
