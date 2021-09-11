#include <string>
#include <fstream>
void help_mode(std::unique_ptr<TCOD_Console, tcod::ConsoleDeleter> &main_win, std::unique_ptr<TCOD_Context, tcod::ContextDeleter> &context, WINDOW *interaction_bar, std::string mode){
    wclear(main_win);
    wclear(interaction_bar);
    mvwaddstr(interaction_bar,0,0,"Help");
    std::ifstream dungeon_help("res/help/"+mode+".txt");
    std::string line;
    int count = 0;
    while(std::getline(dungeon_help, line)){
        mvwaddstr(main_win,count,0,line.c_str());
        count++;
    }
    wrefresh(main_win);
    wrefresh(interaction_bar);
    int ch = wgetch(main_win);
}
