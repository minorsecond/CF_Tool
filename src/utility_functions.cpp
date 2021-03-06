#include "utility_functions.h"
#include "include/elzip.hpp"
#include "ui/errorwindow.h"

#include <filesystem>
#include <string>
#include <chrono>

#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#include <iostream>

void UtilityFunctions::create_directories() {
    /*
     * Creates the deliverable, working, and workspace directories if they don't exist.
     * @param username: The user's username. This detemrines where the directories are created.
     */

    const std::string desktop_path {get_home_path() + "\\Desktop\\"};
    const std::string workspace_path {desktop_path + "Workspaces"};
    const std::string deliverable_path {desktop_path + "Deliverables"};
    const std::string gis_path {get_home_path() + "\\Documents\\Comsof_Jobs"};

    std::cout << deliverable_path << std::endl;
    const std::vector<std::string> paths {workspace_path, deliverable_path, gis_path};
    for (const std::string &path : paths) {
        CreateDirectoryA(path.c_str(), NULL);
    }
}

std::string UtilityFunctions::get_home_path() {
    /*
     * Get the current's user's desktop path.
     */

  WCHAR path[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {

      // Convert to string
      std::wstring ws(path);
      std::string str(ws.begin(), ws.end());
      return str;
  }

  return "PATHNOTFOUND";
}

bool UtilityFunctions::search_string_for_substring(const std::string a, const std::string b) {
    /*
     * Search string a for occurance of substring b
     * @param a: String which to search
     * @param b: string which to search for
     * @return: Boolean denoting whether substring was found or not.
     */

    if (a.find(b) != std::string::npos) {
        return true;
    }

    return false;
}

void UtilityFunctions::unzip_file(const std::string path) {
    /*
     * Unzip the zip file located at path.
     * @param path: The path to the zip file
     */


    const std::string target {get_home_path() + "\\Downloads\\tmp"};
    CreateDirectoryA(target.c_str(), NULL);
    std::cout << "Extracting " << path << " to " << target << std::endl;
    elz::extractZip(path, target);

    // Add a _ to end of file to mark it as having been processed
    // First, get file path without .zip extension
    const size_t lastindex {path.find_last_of(".")};
    std::string naked_path {path.substr(0, lastindex)};
    // Next, add the _
    naked_path += "_.zip";
    std::filesystem::rename(path, naked_path);
}

void UtilityFunctions::zip_files(Job jobinfo) {
    /*
     * Compress files in directory into a zip file. Files will have timestamp in name,
     * in the format YYYY-MM-DD_JOBNUM.zip
     * @param folder_path: The source directory to zip up.
     * @param job_num: Job ID that will be used in resultant file name.
     */

    const std::string date {get_local_date()};
    const std::string city_state_path {jobinfo.get_deliverable_path()};
    const std::string tmp_path {city_state_path + "\\tmp"};
    const std::string const_base_path {tmp_path};
    const std::string target {city_state_path + "\\" + date.c_str() + "-" + jobinfo.get_job_id().c_str() + ".zip"};
    const std::wstring const_base_path_ws {std::wstring(const_base_path.begin(), const_base_path.end())};

    // Add new filenames (without extension) to this array as needed.
    const std::array<std::string, 6> deliverable_files {"OUT_AccessStructures",
                                                        "OUT_Closures",
                                                        "OUT_DistributionCables",
                                                        "OUT_DropCables",
                                                        "OUT_DropClusters",
                                                        "OUT_FeederCables"};

    std::cout << "Scanning " << jobinfo.get_workspace_path() << " for job files" << std::endl;
    create_directory_recursively(const_base_path_ws); // Temp directory to store needed files before zipping
    for (const auto & job_dirs : std::filesystem::directory_iterator(jobinfo.get_workspace_path())) {
        std::cout << job_dirs.path().string() << std::endl;
        if (search_string_for_substring(job_dirs.path().string(), "output")) {
            for (const auto & file : std::filesystem::directory_iterator(job_dirs)) {
                std::string filename {file.path().filename().string()};
                const size_t lastindex {filename.find_last_of(".")};
                const std::string naked_filename {filename.substr(0, lastindex)};
                if (std::find(deliverable_files.begin(), deliverable_files.end(), naked_filename) != deliverable_files.end()) {
                    filename.insert(3, "_" + jobinfo.get_job_id());
                    const std::string out_path {const_base_path + "\\" + filename};
                    try {
                        std::filesystem::copy(file.path().string(), out_path); // Copy files to temp dir
                    }  catch (std::filesystem::filesystem_error) {
                        std::cout << "Output file probably already exists.";
                    }
                }
            }
        }
    }

    std::cout << "Compressing files in " << tmp_path << " to " << target << std::endl;
    elz::zipFolder(tmp_path, target);
    std::filesystem::remove_all(tmp_path);
}

std::string UtilityFunctions::get_local_date() {
    /*
     * Get date in localtime
     * @return: A string denoting the date in localtime, with the format YYYYMMDD
     */

    // Get today's date in local time.
    char query_date[10];
    std::string output;

    const auto todays_date = std::chrono::system_clock::now();
    const auto now_c = std::chrono::system_clock::to_time_t(todays_date);
    const std::tm now_tm = *std::localtime(&now_c);
    std::strftime(query_date, sizeof query_date, "%Y%m%d", &now_tm);

    for (char i : query_date) {
        output += std::string(1, i);
    }

    return output;
}

std::string UtilityFunctions::find_zip_file(Job jobinfo) {
    /*
     * Finds zip file in Downloads directory that contains job_number
     * @param job_number: Job number to search for
     */

    const std::string download_path {get_home_path() + "\\Downloads\\"};
    for (const auto & entry : std::filesystem::directory_iterator(download_path)) {
        const std::string search_path = entry.path().string();
        if (search_string_for_substring(search_path, jobinfo.get_job_id())  && search_string_for_substring(search_path, ".zip")) {
            return search_path;  //TODO: only find files with .zip in the name
        }
    }

    return "FILENOTFOUND";
}

void UtilityFunctions::move_extracted_files(Job jobinfo) {
    /*
     * Move all of the extracted shapefiles from _tmp into working directory
     * @param job_num: Job ID. This will be used to create the working directory
     */
    ErrorWindow er;
    const std::string home {get_home_path()};
    const std::string tmp_dir {home + "\\Downloads\\tmp"};

    if (!jobinfo.get_job_id().empty() && !jobinfo.get_city().empty()) {
        const std::string out_path {jobinfo.new_gis_path()};
        const std::string reproj_path {out_path + "\\reprojected"};
        std::cout << "Moving to working dir " << out_path << std::endl;

        // Convert string to ws
        const std::wstring reproj_path_ws {std::wstring(reproj_path.begin(), reproj_path.end())};

        //_wrename(tmp_dir_wt, out_path_wt);
        try {
           std::filesystem::rename(tmp_dir, out_path);
        }  catch (std::filesystem::filesystem_error) {
            er.set_error_message("Error: path already exists: " + out_path);
            er.exec();
            std::cout << out_path << " already exists" << std::endl;
            std::filesystem::remove_all(tmp_dir);
        }

        create_directory_recursively(reproj_path_ws);
    }
}

void UtilityFunctions::create_directory_recursively(const std::wstring &directory) {
    /*
     * Create directories and subdirectories if they don't exist
     * @param directory: The path to create
     */

    static const std::wstring separators(L"\\/");

      // If the specified directory name doesn't exist, do our thing
      DWORD fileAttributes = ::GetFileAttributesW(directory.c_str());
      if(fileAttributes == INVALID_FILE_ATTRIBUTES) {

        // Recursively do it all again for the parent directory, if any
        std::size_t slashIndex = directory.find_last_of(separators);
        if(slashIndex != std::wstring::npos) {
          create_directory_recursively(directory.substr(0, slashIndex));
        }

        // Create the last directory on the path (the recursive calls will have taken
        // care of the parent directories by now)
        std::wcout << "Attempting to create " << directory << std::endl;
        BOOL result = ::CreateDirectoryW(directory.c_str(), nullptr);
        if(result == FALSE) {
          throw std::runtime_error("Could not create directory");
        }

      } else { // Specified directory name already exists as a file or directory

        bool isDirectoryOrJunction =
          ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
          ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if(!isDirectoryOrJunction) {
          throw std::runtime_error(
            "Could not create directory because a file with the same name exists"
          );
        }
      }
}

void UtilityFunctions::build_working_dirs(Job jobinfo) {
    /*
     * Create workspace and working directory
     * @param job_num: The job number to be used in directory names
     */

    const std::string location_path {jobinfo.get_location_path()};
    const std::string work_path {jobinfo.new_workspace_path()};

    const std::wstring location_path_ws {std::wstring(location_path.begin(), location_path.end())};
    const std::wstring work_path_ws {std::wstring(work_path.begin(), work_path.end())};

    create_directory_recursively(location_path_ws);
    create_directory_recursively(work_path_ws);
}

bool UtilityFunctions::file_exists(const std::string &path) {
    /*
     * Check if file exists
     * @param path: Path of file to check for
     */
    bool exists {false};
    std::filesystem::exists(path) ? exists = true : exists = false;
    return exists;
}

void UtilityFunctions::copy_files_in_dir(std::string in_dir, std::string out_dir) {
    /*
     * Copies all files in in_dir to out_dir.
     * @param in_dir: Directory containing files to copy.
     * @param out_dir: Destination directory.
     */

    for (const auto & file : std::filesystem::directory_iterator(in_dir)) {
        const std::string filename {file.path().filename().string()};
        const size_t lastindex {filename.find_last_of(".")};
        const std::string dest_path {out_dir + "\\" + filename};
        if (!search_string_for_substring(file.path().string(), "reprojected")) {
            std::filesystem::copy(file.path().string(), dest_path);
        }
    }
}
