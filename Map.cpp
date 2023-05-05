#include "Map.h"
#include "Block.h"
#include <fstream>
#include <iostream>
#include <cmath>



Map::Map()
{
    this->row = 0;
    this->column = 0;
}

Map::Map(std::string filename)
{
    std::string file_path = "maps/" + filename + ".txt";
    std::ifstream fin(file_path);
    if (fin)
    {
        fin >> this->row >> this->column;
        for (int i = 0; i < this->row; i++)
        {
            std::string current_row;
            fin >> current_row;
            this->raw_map.push_back(current_row);
        }
        this->generate_map();
    }
    else
    {
        std::cerr << file_path << " is not found.\n";
    }
}

void Map::generate_map()
{
    this->map.clear();
    for (int y = 0; y < this->row; y++)
    {
        std::vector<Block> current_row;
        int raw_length = (int)this->raw_map[y].length();
        for (int x = 0; x < raw_length; x++)
        {
            char raw_block = this->raw_map[y][x];
            switch (raw_block)
            {
            case '/':
                current_row.push_back(Block("wall"));
                break;
            case '0':
                current_row.push_back(Block("player"));
                break;
            case '1':
                current_row.push_back(Block("box"));
                break;
            case '2':
                current_row.push_back(Block("end"));
                break;
            case '-':
                current_row.push_back(Block("air"));
                break;

            default:
                std::cerr << "Unknown raw block type: " << raw_block << std::endl;
                break;
            }
        }

        for (int i = 0; i < std::min(this->column - raw_length, 0); i++)
        {
            current_row.push_back(Block("air"));
        }

        this->map.push_back(current_row);
    }
}


std::string Map::to_string()
{
    std::string map_string = "Map(\n";
    for (int y = 0; y < this->row; y++)
    {
        map_string = map_string + "\t[ ";
        for (int x = 0; x < this->column; x++)
        {
            map_string = map_string + this->map[y][x].to_string() + "\t";
        }
        map_string = map_string + "]\n";
    }
    map_string = map_string + ")\n";

    return map_string;
}