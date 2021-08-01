#ifndef JOB_H
#define JOB_H

#include <string>

class Job
{
public:
    Job(std::string job_id_val = "None", std::string city_val = "None", std::string state_val = "None");
    std::string job_id;
    std::string city;
    std::string state;
    std::string get_location_path();
    std::string new_gis_path();
    std::string find_gis_path();
    std::string new_workspace_path();
    std::string get_workspace_path();
    std::string get_deliverable_path();
};

#endif // JOB_H
