/**
 * JSON parser functions
 */

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <any>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace brutils::json
{

  /**
   * @brief Parses the json the input string contains
   *
   * The returned std::any object will be invalid if the input cannot be parsed (has_value will return false)
   * The returned std::any object will be containing nullptr if the input string is empty
   *
   * The parsed json objects can be:
   *    std::map<std::string, std::any> -> containing json objects
   *    std::vector<std::any> -> containing json arrays
   *    std::string -> json string objects
   *    signed long -> json int values
   *    double -> json double values
   *    bool -> json boolean values
   *    nullptr -> json null values
   *
   * @param input - String object reference
   * @return std::any containing the parsed object
   */
  std::any parse(std::string const& input);

  /**
   * @brief Parses the json the input string_view contains
   *
   * The returned std::any object will be invalid if the input cannot be parsed (has_value will return false)
   * The returned std::any object will be containing nullptr if the input string is empty
   *
   * The parsed json objects can be:
   *    std::map<std::string, std::any> -> containing json objects
   *    std::vector<std::any> -> containing json arrays
   *    std::string -> json string objects
   *    std::uin64_t -> json int values
   *    double -> json double values
   *    bool -> json bool values
   *    nullptr -> json null values
   *
   * @param input
   * @return
   */
  std::any parse(std::string_view input);

} // namespace brutils

#endif //JSON_PARSER_H
