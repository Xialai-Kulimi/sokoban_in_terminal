#include "Profile.h"
#include <fstream>

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

            if (readed_string == "")
            {
                break;
            }
            else
            {
                char key[100];
                int value, step;
                sscanf(readed_string.c_str(), "%s %d %d", key, &value, &step);
                std::string string_key = key;

                if (string_key.substr(string_key.find_last_of(".") + 1) == "txt")
                {
                    // write record to map_record
                    std::vector<int> play_record;
                    play_record.push_back(value);
                    play_record.push_back(step);
                    this->map_record[string_key] = play_record;
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
        this->setting_map["show_border"] = 0;
        this->setting_map["align"] = 0;
        this->setting_map["frame_rate"] = 10;
        this->setting_map["max_width"] = 81;
        this->save();
    }
}

int Profile::read_setting(std::string key)
{
    return this->setting_map[key];
}

std::vector<int> Profile::read_play_record(std::string map_name)
{
    return this->map_record[map_name];
}

void Profile::write_setting(std::string setting_key, int setting_value)
{
    this->setting_map[setting_key] = setting_value;
}

void Profile::update_record(std::string map_name, int step)
{
    std::vector<int> play_record = this->map_record[map_name];
    if (play_record.size() == 0)
    {
        play_record.push_back(0);
        play_record.push_back(step);
    }
    play_record[0] = play_record[0] + 1;
    if (step < play_record[1])
    {
        play_record[1] = step;
    }

    this->map_record[map_name] = play_record;
}

void Profile::save()
{
    std::ofstream fout(".sokoban_profile");
    fout.close();
}
