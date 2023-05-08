#include "Screen.h"
#include "Map.h"
#include <iostream>

int main(){
    std::ios::sync_with_stdio(false);

    Screen screen;

    screen.init_menu(
        "Sokoban in Terminal", 
        "Press up/down or w/s to change selection. Press space or enter to confirm.",
        "Please pick an option."
    );
    screen.add_option("Play");
    screen.add_option("Setting");
    screen.add_option("Exit");
    screen.wait_select();
    screen.set_align("center");
    screen.wait_select();

    Map map("mission3");
    
    screen.init_map(map);
    screen.render();

}