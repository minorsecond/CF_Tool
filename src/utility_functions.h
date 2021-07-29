#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include "job.h"
#include <string>

class UtilityFunctions
{
public:
    void create_directories();
    std::string get_home_path();
    bool search_string_for_substring(const std::string a, const std::string b);
    void unzip_file(const std::string path);
    void zip_files(Job jobinfo);
    std::string get_local_date();
    std::string find_zip_file(Job jobinfo);
    void move_extracted_files(Job jobinfo);
    void copy_files_in_dir(std::string in_dir, std::string out_dir);
    void create_directory_recursively(const std::wstring &directory);
    void build_working_dirs(Job jobinfo);
    bool file_exists(const std::string &path);
};

#endif // UTILITY_FUNCTIONS_H
