#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <string>

class UtilityFunctions
{
public:
    std::string get_workspace_path(const std::string job_num);
    void create_directories();
    std::string get_home_path();
    bool search_string_for_substring(const std::string a, const std::string b);
};

#endif // UTILITY_FUNCTIONS_H
