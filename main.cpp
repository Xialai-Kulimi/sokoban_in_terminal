#include "Screen.h"
#include "Config.h"
#include "Map.h"
#include "Profile.h"
#include <iostream>
#include <algorithm>
#include <locale>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif

Profile profile;
Screen screen;

std::vector<std::string> list_txt(std::string folder_name)
{
#ifdef _WIN32
    std::vector<std::string> names;
    std::string search_path = folder_name + "/*.txt";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                names.push_back(fd.cFileName);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }

#else

    DIR *dr;
    std::vector<std::string> names;
    struct dirent *en;
    dr = opendir(folder_name.c_str()); // open all or present directory
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            std::string filename = std::string(en->d_name);
            if (filename.substr(filename.find_last_of(".") + 1) == "txt")
            {
                names.push_back(filename);
            }
        }
        closedir(dr); // close all directory
    }

#endif
    std::sort(names.begin(), names.end());
    return names;
}

void select_map()
{
    screen.mark_pos = 0;
    while (1)
    {
        screen.init_menu(
            "Select the map",
            "Press up/down or w/s to change selection. Press space or enter to confirm.",
            "Please pick the map you want to play.");

        std::vector<std::string> map_names = list_txt("maps");

        for (int i = 0; i < (int)map_names.size(); i++)
        {
            screen.add_option(map_names[i].substr(0, map_names[i].find_last_of(".")));
        }
        screen.add_option("cancel");

        int answer = screen.wait_select(false);
        if (answer == (int)map_names.size() || answer == -1)
        {
            return;
        }
        else if (0 <= answer && answer <= (int)map_names.size() - 1)
        {
            screen.send_popup("loading...", false);
            screen.play_map(map_names[answer]);
        }
    }
}

std::string bool_to_string(bool bool_value)
{
    if (bool_value)
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

std::string hightlight_when_equal(std::string input_string, int number_1, int number_2)
{
    if (number_1 == number_2)
    {
        if (number_1 == 0 || number_1 == 1)
        {
            return " *" + input_string + "* ";
        }
        else
        {
            return " <" + input_string + "> ";
        }
    }
    else
    {
        return "  " + input_string + "  ";
    }
}

void setting_theme()
{
    screen.mark_pos = 0;
    while (1)
    {
        std::string hint_message = "Press enter or space to flip the value.";
        if (screen.mark_pos == 2 || screen.mark_pos == 3)
        {
            hint_message = "Press left/right or a/d to adjust the value.";
        }

        screen.init_menu(
            "Setting for Sokoban",
            "Press up/down or w/s to change selection. Press space or enter to select the one you want to change.",
            hint_message, false);
        int i = 0;
        screen.add_option("show border:" + hightlight_when_equal(bool_to_string(screen.show_border), i++, screen.mark_pos));
        screen.add_option("align:" + hightlight_when_equal(screen.align, i++, screen.mark_pos));
        screen.add_option("frame rate:" + hightlight_when_equal(std::to_string(screen.frame_rate), i++, screen.mark_pos));
        screen.add_option("max width:" + hightlight_when_equal(std::to_string(screen.default_max_width), i++, screen.mark_pos) + "(current: " + std::to_string(screen.max_width) + ")");
        screen.add_option("cancel");

        // int answer = screen.wait_select(false);
        // int answer = screen.mark_pos;
        std::string recv_string = screen.get_key();
        if (recv_string == "esc")
        {
            return;
        }

        std::vector<int> control_vector = screen.get_arrow(recv_string);
        if (control_vector[0] != 0)
        {
            screen.mark_pos += control_vector[0];
            screen.mod_mark_pos();
        }
        else
        {
            switch (screen.mark_pos)
            {
            case 0:
                if (control_vector[1] == 0)
                {
                    screen.toggle_show_border();
                }
                break;
            case 1:
                if (control_vector[1] == 0)
                {
                    screen.toggle_align();
                }
                break;
            case 2:
                screen.frame_rate = ((screen.frame_rate / 10) + control_vector[1]) * 10;
                screen.frame_rate = (screen.frame_rate + 50) % 60 + 10;

                break;
            case 3:
                screen.default_max_width = std::max(75, screen.default_max_width + control_vector[1] * 6);
                screen.set_size();
                break;
            case 4:
                if (control_vector[1] == 0)
                {
                    return;
                }
                break;

            default:
                break;
            }
        }
    }
}

int main()
{
    screen.clear();
    std::locale l("");
    std::ios::sync_with_stdio(false);

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
            setting_theme();
            break;
        case 2:
        case -1:
            return 0;
            break;

        default:
            break;
        }
    }
}