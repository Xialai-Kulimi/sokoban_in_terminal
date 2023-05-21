#include "Map.h"
#include "Block.h"
#include <map>

class Screen
{
private:
    std::string mode;
    Map map;
    std::string title, description, question;
    std::vector<std::string> options;
    std::map<std::string, std::string> block_texture_map;

    int screen_row, screen_column;
    int map_row, map_column;
    int camera_column, camera_row;
    int max_block_column, max_block_row;
    int move_count;

    std::vector<int> get_arrow(std::string); // {row, column} {1, 0} for down, {0, 1} for right

    void fill(std::string);
    void wait(float);

    int mark_pos;
    int max_width;
    int default_max_width;
    int max_height;

    std::string to_center(std::string);
    std::vector<std::string> base_output;

    void add_base(std::string, bool);

    void add_blockmap_to_base();

    void render_menu();
    void render_map(bool debug = false);

    void print_base(); // print base_output in the middle of screen

    void load_block_texture();
    // user config
    std::string align;
    bool clear_screen_before_render;
    bool show_border;

    // user config
    void set_align(std::string);
    void set_mode(std::string); // set rendering mode
    int frame_rate;

    void toggle_align();
    void toggle_show_border();

    bool popup;
    std::string popup_string;

    void set_size();
    void hide_cursor();
    void show_cursor();

    void mod_mark_pos();

public:
    Screen(bool, bool, int, int);
    Screen();
    ~Screen();

    // for test
    void test();
    void render();
    void clear();
    void reset_cursor();
    
    std::string get_key(bool debug = false);

    // display menu
    void init_menu(std::string, std::string, std::string, bool clear = true);
    void add_option(std::string);
    int wait_select(bool reset = true);

    // display map
    void init_map(Map);
    int play_map(std::string, int); // return step


    void send_popup(std::string, bool wait=true);

    friend void setting_theme();
    friend void select_map();
};
