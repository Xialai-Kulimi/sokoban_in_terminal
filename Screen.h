#include "Map.h"
#include "Block.h"
#include <map>

class Screen
{
private:
    /* data */
    std::string mode;
    Map map;
    std::string title, description, question;
    std::vector<std::string> options;
    std::map<std::string, std::string> block_texture_map;

    int screen_row, screen_column;
    int map_row, map_column;
    int camera_column, camera_row;
    int max_block_column, max_block_row;

    std::vector<int> get_arrow();  // {row, column} {1, 0} for down, {0, 1} for right
    
    void fill(char);
    void wait(float);

    int mark_pos;
    int max_width;
    
    std::string to_center(std::string);
    std::vector<std::string> base_output;
    
    void add_base(std::string, bool);

    void add_blockmap_to_base();

    void render_menu();
    void render_map();

    void print_base();  // print base_output in the middle of screen

    void load_block_texture();
    // user config
    std::string align;
    bool clear_screen_before_render;
public:
    Screen();

    // for test
    void test();
    void render();
    void clear();
    std::string get_key(bool debug=false);

    // display menu
    void init_menu(std::string, std::string, std::string);
    void add_option(std::string);
    int wait_select ();

    // display map
    void init_map(Map);
    int play_map();  // return score
    
    // user config
    void set_align(std::string);
    void set_mode(std::string);  // set rendering mode
};
