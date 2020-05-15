#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization(); 

 private:
    long prevActiveUsage = 0;
    long prevTotalUsage = 0;
};

#endif