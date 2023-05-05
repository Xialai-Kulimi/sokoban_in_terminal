#include "Map.h"

class Screen
{
private:
    /* data */
    int mode;  // 0 for menu, 1 for map
    Map map;
    std::string title, description, question;

    int screen_row, screen_column;

    int get_up_or_down();
    std::vector<int> get_arrow();  // {1, 0} for right, {0, 1} for up


    void clear();
    void fill(char);
    std::string get_key();
    
public:
    Screen();
    
    void test();
    

};
