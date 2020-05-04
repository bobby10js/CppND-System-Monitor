#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
    
    }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {  
    processes_.clear();

    for (int pid : LinuxParser::Pids()){

        std::ifstream stream("/proc/" +std::to_string(pid)+ "/status");
        string line ,id,state="N.A";
        int uid = -1,vmsize=-1;
        if (stream.is_open()) {  
            while(std::getline(stream, line)){
                std::istringstream linestream(line);
                linestream >>id;
                if(id =="State:"){
                    linestream>>state;
                    }
                else if(id =="Uid:"){
                        linestream>>uid;
                }
                else if(id =="VmSize:"){
                    linestream>>vmsize;
                    break;
                }
            }
        }
        Process p(pid,uid,vmsize,state);
        processes_.push_back(p);
    }
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel();
    }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
    }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::ProcessesCount(0); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::ProcessesCount(1); }
// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    uptime =  LinuxParser::UpTime();
    return uptime;
    }
long int System::GetUpTime(){
    return uptime;
} 

// bool attributeSort(const RouteModel::Node& lhs, const RouteModel::Node& rhs) {
// float leftTot = lhs.g_value + lhs.h_value;
// float rigTot = rhs.g_value + rhs.h_value;
// return leftTot < rigTot;
// }

// void SortProcesses(){
//     sort(processes_.begin(), processes_.end(), attributeSort); 
//     ;}
