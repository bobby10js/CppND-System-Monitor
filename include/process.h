#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
   public:
      Process (int ,int ,int ,std::string );
      int Pid();                               // TODO: See src/process.cpp
      std::string User();                      // TODO: See src/process.cpp
      std::string Command();                   // TODO: See src/process.cpp
      float CpuUtilization();                  // TODO: See src/process.cpp
      std::string GetRam();                       // TODO: See src/process.cpp
      long int UpTime();                       // TODO: See src/process.cpp
//       bool operator<(Process const& a) const;  // TODO: See src/process.cpp
   // TODO: Declare any necessary private members
   public:
      int vmsize; // in KB
      int pid;
      int uid;
      long uptime;
      float cpuutilization;
      std::string state;


};

#endif