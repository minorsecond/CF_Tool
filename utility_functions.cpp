#include "utility_functions.h"
#include "elzip.hpp"

#include <filesystem>
#include <string>
#include <chrono>

#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#include <iostream>
#include <unistd.h>

std::string UtilityFunctions::get_workspace_path(const std::string job_num) {
    /*
     * Recursively scan workspace root path to find path of specific job workspace.
     * @param job_num: The job identifier to search for
     * @param username: The user's username. This determines where the directories are found
     * @return workspace_path: The path to the job's workspace
     */

    std::string desktop_path {get_home_path()};
    std::cout << desktop_path << std::endl;
    std::string workspace_root {desktop_path + "\\Desktop\\Workspaces"};

    std::vector<std::string> invalid_directories {"input", "output", "Saved States"};

    // Search for the workspace directory that contains the job_num. The second for loop
    // makes sure that the input, output, or saved states subdirectory isn't selected.
    for (const auto & entry : std::filesystem::directory_iterator(workspace_root)) {
        std::string search_path = entry.path().string();  // Convert  fs path to string
        if (search_string_for_substring(search_path, job_num)) {
            for (const std::string &bad_dir : invalid_directories) {
                if (!search_string_for_substring(search_path, bad_dir)) {
                    return search_path;
                }
            }
        }
    }

    return "PATHNOTFOUND";
}

void UtilityFunctions::create_directories() {
    /*
     * Creates the deliverable, working, and workspace directories if they don't exist.
     * @param username: The user's username. This detemrines where the directories are created.
     */

    const std::string desktop_path {get_home_path() + "\\Desktop\\"};
    const std::string workspace_path {desktop_path + "Workspaces"};
    const std::string deliverable_path {desktop_path + "Deliverables"};

    std::cout << deliverable_path << std::endl;
    const std::vector<std::string> paths {workspace_path, deliverable_path};
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

void UtilityFunctions::process_downloaded_data(const std::string job_num) {
    /*
     * Extract downloaded zip file into temp directory.
     * @param job_num: The job number that should be extracted.
     */

    std::string downloads_path {get_home_path() + "\\Downloads"};
    for (const auto & entry : std::filesystem::directory_iterator(downloads_path)) {
        std::string search_path = entry.path().string();  // Convert  fs path to string
        if (search_string_for_substring(search_path, job_num)) {
            unzip_file(search_path);
        }
    }
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
}

void UtilityFunctions::zip_files(const std::string folder_path, const std::string job_num) {
    /*
     * Compress files in directory into a zip file. Files will have timestamp in name,
     * in the format YYYY-MM-DD_JOBNUM.zip
     * @param folder_path: The source directory to zip up.
     * @param job_num: Job ID that will be used in resultant file name.
     */

    const std::string date {get_local_date()};
    const std::string target {get_home_path() + "\\Desktop\\Deliverables\\" + date + "-" + job_num};
    std::cout << "Compressing files in " << folder_path << " to " << target << std::endl;
    elz::zipFolder(folder_path, target);
}

std::string UtilityFunctions::get_local_date() {
    /*
     * Get date in localtime
     * @return: A string denoting the date in localtime, with the format YYYYMMDD
     */

    // Get today's date in local time.
    char query_date[10];
    std::string output;

    auto todays_date = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(todays_date);
    std::tm now_tm = *std::localtime(&now_c);
    std::strftime(query_date, sizeof query_date, "%Y%m%d", &now_tm);

    for (char i : query_date) {
        output += std::string(1, i);
    }

    return output;
}

std::string UtilityFunctions::find_zip_file(const std::string job_number) {
    /*
     * Finds zip file in Downloads directory that contains job_number
     * @param job_number: Job number to search for
     */

    const std::string download_path {get_home_path() + "\\Downloads\\"};
    for (const auto & entry : std::filesystem::directory_iterator(download_path)) {
        std::string search_path = entry.path().string();
        if (search_string_for_substring(search_path, job_number)) {
            return search_path;
        }
    }

    return "FILENOTFOUND";
}

void UtilityFunctions::move_extracted_files(const std::string job_num, const std::string city, const std::string state) {
    /*
     * Move all of the extracted shapefiles from _tmp into working directory
     * @param job_num: Job ID. This will be used to create the working directory
     */
    const std::string home {get_home_path()};
    const std::string tmp_dir {home + "\\Downloads\\tmp"};
    const std::string date {get_local_date()};
    const std::string dir_structure {home + "\\Documents\\" + state + "\\" + city};
    const std::string out_path {dir_structure + "\\" + date.c_str() + "-" + job_num.c_str()};
    std::cout << "Moving to working dir " << out_path << std::endl;

    // Convert string to ws
    std::wstring dir_structure_ws {std::wstring(dir_structure.begin(), dir_structure.end())};

    std::cout << "Creating " << dir_structure << std::endl;
    create_directory_recursively(dir_structure_ws);

    //_wrename(tmp_dir_wt, out_path_wt);
    std::filesystem::rename(tmp_dir, out_path);
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
