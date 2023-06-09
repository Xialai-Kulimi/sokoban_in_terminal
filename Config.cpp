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
    v.push_back("setting");
    return v;
}

std::vector<std::string> Config::rendering_mode_list = generate_rendering_mode_list();
