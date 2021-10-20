#include <string>
#include <fstream>
#include "headers/main.h"

void help_mode(tcod::ConsolePtr &mainWin, tcod::ContextPtr &context, std::string mode){
    SDL_wclear_mainWin(mainWin, context);
    SDL_wclear_dialog_bar(mainWin, context);
    tcod::print(*mainWin, {0, 0}, "Help", &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
    std::ifstream dungeon_help("res/help/"+mode+".txt");
    std::string line;
    int count=0;
    while(std::getline(dungeon_help, line)){
        tcod::print(*mainWin, {0, count+1}, line, &WHITE, &BLACK, TCOD_BKGND_SET, TCOD_LEFT);
        count++;
    }
    context->present(*mainWin);
    int ch=0;
    while(!(ch>0&&ch<128)){
        ch=SDL_getch(mainWin, context);
    }
}
