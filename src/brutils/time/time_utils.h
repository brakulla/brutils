/**
 * Time utility function declarations
 */

#ifndef BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_
#define BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_

#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>

namespace brutils::time
{

/*!
 * @brief
 *    Returns current time as string in the given format
 * @param[in] format - lvalue reference to the string object containing format information
 * @return std::string
 */
std::string getTimeFormatted(std::string const& format);

/*!
 * @brief
 *    Returns current time as string in the given format
 * @param[in] format - rvalue reference to the string object containing format information
 * @return std::string
 */
std::string getTimeFormatted(std::string&& format);

/*!
 * @brief
 *    Returns given time as string in the given format
 * @param[in] format - lvalue reference to the string object containing format information
 * @param[in] timePoint - time point object to convert
 * @return std::string
 */
std::string getTimeFormatted(std::string const& format, std::chrono::system_clock::time_point const& timePoint);

/*!
 * @brief
 *    Returns given time as string in the given format
 * @param[in] format - rvalue reference to the string object containing format information
 * @param[in] timePoint - time point object to convert
 * @return std::string
 */
std::string getTimeFormatted(std::string&& format, std::chrono::system_clock::time_point const& timePoint);

/*!
 * @brief
 *    Returns current local time as string in the given format
 * @param[in] format - lvalue reference to the string object containing format information
 * @return std::string
 */
std::string getLocaltimeFormatted(std::string const& format);

/*!
 * @brief
 *    Returns current local time as string in the given format
 * @param[in] format - rvalue reference to the string object containing format information
 * @return std::string
 */
std::string getLocaltimeFormatted(std::string&& format);

/*!
 * @brief
 *    Returns given time in local time zone as string in the given format
 * @param[in] format - lvalue reference to the string object containing format information
 * @param[in] timePoint - time point object to convert
 * @return std::string
 */
std::string getLocaltimeFormatted(std::string const& format, std::chrono::system_clock::time_point const& timePoint);

/*!
 * @brief
 *    Returns given time in local time zone as string in the given format
 * @param[in] format - rvalue reference to the string object containing format information
 * @param[in] timePoint - time point object to convert
 * @return std::string
 */
std::string getLocaltimeFormatted(std::string&& format, std::chrono::system_clock::time_point const& timePoint);

}

#endif // BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_
