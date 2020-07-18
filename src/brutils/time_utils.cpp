//
// Created by Burak Ongay on 05/07/2020.
//

#include "brutils/time_utils.h"

std::string brutils::getTimeFormatted(const std::string& format)
{
  return getTimeFormatted(format, std::chrono::system_clock::now());
}
std::string brutils::getTimeFormatted(std::string&& format)
{
  return getTimeFormatted(format, std::chrono::system_clock::now());
}
std::string brutils::getTimeFormatted(const std::string& format, const std::chrono::system_clock::time_point& timePoint)
{
  std::ostringstream osDate;
  auto time = std::chrono::system_clock::to_time_t(timePoint);
  osDate << std::put_time(std::gmtime(&time), format.c_str());
  return osDate.str();
}
std::string brutils::getTimeFormatted(std::string&& format, const std::chrono::system_clock::time_point& timePoint)
{
  return getTimeFormatted(format, timePoint);
}
std::string brutils::getLocaltimeFormatted(const std::string& format)
{
  return getLocaltimeFormatted(format, std::chrono::system_clock::now());
}
std::string brutils::getLocaltimeFormatted(std::string&& format)
{
  return getLocaltimeFormatted(format, std::chrono::system_clock::now());
}
std::string brutils::getLocaltimeFormatted(const std::string& format, const std::chrono::system_clock::time_point& timePoint)
{
  std::ostringstream osDate;
  auto time = std::chrono::system_clock::to_time_t(timePoint);
  osDate << std::put_time(std::localtime(&time), format.c_str());
  return osDate.str();
}
std::string brutils::getLocaltimeFormatted(std::string&& format, const std::chrono::system_clock::time_point& timePoint)
{
  return getLocaltimeFormatted(format, timePoint);
}
