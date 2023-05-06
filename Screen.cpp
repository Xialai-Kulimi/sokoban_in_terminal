#include "Screen.h"
#include "Config.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

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

void Screen::clear()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Screen::Screen()
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
    this->max_width = std::min(80, this->screen_column);
    this->align = "left";
    this->mode = "menu";
}

void Screen::fill(char texture)
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

std::string Screen::get_key()
{
#ifdef _WIN32
    int recv_key = _getch();
    // force exit (ctrl+c)
    if (recv_key == 3)
    {
        exit(0);
    }

    if (recv_key == 0)
    {
        // the arrow key was pressed
        recv_key = _getch();
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
        }

        return std::string(1, (char)recv_key);
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

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);

        int res = select(fileno(stdin) + 1, &set, NULL, NULL, &tv);

        if (res > 0)
        {

            // printf("Input available\n");
            read(fileno(stdin), &recv_char, 3);
            // printf("%s_%d_%d_%d_\n", recv_char, recv_char[0], recv_char[1], recv_char[2]);
            break;
        }
        else if (res < 0)
        {
            // perror("select error");
        }
        else
        {
            // printf("Select timeout\n");
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

std::vector<int> Screen::get_arrow()
{
}

void Screen::test()
{
    this->fill('X');
    for (int i = 0; i < 10; i++)
    {
        std::string recv_string = this->get_key();
        std::cout << "get_key: " << recv_string << std::endl;
    }
    for (int i = 0; i < 10; i++)
    {
        std::string recv_string = this->get_key();
        for (int j = 0; j < (int)recv_string.length(); j++)
        {
            this->fill(recv_string[j]);
            this->wait(0.1);
        }
    }
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
    this->title = new_title;
    this->description = new_description;
    this->question = new_question;
    this->options.clear();
    this->mark_pos = 0;
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

void Screen::clean_base()
{
    this->base_output.clear();
}

void Screen::add_base(std::string newline, bool center = false)
{
    if (newline == "")
    {
        this->base_output.push_back("");
        return;
    }

    if (center && newline.length() > this->max_width)
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
            this->base_output.push_back(newline.substr(i * this->max_width, this->max_width));
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

    int margin_y = std::max((this->screen_row - (int)this->base_output.size()) / 2 + 1, 0);
    int margin_x = std::max((this->screen_column - max_width) / 2 - 1, 0);
    std::string content = "";

    for (int i = 0; i < margin_y; i++)
    {
        content = content + std::string(this->screen_column, ' ') + "\n";
    }
    for (int i = 0; i < (int)this->base_output.size(); i++)
    {
        content = content + std::string(margin_x, ' ') + this->base_output[i] + std::string(margin_x, ' ') + "\n";
    }

    for (int i = 0; i < margin_y - 1; i++)
    {
        content = content + std::string(this->screen_column, ' ') + "\n";
    }
    content = content + std::string(this->screen_column, ' ');
    std::cout << content;
}

void Screen::render()
{
    if (this->mode == "menu")
    {
        this->render_menu();
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

int Screen::wait_select()
{
    if (this->mode != "menu")
    {
        std::cerr << "Cannot wait for option selection when rendering mode is not \"menu\".\n";
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