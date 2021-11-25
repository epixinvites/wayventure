// Wayventure, a complex old-school dungeon adventure.
// Copyright (C) 2021 Zhi Ping Ooi
//
// This file is part of Wayventure.
//
// Wayventure is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iostream>
#include <random>
#include <thread>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <atomic>
#include "headers/main.h"
#include "headers/classes.h"
#include "headers/mode.h"
#include "headers/dungeon.h"
#include "headers/threads.h"
#include "headers/keymap.h"
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/chrono.hpp>

int SDL_getch(tcod::Console &main_win, tcod::ContextPtr &context){
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_Event event;
    SDL_WaitEvent(nullptr);
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_WINDOWEVENT){
            if(event.window.event==SDL_WINDOWEVENT_EXPOSED){
                context->present(main_win);
            }
        }
        if(event.type==SDL_QUIT){
            TCOD_quit();
            std::exit(1);
        }
        if(event.type==SDL_KEYDOWN){
            if(event.key.keysym.mod & KMOD_SHIFT){
                try{
                    return us_qwerty_map.at(event.key.keysym.sym);
                }
                catch(const std::out_of_range){
                    return event.key.keysym.sym;
                }
            }
            else if(event.key.keysym.mod & KMOD_CTRL){
                if(event.key.keysym.sym=='r'){
                    SDL_SetWindowSize(context->get_sdl_window(), main_win.get_width()*tileset.get_tile_width(), main_win.get_height()*tileset.get_tile_height());
                    continue;
                }
            }
            else if(!(event.key.keysym.mod & modkeys)){
                return event.key.keysym.sym;
            }
        }
    }
    return 0;
}

std::pair<int, int> SDL_getch_ex(tcod::Console &main_win, tcod::ContextPtr &context){
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_Event event;
    SDL_WaitEvent(nullptr);
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_WINDOWEVENT){
            if(event.window.event==SDL_WINDOWEVENT_EXPOSED){
                context->present(main_win);
            }
        }
        if(event.type==SDL_QUIT){
            TCOD_quit();
            std::exit(1);
        }
        if(event.type==SDL_KEYDOWN){
            if(event.key.keysym.mod & KMOD_CTRL){
                if(event.key.keysym.sym=='r'){
                    SDL_SetWindowSize(context->get_sdl_window(), main_win.get_width()*tileset.get_tile_width(), main_win.get_height()*tileset.get_tile_height());
                    continue;
                }
            }
            else{
                return {event.key.keysym.mod, event.key.keysym.sym};
            }
        }
    }
    return {0, 0};
}

bool SDL_getch_y_or_n(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue){
    clear_and_draw_dialog(main_win, context, dialogue);
    int ch=SDL_getch(main_win, context);
    while(!(ch>0&&ch<128)){
        ch=SDL_getch(main_win, context);
    }
    if(ch=='y'){
        return true;
    }
    else{
        return false;
    }
}

void SDL_wclear_dialog_bar(tcod::Console &main_win, tcod::ContextPtr &context){
    tcod::print(main_win, {0, 0}, empty_line, WHITE, BLACK);
//    context->present(main_win);
}

void SDL_wclear_stats_bar(tcod::Console &main_win, tcod::ContextPtr &context){
    for(int i=51; i<56; i++){
        tcod::print(main_win, {0, i}, empty_line, WHITE, BLACK);
    }
//    context->present(main_win);
}

void SDL_wclear_main_win(tcod::Console &main_win, tcod::ContextPtr &context){
    for(int i=1; i<51; i++){
        for(int j=0; j<80; j++){
            tcod::print(main_win, {0, i}, empty_line, WHITE, BLACK);
        }
    }
//    context->present(main_win);
}

std::string get_string(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue, std::string original){
    std::string input;
    clear_and_draw_dialog(main_win, context, dialogue);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_RETURN&&input.length()>0){ // KEY_ENTER
            return input;
        }
        else if(ch==SDLK_BACKSPACE){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        else if(ch==SDLK_ESCAPE){ // KEY_ESC
            return original;
        }
        else if((ch>0&&ch<128)&&input.length()<30){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss << dialogue << input << '|';
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

unsigned int get_int(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue){
    std::string input;
    clear_and_draw_dialog(main_win, context, dialogue);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_RETURN&&input.length()>0){ // KEY_ENTER
            return std::stoi(input);
        }
        else if(ch==SDLK_BACKSPACE){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        else if(ch==SDLK_ESCAPE||ch=='q'){ // KEY_ESC
            return 0;
        }
        else if((ch>0&&ch<128)&&input.length()<8&&isdigit(ch)){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss << dialogue << input << '|';
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

long long int get_llint(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue){
    std::string input;
    clear_and_draw_dialog(main_win, context, dialogue);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_RETURN&&input.length()>0){ // KEY_ENTER
            return std::stoll(input);
        }
        else if(ch==SDLK_BACKSPACE){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        else if(ch==SDLK_ESCAPE||ch=='q'){ // KEY_ESC
            return 0;
        }
        else if((ch>0&&ch<128)&&input.length()<18&&isdigit(ch)){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss << dialogue << input << '|';
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

unsigned long long int get_ullint(tcod::Console &main_win, tcod::ContextPtr &context, const std::string &dialogue){
    std::string input;
    clear_and_draw_dialog(main_win, context, dialogue);
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch==SDLK_RETURN&&input.length()>0){ // KEY_ENTER
            return std::stoull(input);
        }
        else if(ch==SDLK_BACKSPACE){ // KEY_BACKSPACE
            if(input.length()>0){
                input.pop_back();
            }
        }
        else if(ch==SDLK_ESCAPE||ch=='q'){ // KEY_ESC
            return 0;
        }
        else if((ch>0&&ch<128)&&input.length()<19&&isdigit(ch)){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss << dialogue << input << '|';
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

void end_program(int sig){
    if(sig==0){
        std::cout << "Nya~" << std::endl;
        std::ifstream cosmos_ascii("src/res/cosmos.txt");
        std::string line;
        while(std::getline(cosmos_ascii, line)){
            std::cout << line << std::endl;
        }
        return;
    }
    if(sig==1){
        std::ifstream tombstone_file("src/res/tombstone.txt");
        std::string line;
        while(std::getline(tombstone_file, line)){
            std::cout << line << std::endl;
        }
        return;
    }
    if(sig==2){
        std::cout << "*Grumbling* You died of hunger... You regret being on a diet while fighting monsters. (Seriously, dieting exccesively is bad for your health)" << std::endl;
        return;
    }
    if(sig==3){
        std::cout << "*Cough* *Cough* You died of thirst... Next time, remember to join the HydroHomies. (Remember to drink water bro)" << std::endl;
    }
}

void end_program(int sig, const std::string &error){
    if(sig==-1){
        std::cout << "Bug vs Programmer - 1:0" << std::endl;
        std::cerr << error << std::endl;
        return;
    }
}

void init_data(Dungeon &dungeon_data, Player &user, No_Delete &perm_config){
    std::string version_check;
    if(username.length()>30){
        throw std::runtime_error("Nope. Nopenopenopenope. You didn't follow my instructions.");
    }
    std::ifstream ifile("save/user.save", std::ios::binary);
    if(std::filesystem::exists(std::filesystem::path("save/user.save"))){
        cereal::PortableBinaryInputArchive retrieve(ifile);
        retrieve(dungeon_data, user, perm_config, version_check);
        if(save_file_version!=version_check){
            throw std::runtime_error("Versions of save files do not match. Aborting. Do not attempt to edit the save files, lost of data will be expected.");
        }
        user.init();
        // Insert data corruption checks
        std::filesystem::remove("save/user.save.1");
        std::filesystem::copy("save/user.save", "save/user.save.1");
    }
    else{
        dungeon_data.init_rooms();
        user.inv.misc.heal_amount=10;
        std::ofstream ofile("save/user.save", std::ios::trunc | std::ios::binary);
        cereal::PortableBinaryOutputArchive archive(ofile);
        archive(dungeon_data, user, perm_config, save_file_version);
    }
}

void save_data(const Dungeon &dungeon_data, Player &user, const No_Delete &perm_config){
    std::ofstream ofile("save/user.save", std::ios::trunc | std::ios::binary);
    cereal::PortableBinaryOutputArchive archive(ofile);
    user.uninitialize_stats();
    archive(dungeon_data, user, perm_config, save_file_version);
}

void print_start_menu(tcod::Console &main_win, tcod::ContextPtr &context, const std::vector<std::string> &logo, unsigned int csr_pos){
    main_win.clear({0x20, WHITE, BLACK});
    int win_iterator=10;
    for(const auto &i:logo){
        tcod::print(main_win, {9, win_iterator}, i, WHITE, BLACK);
        win_iterator++;
    }
    tcod::print(main_win, {40, 25}, "A complex dungeon crawler with endless possibilities...", WHITE, BLACK, TCOD_CENTER, TCOD_BKGND_SET);
    tcod::print(main_win, {40, 33}, "-Press any key to continue-", WHITE, BLACK, TCOD_CENTER, TCOD_BKGND_SET);
    print_bold_with_condition_ex(main_win, context, "[Load Savefile]", WHITE, BLACK, 40, 38, TCOD_CENTER, csr_pos==0);
    print_bold_with_condition_ex(main_win, context, "[Return]", WHITE, BLACK, 40, 40, TCOD_CENTER, csr_pos==1);
    print_bold_with_condition_ex(main_win, context, "[Create new Savefile] (WIP)", WHITE, BLACK, 40, 42, TCOD_CENTER, csr_pos==2);
    context->present(main_win);
}

void starting_screen(tcod::Console &main_win, tcod::ContextPtr &context, Player &user, Dungeon &dungeon_data, No_Delete &perm_config){
    std::ifstream ascii_wayfarer("src/res/wayfarer.txt");
    if(!ascii_wayfarer){
        end_program(-1, "wayfarer.txt not found!");
        return;
    }
    std::vector<std::string> logo;
    std::string line;
    while(std::getline(ascii_wayfarer, line)){
        logo.push_back(line);
    }
    ascii_wayfarer.close();
    int ch;
    unsigned int csr_pos=0;
    print_start_menu(main_win, context, logo, csr_pos);
    while(true){
        ch=SDL_getch(main_win, context);
        if((ch=='w'||ch==SDLK_UP)&&csr_pos>0){
            csr_pos--;
            print_start_menu(main_win, context, logo, csr_pos);
        }
        else if((ch=='s'||ch==SDLK_DOWN)&&csr_pos<2){
            csr_pos++;
            print_start_menu(main_win, context, logo, csr_pos);
        }
        else if(ch==SDLK_RETURN){
            if(csr_pos==0){
                init_data(dungeon_data, user, perm_config);
                return;
            }
            else if(csr_pos==1){
                std::exit(1);
            }
        }
    }
}

void init(){
    // Variable initialization
    Player user;
    No_Delete perm_config;
    Dungeon dungeon_data;
    Thread_Flags thread_flags;
    // libtcod initialization
    tcod::Console main_win=tcod::Console(80, 56);
    TCOD_ContextParams params{};
    params.tcod_version=TCOD_COMPILEDVERSION;
    params.columns=main_win.get_width();
    params.rows=main_win.get_height();
    params.window_title="Wayventure_Client";
    params.sdl_window_flags=SDL_WINDOW_RESIZABLE;
    params.vsync=true;
    params.renderer_type=TCOD_RENDERER_SDL2;
    params.tileset=tileset.get();
    tcod::ContextPtr context=tcod::new_context(params);
    // Print starting screen
    starting_screen(main_win, context, user, dungeon_data, perm_config);
    // Start threads
    std::thread refresh_timer(job_thread, std::ref(dungeon_data.npc.miner), std::ref(dungeon_data.npc.archaeologist), std::ref(thread_flags.terminate));
    std::thread lootbox_refresh_thread (refresh_lootbox, std::ref(thread_flags.terminate), std::ref(thread_flags.main_dungeon_require_update), std::ref(dungeon_data));
    refresh_timer.detach();
    lootbox_refresh_thread.detach();
    // Start main dungeon
    while(!thread_flags.get_flag(thread_flags.terminate)){
        if(dungeon_data.current>=Level{1,1,1}&&dungeon_data.current<=Level{DUNGEON_LEVEL_MAX, DUNGEON_X_MAX, DUNGEON_Y_MAX}){
            main_dungeon(main_win, context, dungeon_data, user, perm_config, thread_flags);
        }
        else if(dungeon_data.current==Level{0, DUNGEON_X_MAX, DUNGEON_Y_MAX}){
            town_mode(main_win, context, user, dungeon_data, perm_config, thread_flags);
        }
    }
    // Terminate libtcod
    TCOD_quit();
    // Save data
    save_data(dungeon_data, user, perm_config);
}

int main(int argc, char *argv[]){
    init();
    return 0;
}
