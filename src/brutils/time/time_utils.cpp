//
// Created by Burak Ongay on 05/07/2020.
//

#include "time_utils.h"

namespace brutils::time
{

std::string getTimeFormatted(std::string const& format)
{
  return getTimeFormatted(format, std::chrono::system_clock::now());
}

std::string getTimeFormatted(std::string&& format)
{
  return getTimeFormatted(format, std::chrono::system_clock::now());
}

std::string getTimeFormatted(std::string const& format, std::chrono::system_clock::time_point const& timePoint)
{
  std::ostringstream osDate;
  auto time = std::chrono::system_clock::to_time_t(timePoint);
  osDate << std::put_time(std::gmtime(&time), format.c_str());
  return osDate.str();
}

std::string getTimeFormatted(std::string&& format, std::chrono::system_clock::time_point const& timePoint)
{
  return getTimeFormatted(format, timePoint);
}

std::string getLocaltimeFormatted(std::string const& format)
{
  return getLocaltimeFormatted(format, std::chrono::system_clock::now());
}

std::string getLocaltimeFormatted(std::string&& format)
{
  return getLocaltimeFormatted(format, std::chrono::system_clock::now());
}

std::string getLocaltimeFormatted(std::string const& format, std::chrono::system_clock::time_point const& timePoint)
{
  std::ostringstream osDate;
  auto time = std::chrono::system_clock::to_time_t(timePoint);
  osDate << std::put_time(std::localtime(&time), format.c_str());
  return osDate.str();
}

std::string getLocaltimeFormatted(std::string&& format, std::chrono::system_clock::time_point const& timePoint)
{
  return getLocaltimeFormatted(format, timePoint);
}

}