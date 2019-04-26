/**
 * @file string_utils.h
 *
 * Description
 */

#ifndef BRUTILS_STRING_UTILS_H
#define BRUTILS_STRING_UTILS_H

#include <string>
#include <sstream>

namespace brutils {
  template <class Container>
  static void split_string(const std::string &str, Container &cont, char delim = ' ') {
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
      cont.push_back(token);
  }
  template <class Container>
  static void split_string(const std::string &str, Container &cont, const std::string delim = " ")
  {
    std::string token;
    for(unsigned long i = 0, index = 0; i < str.size(); i = index+delim.size()) {
      index = str.find(delim, i);
      if (std::string::npos == index) {
        cont.push_back(str.substr(i, str.size()-1));
        break;
      }
      token = str.substr(i, index-i);
      cont.push_back(token);
    }
  }
  static bool str_startsWith(const std::string baseStr, const std::string subStr) {
      if (subStr.size() >= baseStr.size())
          return 0 == baseStr.find(subStr);
      return false;
  }
  static int str_startsWithMatch(const std::string baseStr, const std::string subStr) {
      if (subStr.size() >= baseStr.size()) {
          if (0 == baseStr.find(subStr))
              return (int) baseStr.size();
      }
      return 0;
  }
  static bool str_contains(const std::string baseStr, const std::string subStr) {
    return std::string::npos != baseStr.find(subStr);
  }
}

#endif //BRUTILS_STRING_UTILS_H
