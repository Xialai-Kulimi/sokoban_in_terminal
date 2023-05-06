#include <iostream>
#include "Map.h"
#include "Block.h"
#include "Screen.h"
#include "string"

int main()
{
    std::cout << "Test start\n";
    // Map map = Map("mission3");
    // Block b = Block("air");
    // std::cout << "Air Block: "<<  b.to_string() << "\n";
    // std::cout << "mission3: " << map.to_string() << std::endl;
    Screen screen;
    // screen.test();
    std::cout << screen.get_key(true) << '\n';
    // std::string test_string = "123";
    // std::cout << test_string.substr(1, 2) << "\n";
    // std::cout << test_string.substr(1, 4) << "\n";
}