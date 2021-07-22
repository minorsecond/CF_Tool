#include "job.h"
#include "utility_functions.h"

#include <string>
#include <filesystem>

Job::Job()
{
}

std::string Job::new_gis_path() {
    /*
     * Gets the GIS working path
     */
    UtilityFunctions ut;
    const std::string home_path {ut.get_home_path()};
    const std::string date {ut.get_local_date()};
    std::string gis_path {"PATHNOTFOUND"};
    const std::string dir_structure {home_path + "\\Documents\\Comsof_Jobs\\" + state + "\\" + city};
    gis_path = dir_structure + "\\" + date.c_str() + "-" + job_number.c_str();

    return gis_path;
}

std::string Job::get_workspace_path() {
    /*
     * Gets the Comsof workspace path
     */
    UtilityFunctions ut;
    const std::string home_path {ut.get_home_path()};
    const std::string date {ut.get_local_date()};
    return home_path + "\\Workspaces\\" + state + "\\" + city + "\\" + date.c_str() + "-" + job_number.c_str();
}

std::string Job::get_location_path() {
    /*
     * Get the city state path located within the documents directory
     */

    UtilityFunctions ut;
    const std::string home_path {ut.get_home_path()};
    return home_path + "\\Documents\\Comsof_Jobs\\" + state.c_str() + "\\" + city.c_str();
}

std::string Job::find_gis_path() {
    /*
     * Finds path to GIS directory inside documents directory
     * @param job_number: The job number to search for
     * @return: The path as a string
     */

    /*
     * Finds zip file in Downloads directory that contains job_number
     * @param job_number: Job number to search for
     */
    UtilityFunctions ut;
    const std::string download_path {ut.get_home_path() + "\\Documents\\"};
    for (const auto & entry : std::filesystem::directory_iterator(download_path)) { // Documents path level
        if (ut.search_string_for_substring(entry.path().string(), "Comsof_Jobs")) {
            for (const auto & state : std::filesystem::directory_iterator(entry.path())) {  // Comsof Jobs level
                for (const auto & city : std::filesystem::directory_iterator(state.path())) {  // City level
                    for (const auto & job : std::filesystem::directory_iterator(city.path())) {
                        std::string search_path = job.path().string();
                        if (ut.search_string_for_substring(search_path, job_number)) {
                            return search_path;  //TODO: only find files with .zip in the name
                        }
                    }
                }
            }
        }
    }

    return "FILENOTFOUND";
}
