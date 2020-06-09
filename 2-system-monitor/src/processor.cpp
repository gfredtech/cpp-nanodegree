#include "linux_parser.h"
#include "processor.h"

Processor::Processor() :
    _prevIdle(LinuxParser::IdleJiffies()),
    _prevNonIdle(LinuxParser::ActiveJiffies()),
    _prevTotal(_prevIdle + _prevNonIdle) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    const long currIdle = LinuxParser::IdleJiffies(); 
    const long currNonIdle = LinuxParser::ActiveJiffies(); 
    const long currTotal = currIdle + currNonIdle;

    const long deltaIdle = currIdle - _prevIdle;
    const long deltaTotal = currTotal - _prevTotal;

    _prevIdle = currIdle;
    _prevNonIdle = currNonIdle;
    _prevTotal = currTotal;
    
    const float cpuUsage = deltaTotal == 0 ? 0.0f : (deltaTotal - deltaIdle)*1.0/deltaTotal;
    return cpuUsage;
}