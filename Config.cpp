#include "Config.h"
#include <map>
#include <stdio.h>
#include <iostream>
#include <fstream>

std::vector<std::string> generate_block_type_list()
{
    std::vector<std::string> v;

    v.push_back("wall");
    v.push_back("air");
    v.push_back("player");
    v.push_back("box");
    v.push_back("end");
    v.push_back("placed");
    v.push_back("player_stand");
    return v;
}

std::vector<std::string> Config::block_type_list = generate_block_type_list();

std::vector<std::string> generate_align_mode_list()
{
    std::vector<std::string> v;

    v.push_back("left");
    v.push_back("center");
    return v;
}

std::vector<std::string> Config::align_mode_list = generate_align_mode_list();

std::vector<std::string> generate_rendering_mode_list()
{
    std::vector<std::string> v;

    v.push_back("menu");
    v.push_back("map");
    return v;
}

std::vector<std::string> Config::rendering_mode_list = generate_rendering_mode_list();

std::map<std::string, int> load_config_int_map()
{

    std::map<std::string, int> config_map;

    std::ifstream fin("config.txt");
    if (fin)
    {
        while (1)
        {
            std::string readed_string;
            std::getline(fin, readed_string);
            if (readed_string == "")
            {
                break;
            }
            else
            {
                char key[100];
                int value;
                sscanf(readed_string.c_str(), "%s %d", key, &value);
                config_map[std::string(key)] = value;
            }
        }
    }
    else
    {
        std::cerr << "config.txt is not found.\n";
        exit(1);
    }
    return config_map;
}

std::map<std::string, int> config_int_map = load_config_int_map();

int Config::max_map_number = config_int_map["max_map_number"];