#include "headers/main.h"
#include "headers/classes.h"
#include "headers/mode.h"
void draw_inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const NoDelete &perm_config, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0,0}, "Inventory Modifier Selection", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,1}, "Show only [Rarity]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(csr_pos==0){
        if(perm_config.default_show_rarity_type==DEFAULT_SHOW_SELECTION){
            tcod::print(*main_win, {0,2}, "->[All]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,2}, "[All]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==DEFAULT_SHOW_SELECTION){
            tcod::print(*main_win, {0,2}, "->[All]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,2}, "[All]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==1){
        if(perm_config.default_show_rarity_type==RARITY_COMMON){
            tcod::print(*main_win, {0,3}, "->[Common]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,3}, "[Common]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==RARITY_COMMON){
            tcod::print(*main_win, {0,3}, "->[Common]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,3}, "[Common]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==2){
        if(perm_config.default_show_rarity_type==RARITY_UNCOMMON){
            tcod::print(*main_win, {0,4}, "->[Uncommon]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,4}, "[Uncommon]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==RARITY_UNCOMMON){
            tcod::print(*main_win, {0,4}, "->[Uncommon]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,4}, "[Uncommon]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==3){
        if(perm_config.default_show_rarity_type==RARITY_RARE){
            tcod::print(*main_win, {0,5}, "->[Rare]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,5}, "[Rare]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==RARITY_RARE){
            tcod::print(*main_win, {0,5}, "->[Rare]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,5}, "[Rare]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==4){
        if(perm_config.default_show_rarity_type==RARITY_EPIC){
            tcod::print(*main_win, {0,6}, "->[Epic]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,6}, "[Epic]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==RARITY_EPIC){
            tcod::print(*main_win, {0,6}, "->[Epic]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,6}, "[Epic]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==5){
        if(perm_config.default_show_rarity_type==RARITY_LEGENDARY){
            tcod::print(*main_win, {0,7}, "->[Legendary]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,7}, "[Legendary]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==RARITY_LEGENDARY){
            tcod::print(*main_win, {0,7}, "->[Legendary]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,7}, "[Legendary]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==6){
        if(perm_config.default_show_rarity_type==RARITY_ARTIFACT){
            tcod::print(*main_win, {0,8}, "->[Artifact]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,8}, "[Artifact]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_rarity_type==RARITY_LEGENDARY){
            tcod::print(*main_win, {0,8}, "->[Artifact]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,8}, "[Artifact]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    tcod::print(*main_win, {0,9}, "Show only [Type]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(csr_pos==7){
        if(perm_config.default_show_type_type==DEFAULT_SHOW_SELECTION){
            tcod::print(*main_win, {0,10}, "->[All]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,10}, "[All]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==DEFAULT_SHOW_SELECTION){
            tcod::print(*main_win, {0,10}, "->[All]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,10}, "[All]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==8){
        if(perm_config.default_show_type_type==TYPE_HELMET){
            tcod::print(*main_win, {0,11}, "->[Helmet]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,11}, "[Helmet]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==TYPE_HELMET){
            tcod::print(*main_win, {0,11}, "->[Helmet]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,11}, "[Helmet]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==9){
        if(perm_config.default_show_type_type==TYPE_CHESTPLATE){
            tcod::print(*main_win, {0,12}, "->[Chestplate]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,12}, "[Chestplate]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==TYPE_CHESTPLATE){
            tcod::print(*main_win, {0,12}, "->[Chestplate]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,12}, "[Chestplate]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==10){
        if(perm_config.default_show_type_type==TYPE_GREAVES){
            tcod::print(*main_win, {0,13}, "->[Greaves]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,13}, "[Greaves]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==TYPE_GREAVES){
            tcod::print(*main_win, {0,13}, "->[Greaves]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,13}, "[Greaves]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==11){
        if(perm_config.default_show_type_type==TYPE_BOOTS){
            tcod::print(*main_win, {0,14}, "->[Boots]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,14}, "[Boots]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==TYPE_BOOTS){
            tcod::print(*main_win, {0,14}, "->[Boots]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,14}, "[Boots]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==12){
        if(perm_config.default_show_type_type==TYPE_SHIELD){
            tcod::print(*main_win, {0,15}, "->[Shield]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,15}, "[Shield]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==TYPE_SHIELD){
            tcod::print(*main_win, {0,15}, "->[Shield]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,15}, "[Shield]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==13){
        if(perm_config.default_show_type_type==TYPE_WEAPON){
            tcod::print(*main_win, {0,16}, "->[Weapon]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,16}, "[Weapon]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.default_show_type_type==TYPE_WEAPON){
            tcod::print(*main_win, {0,16}, "->[Weapon]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,16}, "[Weapon]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    tcod::print(*main_win, {0,17}, "Show only [Equipped]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    if(csr_pos==14){
        if(!perm_config.only_show_equipped){
            tcod::print(*main_win, {0,18}, "->[All]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,18}, "[All]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(!perm_config.only_show_equipped){
            tcod::print(*main_win, {0,18}, "->[All]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,18}, "[All]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    if(csr_pos==15){
        if(perm_config.only_show_equipped){
            tcod::print(*main_win, {0,19}, "->[Equipped only]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,19}, "[Equipped only]", &BLACK, &WHITE, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    else{
        if(perm_config.only_show_equipped){
            tcod::print(*main_win, {0,19}, "->[Equipped only]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
        else{
            tcod::print(*main_win, {0,19}, "[Equipped only]", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
    context->present(*main_win);
}
void inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config){
    unsigned int csr_pos = 0;
    int ch;
    while(true){
        draw_inventory_modifier_selection(main_win, context, perm_config, csr_pos);
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_DOWN){
            csr_pos++;
        }
        if(ch==SDLK_UP){
            csr_pos--;
        }
    }
}
