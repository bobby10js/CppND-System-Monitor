#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <bits/stdc++.h> 
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version,kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime,idletime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime  >> idletime;
  }
  return uptime;
}



// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line ,id;
  float total = 1,avail=0;
  int flag=0;
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> id;
      if(id ==  "MemTotal:"){
        linestream >>total;
        flag++;
        }
      else if(id == "MemAvailable:"){
         linestream >> avail;
        flag++;
      }
      else if(flag == 2){
        break;
      }
    }
  }
  float usage  =1 - avail/total;
  return  usage;
}



// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<vector <int>>  LinuxParser::CpuUtilization() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line ,id;
  int value;
  vector<vector <int>> data;
  if (stream.is_open()) {  
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >>id;
      if (id.find("cpu") != string::npos && id.length()==4 ) {
        vector <int> t;
        while(linestream >> value)
          t.push_back(value);
        data.push_back(t);
      }      
    }
  }
  return data;
}

// TODO: Read and return the total number of processes
// int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
// int LinuxParser::RunningProcesses() { return 0; }


int LinuxParser::ProcessesCount(int flag) {  
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line ,id, fstring; 
  int value= 0;
  if(flag == 1)
    fstring = "processes";  
  else if (flag == 0)
    fstring = "procs_running";
  else if (flag == -1)
    fstring = "procs_blocked";
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> id;
      if(id == fstring){
        linestream>> value;
        break;
      }
    }
  }
  return value;
  }


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory +to_string(pid)+kCmdlineFilename);
  std::string cmd,line;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >>cmd;
  }
  return cmd;
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Ram(int pid[[maybe_unused]]) { return string();}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int uid) { 
  std::ifstream stream(kPasswordPath);
  string line ,id,user="N.A",tUid;
  if (stream.is_open()) {  
    while(std::getline(stream, line)){
      std::istringstream ss(line);
      std::getline(ss, user, ':');
      std::getline(ss, tUid, ':');
      std::getline(ss, tUid, ':');
      if(std::stoi(tUid) == uid)
        break;
      }      
    }
  return user;
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function


vector<long> LinuxParser::CpuUtilization(int pid) { 
  std::ifstream stream(kProcDirectory +to_string(pid)+kStatFilename);
  std::string line,value;
  long utime,stime,cutime,cstime,starttime,upTime=0,total_time=0;
  if (stream.is_open()) {
    int i = 1;
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >>value){
      if(i==14){
        utime = std::stol(value);
      }
      else if(i==15){
        stime = std::stol(value);
      }
      else if(i==16){
        cutime = std::stol(value);
      }
      else if(i==17){
        cstime = std::stol(value);
      }
      else if(i==22){
        starttime = std::stol(value);
        break;
      }
      i++;
    }
    try{
      total_time= (utime + stime+ cutime + cstime)/sysconf(_SC_CLK_TCK);
      upTime = LinuxParser::UpTime() - starttime/sysconf(_SC_CLK_TCK);
    }
    catch(const std::exception& e){
      // std::cerr << e.what() << " ::"<< value <<"::"<< '\n';
    }
  }
  return  vector<long>{total_time,upTime};
}