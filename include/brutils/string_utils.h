/**
 * @file string_utils.h
 *
 * Description
 */

#ifndef BRUTILS_STRING_UTILS_H
#define BRUTILS_STRING_UTILS_H

#include <string>
#include <sstream>
#include <algorithm>

namespace brutils {
template<class Container>
static void split_string(const std::string &str, Container &cont, char delim = ' ')
{
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delim))
    cont.push_back(token);
}
template<class Container>
static void split_string(const std::string &str, Container &cont, const std::string delim = " ")
{
  std::string token;
  for (unsigned long i = 0, index = 0; i < str.size(); i = index + delim.size()) {
    index = str.find(delim, i);
    if (std::string::npos == index) {
      cont.push_back(str.substr(i, str.size() - 1));
      break;
    }
    token = str.substr(i, index - i);
    cont.push_back(token);
  }
}
static bool str_startsWith(const std::string baseStr, const std::string subStr)
{
  if (subStr.size() >= baseStr.size())
    return 0 == baseStr.find(subStr);
  return false;
}
static int str_startsWithMatch(const std::string baseStr, const std::string subStr)
{
  if (subStr.size() >= baseStr.size()) {
    if (0 == baseStr.find(subStr))
      return (int) baseStr.size();
  }
  return 0;
}
static bool str_contains(const std::string baseStr, const std::string subStr)
{
  return std::string::npos != baseStr.find(subStr);
}
template<class CharT>
static std::basic_string<CharT> toLower(const std::basic_string<CharT> &input)
{
  std::basic_string<CharT> lower;
  lower.reserve(input.size());
  std::transform(input.begin(), input.end(), lower.begin(), [](CharT c) {
    return std::tolower(c);
  });
  return lower;
}
template<class CharT>
static std::basic_string<CharT> toUpper(const std::basic_string<CharT> &input)
{
  std::basic_string<CharT> upper;
  upper.reserve(input.size());
  std::transform(input.begin(), input.end(), upper.begin(), [](CharT c) {
    return std::toupper(c);
  });
  return upper;
}
static std::string str_threadIdToStr(const std::thread::id id)
{
  std::stringstream ss;
  ss << id;
  return ss.str();
}
}

#endif //BRUTILS_STRING_UTILS_H
