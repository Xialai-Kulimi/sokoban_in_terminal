#pragma once
#include <vector>
#include <string>

class Config
{
private:
public:
    static int max_map_number;
    static std::vector<std::string> block_type_list;
    static std::vector<std::string> align_mode_list;
    static std::vector<std::string> rendering_mode_list;
    static bool show_border;
    Config() {}
};
