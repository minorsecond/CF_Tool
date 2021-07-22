#include "job.h"
#include "utility_functions.h"

#include <string>

Job::Job()
{
}

std::string Job::get_gis_path() {
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
