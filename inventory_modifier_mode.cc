#include "headers/main.h"
#include "headers/classes.h"
#include "headers/mode.h"
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
}sort_ascending;
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
}sort_descending;
void init_copy(Player &User, std::vector<Item*> &items_copy){
    items_copy.clear();
    for(int i = 0; i<User.inv.item.size(); i++){
        items_copy.push_back(&User.inv.item[i]);
    }
}
void process_copy(Player &User, std::vector<Item*> &items_copy, const NoDelete &perm_config){
    init_copy(User, items_copy);
    if(perm_config.only_show_equipped){ // Only show Equipped
        items_copy.erase(std::remove_if(items_copy.begin(), items_copy.end(), [](const Item *v){return v->is_equipped==false;}), items_copy.end());
    }
    if(perm_config.default_show_rarity_type!=DEFAULT_SHOW_SELECTION){ // Only show certain rarity
        items_copy.erase(std::remove_if(items_copy.begin(), items_copy.end(), [perm_config](const Item *v){return v->rarity!=perm_config.default_show_rarity_type;}), items_copy.end());
    }
    if(perm_config.default_show_type_type!=DEFAULT_SHOW_SELECTION){ // Only show certain types
        items_copy.erase(std::remove_if(items_copy.begin(), items_copy.end(), [perm_config](const Item *v){return v->type!=perm_config.default_show_type_type;}), items_copy.end());
    }
    if(perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_ASCENDING){ // Sort by rarity ascending
        std::sort(items_copy.begin(), items_copy.end(), sort_ascending);
    }
    if(perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_DESCENDING){ // Sort by rarity descending
        std::sort(items_copy.begin(), items_copy.end(), sort_descending);
    }
}
void print_item(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, bool curr_pos_matches_condition, bool curr_config_matches_condition, int line, std::string output, bool is_valid=true){
    if(!is_valid){
        tcod::print(*main_win, {0,line}, "["+output+"]", &GRAY, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        return;
    }
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
bool check_if_rarity_valid(const std::vector<Item*> items_copy, char check){
    for(int i = 0; i<items_copy.size(); i++){
        if(items_copy[i]->rarity==check){
            return true;
        }
    }
    return false;
}
bool check_if_type_valid(const std::vector<Item*> items_copy, char check){
    for(int i = 0; i<items_copy.size(); i++){
        if(items_copy[i]->type==check){
            return true;
        }
    }
    return false;
}
bool check_if_equip_valid(const std::vector<Item*> items_copy){
    for(int i = 0; i<items_copy.size(); i++){
        if(items_copy[i]->is_equipped){
            return true;
        }
    }
    return false;
}
void draw_inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::vector<Item*> items_copy, const NoDelete &perm_config, unsigned int csr_pos){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0,0}, "Inventory Modifier Selection", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    tcod::print(*main_win, {0,1}, "Show only [Rarity]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==0, perm_config.default_show_rarity_type==DEFAULT_SHOW_SELECTION, 2, "All");
    print_item(main_win, context, csr_pos==1, perm_config.default_show_rarity_type==RARITY_COMMON, 3, "Common", check_if_rarity_valid(items_copy, RARITY_COMMON));
    print_item(main_win, context, csr_pos==2, perm_config.default_show_rarity_type==RARITY_UNCOMMON, 4, "Uncommon", check_if_rarity_valid(items_copy, RARITY_UNCOMMON));
    print_item(main_win, context, csr_pos==3, perm_config.default_show_rarity_type==RARITY_RARE, 5, "Rare", check_if_rarity_valid(items_copy, RARITY_RARE));
    print_item(main_win, context, csr_pos==4, perm_config.default_show_rarity_type==RARITY_EPIC, 6, "Epic", check_if_rarity_valid(items_copy, RARITY_EPIC));
    print_item(main_win, context, csr_pos==5, perm_config.default_show_rarity_type==RARITY_LEGENDARY, 7, "Legendary", check_if_rarity_valid(items_copy, RARITY_LEGENDARY));
    print_item(main_win, context, csr_pos==6, perm_config.default_show_rarity_type==RARITY_ARTIFACT, 8, "Artifact", check_if_rarity_valid(items_copy, RARITY_ARTIFACT));
    tcod::print(*main_win, {0,10}, "Show only [Type]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==7, perm_config.default_show_type_type==DEFAULT_SHOW_SELECTION, 11, "All");
    print_item(main_win, context, csr_pos==8, perm_config.default_show_type_type==TYPE_HELMET, 12, "Helmet", check_if_type_valid(items_copy, TYPE_HELMET));
    print_item(main_win, context, csr_pos==9, perm_config.default_show_type_type==TYPE_CHESTPLATE, 13, "Chestplate", check_if_type_valid(items_copy, TYPE_CHESTPLATE));
    print_item(main_win, context, csr_pos==10, perm_config.default_show_type_type==TYPE_GREAVES, 14, "Greaves", check_if_type_valid(items_copy, TYPE_GREAVES));
    print_item(main_win, context, csr_pos==11, perm_config.default_show_type_type==TYPE_BOOTS, 15, "Boots", check_if_type_valid(items_copy, TYPE_BOOTS));
    print_item(main_win, context, csr_pos==12, perm_config.default_show_type_type==TYPE_SHIELD, 16, "Shield", check_if_type_valid(items_copy, TYPE_SHIELD));
    print_item(main_win, context, csr_pos==13, perm_config.default_show_type_type==TYPE_WEAPON, 17, "Weapon", check_if_type_valid(items_copy, TYPE_WEAPON));
    tcod::print(*main_win, {0,19}, "Show only [Equipped]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==14, !perm_config.only_show_equipped, 20, "All");
    print_item(main_win, context, csr_pos==15, perm_config.only_show_equipped, 21, "Equipped Only", check_if_equip_valid(items_copy));
    tcod::print(*main_win, {0,23}, "Sort by [Rarity]:", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    print_item(main_win, context, csr_pos==16, perm_config.default_sort_rarity_method==DEFAULT_SHOW_SELECTION, 24, "None");
    print_item(main_win, context, csr_pos==17, perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_ASCENDING, 25, "Ascending");
    print_item(main_win, context, csr_pos==18, perm_config.default_sort_rarity_method==SORT_TYPE_RARITY_DESCENDING, 26, "Descending");
    print_item(main_win, context, csr_pos==19, perm_config.show_current_item_compared_to_equipped, 28, "Show difference between current and equipped item");
    print_item(main_win, context, csr_pos==20, perm_config.keep_changes_persistent, 30, "Keep Changes Persistent");
    print_item(main_win, context, csr_pos==21, false, 32, "Reset To Defaults");
    print_item(main_win, context, csr_pos==22, false, 34, "Done");
    context->present(*main_win);
}
void change_config(NoDelete &perm_config, unsigned int csr_pos, const std::vector<Item*> items_copy){
    switch(csr_pos){
        case 0:
            perm_config.default_show_rarity_type=DEFAULT_SHOW_SELECTION;
            break;
        case 1:
            perm_config.default_show_rarity_type=RARITY_COMMON;
            break;
        case 2:
            perm_config.default_show_rarity_type=RARITY_UNCOMMON;
            break;
        case 3:
            perm_config.default_show_rarity_type=RARITY_RARE;
            break;
        case 4:
            perm_config.default_show_rarity_type=RARITY_EPIC;
            break;
        case 5:
            perm_config.default_show_rarity_type=RARITY_LEGENDARY;
            break;
        case 6:
            perm_config.default_show_rarity_type=RARITY_ARTIFACT;
            break;
        case 7:
            perm_config.default_show_type_type=DEFAULT_SHOW_SELECTION;
            break;
        case 8:
            perm_config.default_show_type_type=TYPE_HELMET;
            break;
        case 9:
            perm_config.default_show_type_type=TYPE_CHESTPLATE;
            break;
        case 10:
            perm_config.default_show_type_type=TYPE_GREAVES;
            break;
        case 11:
            perm_config.default_show_type_type=TYPE_BOOTS;
            break;
        case 12:
            perm_config.default_show_type_type=TYPE_SHIELD;
            break;
        case 13:
            perm_config.default_show_type_type=TYPE_WEAPON;
            break;
        case 14:
            perm_config.only_show_equipped=false;
            break;
        case 15:
            perm_config.only_show_equipped=true;
            break;
        case 16:
            perm_config.default_sort_rarity_method=DEFAULT_SHOW_SELECTION;
            break;
        case 17:
            perm_config.default_sort_rarity_method=SORT_TYPE_RARITY_ASCENDING;
            break;
        case 18:
            perm_config.default_sort_rarity_method=SORT_TYPE_RARITY_DESCENDING;
            break;
        case 19:
            if(perm_config.show_current_item_compared_to_equipped){
                perm_config.show_current_item_compared_to_equipped=false;
            }
            else{
                perm_config.show_current_item_compared_to_equipped=true;
            }
            break;
        case 20:
            if(perm_config.keep_changes_persistent){
                perm_config.keep_changes_persistent=false;
            }
            else{
                perm_config.keep_changes_persistent=true;
            }
            break;
        default:
            break;
    }
}
bool check_if_csr_move_valid(const unsigned int &csr_pos, const std::vector<Item*> items_copy){
    switch(csr_pos){
        case 1:
            if(check_if_rarity_valid(items_copy, RARITY_COMMON)){
                return true;
            }
            break;
        case 2:
            if(check_if_rarity_valid(items_copy, RARITY_UNCOMMON)){
                return true;
            }
            break;
        case 3:
            if(check_if_rarity_valid(items_copy, RARITY_RARE)){
                return true;
            }
            break;
        case 4:
            if(check_if_rarity_valid(items_copy, RARITY_EPIC)){
                return true;
            }
            break;
        case 5:
            if(check_if_rarity_valid(items_copy, RARITY_LEGENDARY)){
                return true;
            }
            break;
        case 6:
            if(check_if_rarity_valid(items_copy, RARITY_ARTIFACT)){
                return true;
            }
            break;
        case 8:
            if(check_if_type_valid(items_copy, TYPE_HELMET)){
                return true;
            }
            break;
        case 9:
            if(check_if_type_valid(items_copy, TYPE_CHESTPLATE)){
                return true;
            }
            break;
        case 10:
            if(check_if_type_valid(items_copy, TYPE_GREAVES)){
                return true;
            }
            break;
        case 11:
            if(check_if_type_valid(items_copy, TYPE_BOOTS)){
                return true;
            }
            break;
        case 12:
            if(check_if_type_valid(items_copy, TYPE_SHIELD)){
                return true;
            }
            break;
        case 13:
            if(check_if_type_valid(items_copy, TYPE_WEAPON)){
                return true;
            }
            break;
        case 15:
            if(check_if_equip_valid(items_copy)){
                return true;
            }
            break;
        default:
            return true;
    }
    return false;
}
void repeat_down_until_valid_move(unsigned int &csr_pos, const std::vector<Item*> items_copy){
    csr_pos++;
    if(!check_if_csr_move_valid(csr_pos, items_copy)){
        repeat_down_until_valid_move(csr_pos, items_copy);
    }
}
void repeat_up_until_valid_move(unsigned int &csr_pos, const std::vector<Item*> items_copy){
    csr_pos--;
    if(!check_if_csr_move_valid(csr_pos, items_copy)){
        repeat_down_until_valid_move(csr_pos, items_copy);
    }
}
void inventory_modifier_selection(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, NoDelete &perm_config, const std::vector<Item*> items_copy){
    unsigned int csr_pos = 0;
    int ch;
    while(true){
        draw_inventory_modifier_selection(main_win, context, items_copy, perm_config, csr_pos);
        ch=SDL_getch(main_win, context);
        if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<22){
            repeat_down_until_valid_move(csr_pos, items_copy);
        }
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            repeat_up_until_valid_move(csr_pos, items_copy);
        }
        if(ch==SDLK_RETURN){
            if(csr_pos==21){
                perm_config.default_show_type_type = DEFAULT_SHOW_SELECTION;
                perm_config.default_show_rarity_type = DEFAULT_SHOW_SELECTION;
                perm_config.default_sort_rarity_method = DEFAULT_SHOW_SELECTION;
                perm_config.only_show_equipped = false;
            }
            if(csr_pos==22){
                return;
            }
            change_config(perm_config, csr_pos, items_copy);
        }
        if(ch=='q'){
            return;
        }
    }
}
