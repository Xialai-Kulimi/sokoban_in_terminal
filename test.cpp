#include <iostream>
// #include "Map.h"
// #include "Block.h"
// #include "Screen.h"
#include "string"
#include <filesystem>


int main()
{
    std::cout << "Test start\n";
    // Map map = Map(3);
    // Block b = Block("air");
    // std::cout << "Air Block: "<<  b.to_string() << "\n";
    // std::cout << "mission3: " << map.to_string() << std::endl;
    // Screen screen;
    // screen.test();
    // std::cout << screen.get_key(true) << '\n';
    // std::string test_string = "123";
    // std::cout << test_string.substr(1, 2) << "\n";

    // std::cout << test_string.substr(1, 4) << "\n";
    std::string path = "maps";
    for (const auto & entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;

}