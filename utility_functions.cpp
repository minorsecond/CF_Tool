#include "utility_functions.h"

#include <filesystem>
#include <string>

#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#include <iostream>

#pragma comment(lib,"Shell32")
#pragma comment(lib,"Ole32")

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

    const std::string home_path {get_home_path()};
    const std::string desktop_path {home_path + "\\Desktop\\"};
    const std::string workspace_path {desktop_path + "Workspaces"};
    const std::string deliverable_path {desktop_path + "\\Deliverables"};

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
            unzip_file(search_path, job_num);
        }
    }
}

void UtilityFunctions::unzip_file(const std::string path, const std::string job_num) {
    /*
     * Unzip the zip file located at path.
     * @param path: The path to the zip file
     */

    // TODO: use ZipLib library to unzip files
}

void UtilityFunctions::zip_file(const std::string folder_path, const std::string job_num) {
    /*
     * Compress files in directory into a zip file. Files will have timestamp in name,
     * in the format YYYY-MM-DD_JOBNUM.zip
     * @param folder_path: The source directory to zip up.
     * @param job_num: Job ID that will be used in resultant file name.
     */

    // TODO: Finish this method
}
