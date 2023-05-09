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
    // screen.wait_select();
    screen.set_align("center");
    screen.wait_select();

    Map map("mission3");
    
    screen.init_map(map);
    screen.render();
    screen.init_map(Map("mission1"));
    screen.render();
    std::vector<int> p_pos = map.find_player_pos();
    printf("p_row: %d, p_col: %d\n", p_pos[0], p_pos[1]);

}