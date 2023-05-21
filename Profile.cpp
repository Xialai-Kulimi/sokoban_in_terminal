#include "Profile.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#define DEBUG false

Profile::Profile()
{
    this->load();
}

Profile::~Profile()
{
    this->save();
}

void Profile::load()
{

    std::ifstream fin(".sokoban_profile");
    if (fin)
    {
        while (1)
        {
            std::string readed_string;
            getline(fin, readed_string);
            if (DEBUG)
            {
                std::cout << "read from .sokoban_file: " << readed_string << std::endl;
            }

            if (readed_string == "")
            {
                break;
            }
            else
            {
                char key[100];
                int value;
                sscanf(readed_string.c_str(), "%s %d", key, &value);
                std::string string_key = key;
                // std::cout << "read: " << key << " " << value << " " << step << "\n";

                if (string_key.substr(string_key.find_last_of(".") + 1) == "txt")
                {
                    // write record to map_record
                    this->map_record[string_key] = value;
                }
                else
                {
                    // write setting to settig_map
                    this->setting_map[string_key] = value;
                }
            }
        }
        fin.close();
    }
    else
    {
// .sokoban_profile does not exist.
// generate default value and save.
#ifdef _WIN32
        this->setting_map["show_border"] = 0;
#else
        // Assume POSIX
        this->setting_map["show_border"] = 1;
#endif
        this->setting_map["align_center"] = 0;
        this->setting_map["frame_rate"] = 10;
        this->setting_map["default_max_width"] = 81;
        this->save();
    }
}

int Profile::read_setting(std::string key)
{
    if (this->setting_map.count(key) > 0)
    {
        return this->setting_map[key];
    }
    else
    {
        std::cerr << "\"" << key << "\" is not found in setting_map\n";
        exit(1);
    }
}

int Profile::read_play_record(std::string map_name)
{
    return this->map_record[map_name];
}

void Profile::write_setting(std::string setting_key, int setting_value)
{
    this->setting_map[setting_key] = setting_value;
    this->save();
}

void Profile::update_record(std::string map_name, int step)
{

    if (step > 0)
    {
        if (step < this->map_record[map_name] || this->map_record[map_name] == 0)
        {
            this->map_record[map_name] = step;
            this->save();
        }
    }
}

void Profile::save()
{
    std::ofstream fout(".sokoban_profile");
    fout.clear();
    for (std::map<std::string, int>::iterator it = this->setting_map.begin(); it != this->setting_map.end(); ++it)
    {
        fout << it->first << " " << it->second << "\n";
    }
    for (std::map<std::string, int>::iterator it = this->map_record.begin(); it != this->map_record.end(); ++it)
    {
        fout << it->first << " " << it->second << "\n";
    }
    fout.close();
}
