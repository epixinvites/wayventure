#include <string>
#include <fstream>
#include "headers/main.h"
void help_mode(tcod::ConsolePtr &main_win, tcod::ContextPtr &context, std::string mode){
    SDL_wclear_main_win(main_win, context);
    SDL_wclear_dialog_bar(main_win, context);
    tcod::print(*main_win, {0,0}, "Help", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    std::ifstream dungeon_help("res/help/"+mode+".txt");
    std::string line;
    int count = 0;
    while(std::getline(dungeon_help, line)){
        tcod::print(*main_win, {0,count+1}, line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        count++;
    }
    context->present(*main_win);
    int ch = SDL_getch(main_win, context);
}
