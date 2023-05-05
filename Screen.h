#include "Map.h"

class Screen
{
private:
    /* data */
    int mode;  // 0 for menu, 1 for map
    Map map;
    string title, description, question;

    int screen_row, screen_column;

    int get_up_or_down();
    int get_arrow();
    void clear();
public:
    Screen();
    void fill(char);
};
