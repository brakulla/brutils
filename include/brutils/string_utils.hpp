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
class string_utils {
 public:
  template<class Container>
  static void split_string(const std::string &str, Container &cont, char delim = ' ') {
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
      cont.push_back(token);
  }
};
}

#endif //BRUTILS_STRING_UTILS_H
