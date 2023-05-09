#pragma once
#include <string>
#include <vector>
#include "Block.h"


class Map
{
private:
    std::vector<std::string> raw_map;
    std::vector<std::vector<Block> > map;
    int row, column;

    void generate_map();

public:
    Map();
    Map(std::string); // init with map filename

    std::string to_string();
    Block get(int, int);
    int get_row();
    int get_column();

    std::vector<int> find_player_pos();

};
