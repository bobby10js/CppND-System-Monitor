#include <sstream>
#include <string>
#include "process.h"
#include "linux_parser.h"
using std::string;
std::vector<long> cpudata{0,0};

Process::Process (int pid,int uid,int vmsize,string state){
    Process::pid =pid;
    Process::uid = uid;
    Process::vmsize = vmsize;
    Process::state = state;
    std::vector <long> data = LinuxParser::CpuUtilization(pid);
    if(data[1]!=cpudata[1])
        Process::cpuutilization = (float)data[0]/data[1];
    else 
        Process::cpuutilization = 0;
    cpudata = data;
    uptime = cpudata[1];

}

// TODO: Return this process's ID
int Process::Pid() { return pid; }


// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuutilization; }

// TODO: Return the command that generated this process
string Process::Command() { 
   return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::GetRam() { 
return std::to_string( Process::vmsize/1024); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(uid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return  Process::uptime;  }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }