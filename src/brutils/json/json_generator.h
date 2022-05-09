/**
 * JSON generator functions
 */

#ifndef JSON_GENERATOR_H
#define JSON_GENERATOR_H

#include <any>
#include <string>

namespace brutils::json
{

  /**
   * @brief Generates a json string from the input any object
   *
   * It expects the input type to be one of the below:
   *    nullptr -> as json null value
   *    bool -> as json boolean value
   *    signed long -> as json int number value
   *    double -> as json double number value
   *    std::string -> as json string value
   *    std::vector<std::any> -> as json array containing the values above
   *    std::map<std::string, std::any> -> as json object containing the values above
   *
   * @param json - input data to be converted to json string
   * @param compact - whether to construct the json string as compact as possible or with indents
   * @param indent - number of empty spaces to be used for indents
   * @return string object containing json string
   */
  std::string generate(std::any const& json, bool compact = false, int indent = 2);

}

#endif //JSON_GENERATOR_H
