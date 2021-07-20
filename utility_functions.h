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
    void zip_file(const std::string input_path, const std::string output_path);
    void unzip_file(const std::string path);
    void zip_files(const std::string folder_path, const std::string job_num);
    std::string get_local_date();
    std::string find_zip_file(const std::string job_number);
    std::string find_gis_path(const std::string job_number);
    void move_extracted_files(const std::string job_num, const std::string city, const std::string state);
    void create_directory_recursively(const std::wstring &directory);
    void build_working_dirs(const std::string job_num, const std::string city, const std::string state);
    bool file_exists(const std::string &path);
};

#endif // UTILITY_FUNCTIONS_H
