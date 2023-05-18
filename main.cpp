#include "Screen.h"
#include "Config.h"
#include "Map.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif
Screen screen;

#ifdef _WIN32
std::vector<std::string> list_txt(std::string folder_name)
{
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
    return names;
}
#else
std::vector<std::string> list_txt(std::string folder_name)
{
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
    return names;
}

#endif

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
        int answer;
        while (1)
        {
            answer = screen.wait_select();
            if (answer == (int)map_names.size())
            {
                return;
            }
            else if (0 <= answer && answer <= (int)map_names.size() - 1)
            {
                break;
            }
        }

        screen.play_map(map_names[answer]);
    }
}

void setting_theme()
{
    while (1)
    {
        screen.init_menu(
            "Setting for Sokoban",
            "Press up/down or w/s to change selection. Press space or enter to select the one you want to change.",
            "Please pick an option.");
        screen.add_option("show border: ");
        screen.add_option("max map number");

        screen.add_option("align: ");
        int answer = screen.wait_select();
        switch (answer)
        {
        case 0:
            select_map();
            break;
        case 1:

            break;
        case 2:
            return;
            break;

        default:
            break;
        }
    }
}

int main()
{
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