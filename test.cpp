#include <iostream>
#include "Map.h"
#include "Block.h"
#include "Screen.h"
#include "string"

using namespace std;

int main(){
    cout << "Test start\n";
    Map map = Map("mission3");
    Block b = Block("air");
    cout << "Air Block: "<<  b.to_string() << "\n";
    cout << "mission3: " << map.to_string() << endl;
    Screen screen;
    screen.fill('X');
}