#ifndef JOB_H
#define JOB_H

#include <string>

class Job
{
public:
    Job();
    std::string job_number;
    std::string city;
    std::string state;
    std::string get_location_path();
    std::string get_gis_path();
    std::string get_workspace_path();
};

#endif // JOB_H
