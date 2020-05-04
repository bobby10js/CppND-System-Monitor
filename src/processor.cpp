#include <vector>
#include "linux_parser.h"
#include "processor.h"

using std::vector;

// TODO: Return the aggregate CPU utilization
float total(vector<int> data) {
    float total = 0;
    for (int i: data)
        total+=i;
    return total; 
}

vector<float> Processor::Utilization() { 
   vector<float> data;
   int j=0,N = LinuxParser::CpuUtilization().size();
    prevState.resize(N);
    for (vector <int> currentState :  LinuxParser::CpuUtilization()){
        float total= 0,idle=0,prevtotal= 0,prevIdle = 0;
        if (!prevState[j].empty()){
            for (size_t i = 0; i < prevState[j].size(); i++){
                total += currentState[i];
                prevtotal += prevState[j][i];
                if(i==3 or i==4){ // 3,4 entries corresponds to idle and wait of cpu 
                    idle += currentState[i];
                    prevIdle += prevState[j][i];
                }
            }
        }
        float res = (total == prevtotal)? 0.0 : 1 - (idle - prevIdle)/(total - prevtotal);
        prevState[j] = currentState;
        data.push_back(res);
        j++;
    }
    return data; 
}
