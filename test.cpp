#include <iostream>
#include "Map.h"
#include "Block.h"
#include "Screen.h"
#include "string"



int main(){
    std::cout << "Test start\n";
    Map map = Map("mission3");
    Block b = Block("air");
    std::cout << "Air Block: "<<  b.to_string() << "\n";
    std::cout << "mission3: " << map.to_string() << std::endl;
    Screen screen;
    screen.fill('X');
    for (int i = 0; i < 10; i++)
    {
        /* code */
    std::cout << "get_key: " << screen.get_key() << std::endl;
    }
    
}