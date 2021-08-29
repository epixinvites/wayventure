#include <curses.h>
#include "headers/mode.h"
#include "headers/classes.h"
#include "headers/draw.h"
class NPC{
    int x, y;
    char type;
};
void bar_mode(WINDOW *main_win, WINDOW *status_win, WINDOW *interaction_bar,Player &User){
    wclear(main_win);
    wclear(interaction_bar);
    mvwaddstr(interaction_bar,0,0,"Pub");
    draw_border(main_win);
    wrefresh(interaction_bar);
    wrefresh(main_win);
    int ch;
    while(true){
        ch=wgetch(main_win);
    }
}
