#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ConvertNumberToString(const long number);
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif