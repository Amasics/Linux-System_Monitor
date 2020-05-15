#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    long activeUsage = LinuxParser::ActiveJiffies() - prevActiveUsage;
    long totalUsage = LinuxParser::Jiffies() - prevTotalUsage;
    prevActiveUsage = activeUsage;
    prevTotalUsage = totalUsage;
    return 1.0f * activeUsage / totalUsage;
}