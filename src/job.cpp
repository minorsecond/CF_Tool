#include "job.h"
#include "utility_functions.h"

#include <string>
#include <filesystem>
#include <iostream>

Job::Job(std::string job_id_val)
    : job_id{job_id_val} {

}

Job::Job(std::string job_id_val, std::string city_val, std::string state_val)
    : job_id{job_id_val}, city{city_val}, state{state_val} {
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
    gis_path = dir_structure + "\\" + date.c_str() + "-" + job_id.c_str();

    return gis_path;
}

std::string Job::new_workspace_path() {
    /*
     * Generate new workspace path
     */

    UtilityFunctions ut;
    const std::string date {ut.get_local_date()};
    return ut.get_home_path() + "\\Desktop\\Workspaces\\" + state + "\\" + city + "\\" + date.c_str() + "-" + job_id.c_str();
}

std::string Job::get_workspace_path() {
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
    const std::string download_path {ut.get_home_path() + "\\Documents\\Comsof_Jobs"};
    for (const auto & entry : std::filesystem::directory_iterator(download_path)) { // Documents path level
        std::cout << entry.path().string() << std::endl;
        for (const auto & city : std::filesystem::directory_iterator(entry.path())) {  // City level
            for (const auto & job : std::filesystem::directory_iterator(city.path())) {
                std::string search_path = job.path().string();
                if (ut.search_string_for_substring(search_path, job_id)) {
                    return search_path;  //TODO: only find files with .zip in the name
                }
            }
        }
    }

    return "FILENOTFOUND";
}

std::string Job::get_deliverable_path() {
    /*
     * Get path to deliverable
     */

    UtilityFunctions ut;
    return ut.get_home_path() + "\\Desktop\\Deliverables\\" + state + "\\" + city;
}
