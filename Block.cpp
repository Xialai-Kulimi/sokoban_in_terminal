#include "Block.h"
#include "config.cpp"
#include <iostream>

Block::Block(string block_type)
{
    bool valid = false;
    for (int i = 0; i < (int)config.block_type_list.size(); i++)
    {
        if (block_type == config.block_type_list[i])
        {
            valid = true;
            break;
        }
    }
    if (valid)
    {
        this->type = block_type;
    }
    else
    {
        cerr << block_type << " is not a valid block type.\n";
    }
}

string Block::get_type()
{
    return this->type;
}

string Block::to_string()
{
    return "Block(\"" + this->get_type() + "\")";
}
