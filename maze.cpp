#include <iostream>
#include <fstream>
#include <vector>

#include <algorithm>
#include <random>
#include <ctime>

#define SIZE 31

using namespace std;

int maze[SIZE][SIZE] = {0};
int rand_count = 0;

void grow(int row, int column)
{
    maze[row][column] = 1;
    vector<vector<int> > choices;
    vector<int> unit_vector;
    unit_vector.clear();
    unit_vector.push_back(1);
    unit_vector.push_back(0);
    choices.push_back(unit_vector);
    unit_vector.clear();
    unit_vector.push_back(-1);
    unit_vector.push_back(0);
    choices.push_back(unit_vector);
    unit_vector.clear();
    unit_vector.push_back(0);
    unit_vector.push_back(1);
    choices.push_back(unit_vector);
    unit_vector.clear();
    unit_vector.push_back(0);
    unit_vector.push_back(-1);
    choices.push_back(unit_vector);
    shuffle(choices.begin(), choices.end(), default_random_engine(time(0) + rand_count++));

    for (int i = 0; i < 4; i++)
    {
        int new_row = row + choices[i][0] * 2;
        int new_column = column + choices[i][1] * 2;
        if (maze[new_row][new_column] == 0)
        {
            maze[row + choices[i][0]][column + choices[i][1]] = 1;
            grow(new_row, new_column);
        }
    }
}

int main()
{
    // init maze
    for (int i = 0; i < SIZE; i++)
    {
        maze[0][i] = 1;
        maze[SIZE - 1][i] = 1;
        maze[i][0] = 1;
        maze[i][SIZE - 1] = 1;
    }
    grow(8, 8);
    // save maze
    ofstream fout("output_c.txt");
    fout.clear();
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (maze[i][j] == 0)
            {
                fout << "/";
            }
            else
            {
                fout << "-";
            }
        }
        fout << "\n";
    }
    fout.close();
}
