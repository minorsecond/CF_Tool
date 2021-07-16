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
    void process_downloaded_data(const std::string job_num);
    void unzip_file(const std::string path);
    void zip_files(const std::string folder_path, const std::string job_num);
    std::string get_local_date();
    std::string find_zip_file(const std::string job_number);
    void move_extracted_files(const std::string job_num);
};

#endif // UTILITY_FUNCTIONS_H
