#include "Screen.h"
#include "Map.h"
#include <iostream>
#include <filesystem>



Screen screen;

void select_map()
{
    

    Map map(3);

    screen.init_map(map);
    screen.render();
    screen.init_map(Map(4));
    screen.render();
    std::vector<int> p_pos = map.find_player_pos();
    printf("p_row: %d, p_col: %d\n", p_pos[0], p_pos[1]);
    screen.play_map();
}

void test_map()
{
    Map map(3);

    screen.init_map(map);
    screen.render();
    screen.init_map(Map(2));
    screen.render();
    std::vector<int> p_pos = map.find_player_pos();
    printf("p_row: %d, p_col: %d\n", p_pos[0], p_pos[1]);
    screen.play_map();
}

int main()
{
    std::ios::sync_with_stdio(false);

    screen.init_menu(
        "Sokoban in Terminal",
        "Press up/down or w/s to change selection. Press space or enter to confirm.",
        "Please pick an option.");
    screen.add_option("Play");
    screen.add_option("Setting");
    screen.add_option("Exit");
    // screen.wait_select();
    // screen.set_align("center");
    while (1)
    {
        int answer = screen.wait_select();
        switch (answer)
        {
        case 0:
            select_map();
            break;
        case 1:
            test_map();
            break;
        case 2:
            return 0;
            break;

        default:
            break;
        }
    }
}