#include <vector>
#include <string>

class Config
{
private:

public:
    std::vector<std::string> block_type_list;
    Config();
};

Config::Config()
{
    block_type_list.push_back("wall");
    block_type_list.push_back("air");
    block_type_list.push_back("player");
    block_type_list.push_back("box");
    block_type_list.push_back("end");
}

Config config;
