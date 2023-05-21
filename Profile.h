#include <string>
#include <map>
#include <vector>

class Profile
{
private:
    void load();
    std::map<std::string, int> setting_map;
    std::map<std::string, int > map_record;
public:
    Profile();
    ~Profile();

    void save();

    int read_setting(std::string);
    int read_play_record(std::string);

    void write_setting(std::string, int);
    void update_record(std::string, int);

};
