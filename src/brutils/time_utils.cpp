//
// Created by Burak Ongay on 05/07/2020.
//

#include "brutils/time_utils.h"

std::string brutils::getTimeFormatted(std::string& format)
{
  std::ostringstream osDate;
  auto time = std::time(nullptr);
  osDate << std::put_time(std::gmtime(&time), format.c_str());
  return osDate.str();
}
std::string brutils::getTimeFormatted(std::string&& format)
{
  return getTimeFormatted(format);
}
std::string brutils::getLocaltimeFormatted(std::string& format)
{
  std::ostringstream osDate;
  auto time = std::time(nullptr);
  osDate << std::put_time(std::localtime(&time), format.c_str());
  return osDate.str();
}
std::string brutils::getLocaltimeFormatted(std::string&& format)
{
  return getLocaltimeFormatted(format);
}
