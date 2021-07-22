#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include "job.h"
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
    void zip_files(Job jobinfo);
    std::string get_local_date();
    std::string find_zip_file(const std::string job_number);
    std::string find_gis_path(const std::string job_number);
    void move_extracted_files(Job jobinfo);
    void create_directory_recursively(const std::wstring &directory);
    void build_working_dirs(Job jobinfo);
    bool file_exists(const std::string &path);
};

#endif // UTILITY_FUNCTIONS_H
