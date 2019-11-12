//
// Created by congay on 12/11/2019.
//

#ifndef BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_
#define BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_

#include <string>
#include <iomanip>
#include <sstream>

namespace brutils
{
static std::string getTimeFormatted(std::string &format)
{
  std::ostringstream osDate;
  auto time = std::time(nullptr);
  osDate << std::put_time(std::gmtime(&time), format.c_str());
  return osDate.str();
}
static std::string getTimeFormatted(std::string &&format)
{
  std::ostringstream osDate;
  auto time = std::time(nullptr);
  osDate << std::put_time(std::gmtime(&time), format.c_str());
  return osDate.str();
}
static std::string getLocaltimeFormatted(std::string &format)
{
  std::ostringstream osDate;
  auto time = std::time(nullptr);
  osDate << std::put_time(std::localtime(&time), format.c_str());
  return osDate.str();
}
static std::string getLocaltimeFormatted(std::string &&format)
{
  std::ostringstream osDate;
  auto time = std::time(nullptr);
  osDate << std::put_time(std::localtime(&time), format.c_str());
  return osDate.str();
}
}

#endif //BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_
