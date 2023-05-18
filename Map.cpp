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

Map::Map(std::string map_name)
{
    std::ifstream fin("maps/" + map_name);
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
        std::cerr << map_name << " is not found.\n";
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
            case '3':
                current_row.push_back(Block("placed"));
                break;
            case '4':
                current_row.push_back(Block("player_stand"));
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

Block Map::get(int block_row, int block_column)
{
    // printf("r: %d, c: %d\n", block_row, block_column);
    return this->map[block_row][block_column];
}

int Map::get_column()
{
    return this->column;
}

int Map::get_row()
{
    return this->row;
}

std::vector<int> Map::find_player_pos()
{
    std::vector<int> pos;
    for (int r = 0; r < this->row; r++)
    {
        for (int c = 0; c < this->column; c++)
        {
            std::string block_type = this->map[r][c].get_type();
            if (block_type == "player" || block_type == "player_stand")
            {
                pos.push_back(r);
                pos.push_back(c);
                return pos;
            }
        }
    }
    std::cerr << "Player is not found in the map.\n";
    pos.push_back(-1);
    pos.push_back(-1);
    return pos;
}

void Map::swap(int row_1, int column_1, int row_2, int column_2)
{
    Block temp = this->get(row_1, column_1);
    this->map[row_1][column_1] = this->get(row_2, column_2);
    this->map[row_2][column_2] = temp;
}

bool Map::player_move(std::vector<int> mov_vector)
{
    std::vector<int> pos = this->find_player_pos();
    Block player_block = this->get(pos[0], pos[1]);

    Block target_block = this->get(pos[0] + mov_vector[0], pos[1] + mov_vector[1]);

    if (player_block.get_type() == "player")
    {

        if (target_block.get_type() == "air")
        {
            this->swap(pos[0], pos[1], pos[0] + mov_vector[0], pos[1] + mov_vector[1]);
            return true;
        }
        else if (target_block.get_type() == "end")
        {
            this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player_stand");
            this->map[pos[0]][pos[1]] = Block("air");
            return true;
        }
        else if (target_block.get_type() == "box")
        {
            Block behind_target = this->get(pos[0] + 2 * mov_vector[0], pos[1] + 2 * mov_vector[1]);
            if (behind_target.get_type() == "air")
            {
                this->swap(pos[0] + mov_vector[0], pos[1] + mov_vector[1], pos[0] + 2 * mov_vector[0], pos[1] + 2 * mov_vector[1]);
                this->swap(pos[0], pos[1], pos[0] + mov_vector[0], pos[1] + mov_vector[1]);
                return true;
            }
            else if (behind_target.get_type() == "end")
            {
                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("placed");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player");
                this->map[pos[0]][pos[1]] = Block("air");
                return true;
            }
        }
        else if (target_block.get_type() == "placed")
        {
            Block behind_target = this->get(pos[0] + 2 * mov_vector[0], pos[1] + 2 * mov_vector[1]);
            if (behind_target.get_type() == "air")
            {
                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("box");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player_stand");
                this->map[pos[0]][pos[1]] = Block("air");
                return true;
            }
            else if (behind_target.get_type() == "end")
            {
                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("placed");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player_stand");
                this->map[pos[0]][pos[1]] = Block("air");
                return true;
            }
        }
        return false;
    }
    else
    {
        // player type = player_stand
        if (target_block.get_type() == "air")
        {
            this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player");
            this->map[pos[0]][pos[1]] = Block("end");
            return true;
        }
        else if (target_block.get_type() == "end")
        {
            this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player_stand");
            this->map[pos[0]][pos[1]] = Block("end");
            return true;
        }
        else if (target_block.get_type() == "box")
        {
            Block behind_target = this->get(pos[0] + 2 * mov_vector[0], pos[1] + 2 * mov_vector[1]);
            if (behind_target.get_type() == "air")
            {

                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("box");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player");
                this->map[pos[0]][pos[1]] = Block("end");
                return true;
            }
            else if (behind_target.get_type() == "end")
            {
                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("placed");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player");
                this->map[pos[0]][pos[1]] = Block("end");
                return true;
            }
        }
        else if (target_block.get_type() == "placed")
        {
            Block behind_target = this->get(pos[0] + 2 * mov_vector[0], pos[1] + 2 * mov_vector[1]);
            if (behind_target.get_type() == "air")
            {
                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("box");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player_stand");
                this->map[pos[0]][pos[1]] = Block("end");
                return true;
            }
            else if (behind_target.get_type() == "end")
            {
                this->map[pos[0] + 2 * mov_vector[0]][pos[1] + 2 * mov_vector[1]] = Block("placed");
                this->map[pos[0] + 1 * mov_vector[0]][pos[1] + 1 * mov_vector[1]] = Block("player_stand");
                this->map[pos[0]][pos[1]] = Block("end");
                return true;
            }
        }
        return false;
    }
}

bool Map::check_win()
{
    for (int row = 0; row < this->row; row++)
    {
        for (int column = 0; column < this->column; column++)
        {
            if (this->get(row, column).get_type() == "box")
            {
                return false;
            }
        }
    }
    return true;
}