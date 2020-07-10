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
/*!
 * @brief
 *    Returns current time as string in the given format
 * @param[in] format - lvalue reference to the string object containing format information
 * @return std::string
 */
std::string getTimeFormatted(std::string &format);
/*!
 * @brief
 *    Returns current time as string in the given format
 * @param[in] format - rvalue reference to the string object containing format information
 * @return std::string
 */
std::string getTimeFormatted(std::string &&format);
/*!
 * @brief
 *    Returns current local time as string in the given format
 * @param[in] format - lvalue reference to the string object containing format information
 * @return std::string
 */
std::string getLocaltimeFormatted(std::string &format);
/*!
 * @brief
 *    Returns current local time as string in the given format
 * @param[in] format - rvalue reference to the string object containing format information
 * @return std::string
 */
std::string getLocaltimeFormatted(std::string &&format);
}

#endif //BRUTILS_INCLUDE_BRUTILS_TIME_UTILS_H_
