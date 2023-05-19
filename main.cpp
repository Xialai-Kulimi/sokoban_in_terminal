#include "Screen.h"
#include "Config.h"
#include "Map.h"
#include <iostream>
#include <algorithm>
#include <locale>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif

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

        int answer = screen.wait_select();
        if (answer == (int)map_names.size())
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

void setting_theme()
{
    screen.mark_pos = 0;
    while (1)
    {
        screen.init_menu(
            "Setting for Sokoban",
            "Press up/down or w/s to change selection. Press space or enter to select the one you want to change.",
            "Please pick an option.");

        screen.add_option("show border: " + bool_to_string(screen.show_border));
        screen.add_option("align: " + screen.align);
        screen.add_option("frame rate: " + std::to_string(screen.frame_rate));
        screen.add_option("cancel");

        int answer = screen.wait_select(false);
        switch (answer)
        {
        case 0:
            screen.toggle_show_border();
            break;
        case 1:
            screen.toggle_align();
            break;
        case 2:
            screen.frame_rate = (screen.frame_rate / 10 % 6 + 1) * 10 ;
            break;
        case 3:
            return;
            break;

        default:
            break;
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
            return 0;
            break;

        default:
            break;
        }
    }
}