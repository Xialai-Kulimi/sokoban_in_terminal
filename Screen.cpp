#include "Screen.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
// Assume POSIX
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#endif

using namespace std;

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

void Screen::fill(char texture){
    for (int i = 0; i < this->screen_row; i++)
    {
        for (int j = 0; j < this->screen_column; j++)
        {
            cout << texture;
        }
        if (i != this->screen_row-1)
        {
            cout << '\n';
        }
    }
    
}