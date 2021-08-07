#include "job.h"
#include "utility_functions.h"

#include <string>
#include <filesystem>
#include <iostream>
#include <fileapi.h>

Job::Job(std::string job_id_val, std::string city_val, std::string state_val)
    : job_id{job_id_val}, city{city_val}, state{state_val} {
}

std::string Job::get_job_id() const {
    /*
     * Gets the job ID
     */

    return job_id;
}

std::string Job::get_city() const {
    /*
     * Gets the city name
     */

    return city;
}

std::string Job::get_state() const {
    /*
     * Gets the state
     */

    return state;
}

std::string Job::new_gis_path() const {
    /*
     * Gets the GIS working path
     */
    UtilityFunctions ut;
    const std::string home_path {ut.get_home_path()};
    const std::string date {ut.get_local_date()};
    std::string gis_path {"PATHNOTFOUND"};
    const std::string dir_structure {home_path + "\\Documents\\Comsof_Jobs\\" + state + "\\" + city};
    gis_path = dir_structure + "\\" + date.c_str() + "-" + job_id.c_str();

    return gis_path;
}

std::string Job::new_workspace_path() const {
    /*
     * Generate new workspace path
     */

    UtilityFunctions ut;
    const std::string date {ut.get_local_date()};
    return ut.get_home_path() + "\\Desktop\\Workspaces\\" + state + "\\" + city + "\\" + date.c_str() + "-" + job_id.c_str();
}

std::string Job::get_workspace_path() const {
    /*
     * Gets the Comsof workspace path
     */
    UtilityFunctions ut;
    const std::string workspace_base {ut.get_home_path() + "\\Desktop\\Workspaces\\" + state + "\\" + city};

    for (const auto & entry : std::filesystem::directory_iterator(workspace_base)) { // Documents path level
        if (ut.search_string_for_substring(entry.path().string(), job_id)) {
            return entry.path().string();
        }
    }

    return "PATHNOTFOUND";
}

std::string Job::get_location_path() const {
    /*
     * Get the city state path located within the documents directory
     */

    UtilityFunctions ut;
    const std::string home_path {ut.get_home_path()};
    return home_path + "\\Documents\\Comsof_Jobs\\" + state.c_str() + "\\" + city.c_str();
}

std::string Job::find_gis_path() const {
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
    const std::string download_path {ut.get_home_path() + "\\Documents\\Comsof_Jobs"};
    for (const auto & entry : std::filesystem::directory_iterator(download_path)) { // Iterate over states
        if (std::filesystem::is_directory(entry)) {  // Don't iterate over files
            for (const auto & city : std::filesystem::directory_iterator(entry.path())) {  // Iterate over Cities
                if (std::filesystem::is_directory(city)) {
                    for (const auto & job : std::filesystem::directory_iterator(city.path())) {  // Iterate over Jobs
                        std::string search_path = job.path().string();
                        std::cout << "SEARCH PATH: " << search_path << std::endl;
                        if (ut.search_string_for_substring(search_path, job_id)) {
                            return search_path;  //TODO: only find files with .zip in the name
                        }
                    }
                }
            }
        }
    }

    return "FILENOTFOUND";
}

std::string Job::get_deliverable_path() const {
    /*
     * Get path to deliverable
     */

    UtilityFunctions ut;
    return ut.get_home_path() + "\\Desktop\\Deliverables\\" + state + "\\" + city;
}
