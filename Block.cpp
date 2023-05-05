#include "Block.h"
#include "Config.h"
#include <iostream>

Block::Block(std::string block_type)
{
    std::vector<std::string> block_type_list = Config::block_type_list;
    if (std::count(block_type_list.begin(), block_type_list.end(), block_type))
    {
        this->type = block_type;
    }
    else
    {
        std::cerr << block_type << " is not a valid block type.\n";
    }
}

std::string Block::get_type()
{
    return this->type;
}

std::string Block::to_string()
{
    return "Block(\"" + this->get_type() + "\")";
}
