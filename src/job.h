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
    std::string get_location_path() const;
    std::string new_gis_path() const;
    std::string find_gis_path() const;
    std::string new_workspace_path() const;
    std::string get_workspace_path() const;
    std::string get_deliverable_path() const;
};

#endif // JOB_H
