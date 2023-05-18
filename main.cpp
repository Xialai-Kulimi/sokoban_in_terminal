#include "Screen.h"
#include "Config.h"
#include "Map.h"
#include <iostream>
#include <filesystem>

Screen screen;

void test_map();

void select_map()
{
    while (1)
    {

        screen.init_menu(
            "Select the map",
            "Press up/down or w/s to change selection. Press space or enter to confirm.",
            "Please pick the map you want to play.");
        for (int i = 1; i <= Config::max_map_number; i++)
        {
            screen.add_option("level " + std::to_string(i));
        }
        screen.add_option("cancel");
        int answer;
        while (1)
        {
            answer = screen.wait_select();
            if (answer == Config::max_map_number)
            {
                return;
            }
            else if (0 <= answer && answer <= Config::max_map_number - 1)
            {
                break;
            }
        }
        Map map(answer + 1);

        screen.init_map(map);
        screen.play_map();
    }
}

void test_map()
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

int main()
{
    std::ios::sync_with_stdio(false);

    // screen.wait_select();
    // screen.set_align("center");
    while (1)
    {
        screen.init_menu(
            "Sokoban in Terminal",
            "Press up/down or w/s to change selection. Press space or enter to confirm.",
            "Please pick an option.");
        screen.add_option("Play");
        screen.add_option("Setting");
        screen.add_option("Exit");
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