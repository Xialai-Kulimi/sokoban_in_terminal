#include "Screen.h"
#include <iostream>
#include <string>
#include <stdlib.h>

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
    return "";

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
    return "";

#endif
}

int Screen::get_up_or_down()
{
    std::string recv_key = this->get_key();
    if (recv_key == "w" || recv_key == "up")
    {
        return 1;
    }
    else if (recv_key == "s" || recv_key == "down")
    {
        return -1;
    }
    return 0;
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
        std::cout << (recv_string == " ") << std::endl;
        std::cout << (recv_string == "\n") << std::endl;
    }

}