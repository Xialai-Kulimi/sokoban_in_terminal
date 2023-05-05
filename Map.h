#pragma once
#include <string>
#include <vector>
#include "Block.h"
using namespace std;

class Map
{
private:
    vector<string> raw_map;
    vector<vector<Block> > map;
    int row, column;

    void generate_map();

public:
    Map();
    Map(string); // init with map filename

    string to_string();
};
