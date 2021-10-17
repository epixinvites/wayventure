#include <iostream>
#include <random>
#include <thread>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include "headers/main.h"
#include "headers/classes.h"
#include "headers/draw.h"
#include "headers/generate.h"
#include "headers/mode.h"
#include "headers/dungeon.h"
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/chrono.hpp>

int SDL_getch(tcod::ConsolePtr &main_win, tcod::ContextPtr &context){
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_Event event;
    SDL_WaitEvent(nullptr);
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_WINDOWEVENT){
            if(event.window.event==SDL_WINDOWEVENT_EXPOSED){
                context->present(*main_win);
            }
        }
        if(event.type==SDL_QUIT){
            std::exit(1);
        }
        if(event.type==SDL_KEYDOWN){
            if(event.key.keysym.mod & KMOD_SHIFT){
                return toupper(event.key.keysym.sym);
            }
            if(event.key.keysym.mod & KMOD_CTRL){
                if(event.key.keysym.sym=='r'){
                    SDL_SetWindowSize(context->get_sdl_window(), main_win->w*tileset->tile_width, main_win->h*tileset->tile_height);
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

std::pair<int, int> SDL_getch_ex(tcod::ConsolePtr &main_win, tcod::ContextPtr &context){
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_Event event;
    SDL_WaitEvent(nullptr);
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_WINDOWEVENT){
            if(event.window.event==SDL_WINDOWEVENT_EXPOSED){
                context->present(*main_win);
            }
        }
        if(event.type==SDL_QUIT){
            std::exit(1);
        }
        if(event.type==SDL_KEYDOWN){
            if(event.key.keysym.mod & KMOD_CTRL){
                if(event.key.keysym.sym=='r'){
                    SDL_SetWindowSize(context->get_sdl_window(), main_win->w*tileset->tile_width, main_win->h*tileset->tile_height);
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

bool SDL_getch_y_or_n(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue){
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

void SDL_wclear_dialog_bar(tcod::ConsolePtr &main_win, tcod::ContextPtr &context){
    tcod::print(*main_win, {0, 0}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
//    context->present(*main_win);
}

void SDL_wclear_stats_bar(tcod::ConsolePtr &main_win, tcod::ContextPtr &context){
    for(int i=51; i<56; i++){
        tcod::print(*main_win, {0, i}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    }
//    context->present(*main_win);
}

void SDL_wclear_main_win(tcod::ConsolePtr &main_win, tcod::ContextPtr &context){
    for(int i=1; i<51; i++){
        for(int j=0; j<80; j++){
            tcod::print(*main_win, {0, i}, empty_line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        }
    }
//    context->present(*main_win);
}

std::string get_string(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue, std::string original){
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
        else if((ch>0&&ch<128)&&input.length()<30&&(isdigit(ch)||isalpha(ch)||isblank(ch))){
            input.push_back(ch);
        }
        std::stringstream ss;
        ss << dialogue << input;
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

unsigned int get_int(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue){
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
        ss << dialogue << input;
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

long long int get_llint(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue){
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
        ss << dialogue << input;
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

unsigned long long int get_ullint(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, const std::string &dialogue){
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
        ss << dialogue << input;
        clear_and_draw_dialog(main_win, context, ss.str());
    }
}

void timer_thread(Miner &miner, const bool &terminate){
    while(!terminate){
        if(miner.job.has_active_job&&miner.job.is_job_finished()){
            miner.loot.mysterious_piece+=((20*miner.job.number_of_miners)+generate_random_number(0,2*miner.job.number_of_miners))*miner.job.loot_multiplier;
            miner.loot.mysterious_artifact+=((4*miner.job.number_of_miners)+generate_random_number(0,miner.job.number_of_miners))*miner.job.loot_multiplier;
            miner.job=Miner::Job_Details();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}

void end_program(int sig){
    if(sig==0){
        std::cout << "Nya~" << std::endl;
        std::ifstream cosmos_ascii("res/cosmos.txt");
        std::string line;
        while(std::getline(cosmos_ascii, line)){
            std::cout << line << std::endl;
        }
        return;
    }
    if(sig==1){
        std::ifstream tombstone_file("res/tombstone.txt");
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

bool is_empty(std::ifstream &pFile){
    return pFile.peek()==std::ifstream::traits_type::eof();
}

void init_data(Player &User, level &Current, Csr &csr_pos, std::vector<monster> &monsters, NPC &npc, NoDelete &perm_config){
    std::string version_check;
    if(username.length()>30){
        throw std::runtime_error("Nope. Nopenopenopenope. You didn't follow my instructions.");
    }
    std::ifstream ifile("save/user.save", std::ios::binary);
    if(!is_empty(ifile)){
        cereal::PortableBinaryInputArchive retrieve(ifile);
        retrieve(User, Current, csr_pos, monsters, npc, perm_config, version_check);
        if(save_file_version!=version_check){
            throw std::runtime_error("Versions of save files do not match. Aborting. Do not attempt to edit the save files, lost of data will be expected.");
        }
        User.init();
        // Insert data corruption checks
        std::filesystem::remove("save/user.save.1");
        std::filesystem::copy("save/user.save", "save/user.save.1");
    }
}

void save_data(Player User, level Current, Csr csr_pos, const std::vector<monster> &monsters, const NPC &npc, NoDelete perm_config){
    std::ofstream ofile("save/user.save", std::ios::trunc | std::ios::binary);
    cereal::PortableBinaryOutputArchive archive(ofile);
    User.uninitialize_stats();
    archive(User, Current, csr_pos, monsters, npc, perm_config, save_file_version);
}

void print_starting_screen(tcod::ConsolePtr &main_win, tcod::ContextPtr &context){
    std::ifstream ascii_wayfarer("res/wayfarer.txt");
    if(!ascii_wayfarer){
        end_program(-1, "wayfarer.txt not found!");
        return;
    }
    int win_iterator=10;
    std::string line;
    while(std::getline(ascii_wayfarer, line)){
        tcod::print(*main_win, {7, win_iterator}, line, &WHITE, &FULL_BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        win_iterator++;
    }
    tcod::print(*main_win, {37, 25}, "A complex dungeon crawler with endless possibilities...", &WHITE, &FULL_BLACK, TCOD_BKGND_SET, TCOD_CENTER);
    tcod::print(*main_win, {35, 33}, "-Press any key to continue-", &WHITE, &FULL_BLACK, TCOD_BKGND_SET, TCOD_CENTER);
    context->present(*main_win);
    ascii_wayfarer.close();
    int ch;
    while(true){
        ch=SDL_getch(main_win, context);
        if(ch>0&&ch<128){
            break;
        }
    }
}

void init(){
    // Variable initialization
    Csr csr_pos{1, 1};
    Player User;
    level Current{1, 1, 1};
    NPC npc;
    NoDelete perm_config;
    User.inv.misc.heal_amount=10;
    tcod::ConsolePtr main_win=tcod::new_console(80, 56);
    TCOD_ContextParams params{};
    params.tcod_version=TCOD_COMPILEDVERSION;
    params.columns=main_win->w;
    params.rows=main_win->h;
    params.window_title="Wayventure_Client";
    params.sdl_window_flags=SDL_WINDOW_RESIZABLE;
    params.vsync=true;
    params.renderer_type=TCOD_RENDERER_SDL2;
    params.tileset=tileset.get();
    tcod::ContextPtr context=tcod::new_context(params);
    TCOD_console_set_default_foreground(main_win.get(), WHITE);
    TCOD_console_set_default_background(main_win.get(), BLACK);
    std::vector<monster> monsters;
    bool terminate_timer=false;
    // Get data from save files (if present)
    init_data(User, Current, csr_pos, monsters, npc, perm_config);
    // Start timer thread to refresh jobs
    std::thread refresh_timer(timer_thread, std::ref(npc.miner), std::ref(terminate_timer));
    // Start main dungeon
    print_starting_screen(main_win, context);
    init_dungeon(main_win, context, csr_pos, User, Current, monsters, npc, perm_config);
    // Terminate thread
    terminate_timer=true;
    refresh_timer.join();
    // Terminate libtcod
    TCOD_quit();
}

int main(int argc, char *argv[]){
    init();
    return 0;
}
