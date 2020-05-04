#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>


class Processor {
 public:
  std::vector<float> Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
  private:
    std::vector<std::vector<int>> prevState;
  
 };

#endif