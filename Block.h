#pragma once
#include <string>

using namespace std;

class Block
{
private:
    string type;

public:
    string get_type();
    Block(string);
    string to_string();
};

