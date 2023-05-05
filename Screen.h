#include "Map.h"

class Screen
{
private:
    /* data */
    std::string mode;
    Map map;
    std::string title, description, question;
    std::vector<std::string> options;

    int screen_row, screen_column;

    int get_up_or_down();
    std::vector<int> get_arrow();  // {1, 0} for right, {0, 1} for up


    void clear();
    void fill(char);
    void wait(float);

    int mark_pos;

    int max_width;
    
    std::string to_center(std::string);

    std::string align;

    std::vector<std::string> base_output;
    
    void clean_base();
    void add_base(std::string, bool);
    
    void render_menu();
public:
    Screen();
    
    void test();
    void init_menu(std::string, std::string, std::string);
    void add_option(std::string);
    void render();

    std::string get_key();

    // int wait_menu();
    void set_align(std::string);

    

};
