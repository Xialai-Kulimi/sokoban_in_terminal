#pragma once
#include <string>


class Block
{
private:
    std::string type;

public:
    std::string get_type();
    Block(std::string);
    std::string to_string();
};

