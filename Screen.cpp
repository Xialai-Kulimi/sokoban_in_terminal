#include "Screen.h"
#include "Config.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
// Assume POSIX
#include <sys/ioctl.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

void Screen::slow_clear()
{
    // fill it with blank
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Screen::clear()
{
    // try to reset cursor to 0, 0
#ifdef _WIN32
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // DWORD count;
    // DWORD cellCount;
    COORD homeCoords = {0, 0};

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return this->slow_clear();

    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return this->slow_clear();

    // cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    // if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count))
    // {
    //     return this->slow_clear();
    // }
    // if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count))
    // {
    //     return this->slow_clear();
    // }
    SetConsoleCursorPosition(hStdOut, homeCoords);
#else
    return this->slow_clear();
#endif
}

Screen::Screen()
{
#ifdef _WIN32
    this->show_border = false;
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(handle);
#else
    this->show_border = true;
#endif
    this->align = "left";
    this->mode = "menu";
    this->load_block_texture();
    this->clear_screen_before_render = true;
    this->set_size();
    this->render_rate = 30;
}

void Screen::set_size()
{
#ifdef _WIN32

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    this->screen_column = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    this->screen_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    this->screen_row = w.ws_row;
    this->screen_column = w.ws_col;

#endif
    this->max_width = std::min(81, (this->screen_column - 4) / 3 * 3);
}

void Screen::fill(std::string texture)
{
    std::string content = "";
    for (int i = 0; i < this->screen_row; i++)
    {
        for (int j = 0; j < this->screen_column; j++)
        {
            content = content + texture;
        }
        if (i != this->screen_row - 1)
        {
            content = content + '\n';
        }
    }
    std::cout << content;
}

std::string Screen::get_key(bool debug)
{
#ifdef _WIN32

    while (1)
    {

        if (_kbhit())
        {
            int recv_key = _getch();
            if (debug)
            {
                std::cout << "recv: " << recv_key << "\n";
            }

            // force exit (ctrl+c)
            if (recv_key == 3)
            {
                exit(0);
            }

            if (recv_key == 0 || recv_key == 224) // When reading a function key or an arrow key, each function must be called twice; the first call returns 0 or 0xE0
            {
                // the arrow key was pressed

                recv_key = _getch();
                if (debug)
                {
                    std::cout << "recv: " << recv_key << "\n";
                }
                switch (recv_key)
                {

                case 72:
                    return "up";
                case 80:
                    return "down";
                case 75:
                    return "left";
                case 77:
                    return "right";
                }
            }
            else
            {
                switch (recv_key)
                {
                case 13:
                    return "enter";
                case 32:
                    return "space";
                case 27:
                    return "esc";
                }

                return std::string(1, (char)recv_key);
            }
        }
        else
        {
            this->render();
            this->wait(1.0 / (double)this->render_rate);
        }
    }

#else
    struct termios oldSettings, newSettings;

    tcgetattr(fileno(stdin), &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);
    char recv_char[10] = {0};
    while (1)
    {
        fd_set set;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 1000000 / this->render_rate;

        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);

        int res = select(fileno(stdin) + 1, &set, NULL, NULL, &tv);

        if (res > 0)
        {

            // printf("Input available\n");
            read(fileno(stdin), &recv_char, 3);
            if (debug)
            {
                printf("\n%d_%d_%d\n", recv_char[0], recv_char[1], recv_char[2]);
            }

            break;
        }
        else if (res < 0)
        {
            // perror("select error");
        }
        else
        {
            this->render();
        }
    }

    tcsetattr(fileno(stdin), TCSANOW, &oldSettings);

    if (recv_char[1] == 0)
    {
        switch (recv_char[0])
        {
        case ' ':
            return "space";
        case '\n':
            return "enter";
        case 27:
            return "esc";
        }
        return std::string(recv_char);
    }
    else if (recv_char[0] == 27 && recv_char[1] == 91)
    {
        switch (recv_char[2])
        {
        case 65:
            return "up";
        case 66:
            return "down";
        case 68:
            return "left";
        case 67:
            return "right";
        }
    }

#endif
    return "";
}

void Screen::test()
{
    // this->fill("█");
    // for (int i = 0; i < 10; i++)
    // {
    //     std::string recv_string = this->get_key();
    //     std::cout << "get_key: " << recv_string << std::endl;
    // }
    // for (int i = 0; i < 10; i++)
    // {
    //     std::string recv_string = this->get_key();
    //     for (int j = 0; j < (int)recv_string.length(); j++)
    //     {
    //         this->fill(recv_string[j]);
    //         this->wait(0.1);
    //     }
    // }
}

void Screen::wait(float sec)
{
#ifdef _WIN32
    Sleep((int)(1000.0 * sec));
#else
    usleep((int)(1000000.0 * sec));
#endif
}

void Screen::init_menu(
    std::string new_title,
    std::string new_description,
    std::string new_question)
{
    this->slow_clear();
    this->set_mode("menu");
    this->title = new_title;
    this->description = new_description;
    this->question = new_question;
    this->options.clear();
}

void Screen::add_option(std::string new_option)
{
    this->options.push_back(new_option);
}

std::string Screen::to_center(std::string input_string)
{
    int string_length = input_string.length();
    int margin_x = (this->max_width - string_length) / 2;
    return std::string(margin_x, ' ') + input_string + std::string(margin_x, ' ');
}

void Screen::add_base(std::string newline, bool center = false)
{
    if (newline == "")
    {
        this->base_output.push_back(std::string(this->max_width, ' '));
        return;
    }

    if (center && (int)newline.length() > this->max_width)
    {
        // std::cerr << "Error, you can't center a string which is longer then max_width.\n";
        // auto align to left when newline is too long
        center = false;
    }

    if (center)
    {
        this->base_output.push_back(this->to_center(newline));
    }
    else
    {
        for (int i = 0; i < std::ceil(((float)newline.length()) / ((float)this->max_width)); i++)
        {
            std::string new_string = newline.substr(i * this->max_width, this->max_width);
            this->base_output.push_back(new_string + std::string(this->max_width - (int)new_string.length(), ' '));
        }
    }
}

void Screen::render_menu()
{
    // generate base window, and make the window in the center
    this->base_output.clear();

    bool render_center = this->align == "center";

    this->add_base("# " + this->title, render_center);
    this->add_base("");
    this->add_base(this->description, render_center);
    this->add_base("");
    this->add_base("[ " + this->question + " ]", render_center);
    this->add_base("");
    for (int i = 0; i < (int)this->options.size(); i++)
    {
        if (render_center)
        {
            if (i == this->mark_pos)
            {
                this->add_base("> " + this->options[i] + " <", true);
            }
            else
            {
                this->add_base(this->options[i], true);
            }
        }
        else
        {
            if (i == this->mark_pos)
            {
                this->add_base("> " + this->options[i]);
            }
            else
            {
                this->add_base("  " + this->options[i]);
            }
        }
    }

    this->print_base();
}

void Screen::print_base()
{

    if (this->popup)
    {
        // add popup to base
        int base_row = (int)this->base_output.size();
        for (int i = 0; i < base_row; i++)
        {

            int base_column = (int)this->base_output[i].size();
            for (int j = 0; j < base_column; j++)
            {
                if (i == (base_row / 2))
                {
                    for (int k = 0; k < (int)this->popup_string.length(); k++)
                    {
                        this->base_output[i][base_column / 2 - (int)this->popup_string.length() / 2 + k] = this->popup_string[k];
                    }
                }
                else if (abs(i - (base_row / 2)) == 1)
                {
                    if (j == base_column / 2 - (int)this->popup_string.length() / 2 + 1)
                    {
                        this->base_output[i][j] = '[';
                    }
                    else if (j == base_column / 2 + (int)this->popup_string.length() / 2 - 2)
                    {
                        this->base_output[i][j] = ']';
                    }
                    else if (abs(j - (base_column / 2)) <= (int)this->popup_string.length() / 2)
                    {
                        this->base_output[i][j] = ' ';
                    }
                }
            }
        }
    }

    int margin_y = std::max((this->screen_row - (int)this->base_output.size()) / 2 - 1, 2);
    int margin_x = std::max((this->screen_column - this->max_width) / 2, 2);

    // printf("scr_w: %d, max_w: %d,margin_x: %d\n", this->screen_column, this->max_width, margin_x);
    std::vector<std::string> content;

    for (int i = 0; i < margin_y - 1; i++)
    {
        content.push_back(std::string(this->screen_column, ' '));
    }

    if (this->show_border)
    {
        std::string this_line = std::string(margin_x - 1, ' ') + "┏";
        for (int i = 0; i < screen_column - (margin_x * 2); i++)
        {
            this_line = this_line + "━";
        }
        this_line = this_line + "┓" + std::string(margin_x - 1, ' ');
        content.push_back(this_line);
    }
    else
    {
        content.push_back(std::string(this->screen_column, ' '));
    }
    if (this->show_border)
    {

        for (int i = 0; i < (int)this->base_output.size(); i++)
        {
            content.push_back(std::string(margin_x - 1, ' ') + "┃" + this->base_output[i] + std::string(screen_column - (margin_x * 2) - this->base_output[i].length(), ' ') + "┃" + std::string(margin_x - 1, ' '));
        }
    }
    else
    {
        for (int i = 0; i < (int)this->base_output.size(); i++)
        {
            content.push_back(std::string(margin_x, ' ') + this->base_output[i] + std::string(margin_x - 1, ' '));
        }
    }

    if (this->show_border)
    {
        std::string this_line = std::string(margin_x - 1, ' ') + "┗";
        for (int i = 0; i < screen_column - (margin_x * 2); i++)
        {
            this_line = this_line + "━";
        }
        this_line = this_line + "┛" + std::string(margin_x - 1, ' ');
        content.push_back(this_line);
    }
    else
    {
        content.push_back(std::string(this->screen_column, ' '));
    }

    for (int i = 0; i < margin_y - 1; i++)
    {
        content.push_back(std::string(this->screen_column, ' '));
    }
    // return content;

    // std::cout << content;
    std::string real_content = "";
    for (int i = 0; i < (int)content.size(); i++)
    {
        real_content = real_content + "\n" + content[i];
    }
    std::cout << real_content;
    std::cout.flush();
}

void Screen::init_map(Map map)
{
    this->slow_clear();
    this->map = map;
    this->set_mode("map");
    this->map_row = map.get_row();
    this->map_column = map.get_column();
    this->map_row = map.get_row();

    this->camera_column = 0;
    this->camera_row = 0;
}

void Screen::add_blockmap_to_base()
{

    for (int i = 0; i < this->max_block_row; i++)
    {
        // std::string current_line = std::to_string(i)+": ";
        std::string current_line = "";
        for (int j = 0; j < this->max_block_column; j++)
        {
            current_line = current_line + this->block_texture_map[this->map.get(i + camera_row, j + camera_column).get_type()];
        }
        this->base_output.push_back(this->to_center(current_line));
    }
}

void Screen::render_map(bool debug)
{
    // recalculate block map size, for auto resize
    this->max_block_column = std::min(this->map.get_column(), this->max_width / 3);
    this->max_block_row = std::min(std::min(this->map.get_row(), this->max_width / 3), std::max(this->screen_row - 7, 0));

    // calculate camera position
    std::vector<int> player_pos = this->map.find_player_pos();

    this->camera_row = std::max(player_pos[0] - this->max_block_row / 2, 0);
    this->camera_column = std::max(player_pos[1] - this->max_block_column / 2, 0);

    this->camera_row = std::min(this->camera_row, std::max(this->map_row - this->max_block_row, 0));
    this->camera_column = std::min(this->camera_column, std::max(this->map_column - this->max_block_column, 0));
    if (debug)
    {
        printf("max_block_row: %d, max_block_column: %d\n", this->max_block_row, this->max_block_column);
        printf("player_row: %d, player_column: %d\n", player_pos[0], player_pos[1]);
        printf("camera_row: %d, camera_column: %d\n", camera_row, camera_column);
        this->get_key();
    }

    // put map into base
    this->base_output.clear();
    this->add_blockmap_to_base();
    this->add_base("> Press \"ESC\" to quit, press \"r\" to restart.");
    // put player stat into base
    this->add_base("move count: " + std::to_string(this->move_count));

    if (debug)
    {
        this->add_base("");
        this->add_base("camera_row: " + std::to_string(this->camera_row));
        this->add_base("camera_column: " + std::to_string(this->camera_column));
        std::vector<int> pos = this->map.find_player_pos();
        this->add_base("player_row: " + std::to_string(pos[0]));
        this->add_base("player_column: " + std::to_string(pos[1]));
        this->add_base("map_row: " + std::to_string(this->map_row));
        this->add_base("map_column: " + std::to_string(this->map_column));
        this->add_base("max_width: " + std::to_string(this->max_width));
    }

    this->print_base();
}

void Screen::render()
{
    this->set_size();
    if (this->clear_screen_before_render)
    {
        this->clear();
    }
    // std::vector<std::string> content;
    if (this->mode == "menu")
    {
        this->render_menu();
    }
    else if (this->mode == "map")
    {
        this->render_map();
    }
}

void Screen::set_align(std::string new_align_mode)
{
    std::vector<std::string> align_mode_list = Config::align_mode_list;
    if (std::count(align_mode_list.begin(), align_mode_list.end(), new_align_mode))
    {
        this->align = new_align_mode;
    }
    else
    {
        std::cerr << "\"" << new_align_mode << "\" is not a valid align mode.\n";
    }
}

void Screen::set_mode(std::string new_rendering_mode)
{
    std::vector<std::string> mode_list = Config::rendering_mode_list;
    if (std::count(mode_list.begin(), mode_list.end(), new_rendering_mode))
    {
        this->mode = new_rendering_mode;
    }
    else
    {
        std::cerr << "\"" << new_rendering_mode << "\" is not a valid rendering mode.\n";
    }
}

int Screen::wait_select(bool reset)
{
    if (reset)
    {
        this->mark_pos = 0;
    }

    if (this->mode != "menu")
    {
        std::cerr << "Cannot wait for option selection when rendering mode is not \"menu\" (is \"" + this->mode + "\").\n";
        this->mode = "menu";
    }

    while (1)
    {
        this->render();
        std::string recv_key = this->get_key();

        if (recv_key == "w" || recv_key == "up")
        {
            this->mark_pos -= 1;
        }
        else if (recv_key == "s" || recv_key == "down")
        {
            this->mark_pos += 1;
        }
        else if (recv_key == "enter" || recv_key == "space")
        {
            return this->mark_pos;
        }
        this->mark_pos = (this->mark_pos + (int)this->options.size()) % (int)this->options.size();
    }
}

std::string read_string(std::ifstream &fin)
{
    std::string readed_string;
    while (1)
    {
        getline(fin, readed_string);
        if (readed_string != "")
        {
            break;
        }
    }
    return readed_string;
}

void Screen::load_block_texture()
{
    std::string file_path = "texture.txt";
    std::ifstream fin(file_path);
    if (fin)
    {
        int block_count;
        fin >> block_count;
        for (int i = 0; i < block_count; i++)
        {
            std::string block_name = read_string(fin);
            std::string block_texture = read_string(fin);
            this->block_texture_map[block_name] = block_texture;
            // std::cout << block_name << ": " << block_texture << "\n";
        }
    }
    else
    {
        std::cerr << "texture.txt is not found.\n";
        exit(1);
    }
}

std::vector<int> Screen::get_arrow(std::string recv_key)
{

    std::vector<int> recv_vector;
    if (recv_key == "up" || recv_key == "w")
    {
        recv_vector.push_back(-1);
        recv_vector.push_back(0);
    }
    else if (recv_key == "left" || recv_key == "a")
    {
        recv_vector.push_back(0);
        recv_vector.push_back(-1);
    }
    else if (recv_key == "down" || recv_key == "s")
    {
        recv_vector.push_back(1);
        recv_vector.push_back(0);
    }
    else if (recv_key == "right" || recv_key == "d")
    {
        recv_vector.push_back(0);
        recv_vector.push_back(1);
    }
    else
    {
        recv_vector.push_back(0);
        recv_vector.push_back(0);
    }
    return recv_vector;
}

void Screen::send_popup(std::string popup_message, bool wait)
{
    this->popup = true;
    this->popup_string = " [ " + popup_message + " ] ";
    this->render();
    if (wait)
    {
        this->wait(0.5);
        this->get_key();
    }
    this->popup = false;
}

int Screen::play_map(std::string map_name)
{
    this->init_map(Map(map_name));
    this->move_count = 0;
    while (1)
    {
        // render map and player stat
        this->render();
        // recv key
        std::string recv_key = this->get_key();
        if (recv_key == "esc")
        {
            this->send_popup("You quit the game.");
            return 0;
        }
        else if (recv_key == "r")
        {
            return this->play_map(map_name);
        }

        std::vector<int> recv_vector = this->get_arrow(recv_key);

        this->move_count++;
        this->map.player_move(recv_vector);

        if (this->map.check_win())
        {
            this->send_popup("You win!");
            return 1;
        }
    }
}

void Screen::toggle_align()
{
    if (this->align == "center")
    {
        this->set_align("left");
    }
    else
    {
        this->set_align("center");
    }
}

void Screen::toggle_show_border()
{
    this->show_border = !this->show_border;
}