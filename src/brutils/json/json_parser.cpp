/**
 * JSON parser implementations
 */

#include "json_parser.h"

namespace brutils::json
{

  namespace
  {
    void removeWhitespace(std::string_view& input);
    long intPower(long nth);
    double parseDecimal(std::string_view& input);
    long parseWholeNumber(std::string_view& input);
    std::any parseLiteral(std::string_view& input);
    std::any parseNumber(std::string_view& input);
    std::string parseString(std::string_view& input);
    std::any parseValue(std::string_view& input);
    std::vector<std::any> parseArray(std::string_view& input);
    std::map<std::string, std::any> parseObject(std::string_view& input);

    void removeWhitespace(std::string_view& input)
    {
      while (!input.empty() && std::isspace(input.at(0))) {
        input.remove_prefix(1);
      }
    }

    long intPower(long nth)
    {
      signed long val = 1;
      while (nth--) {
        val *= 10;
      }
      return val;
    }

    double parseDecimal(std::string_view& input)
    {
      double val = 0;
      for (int i = 0; std::isdigit(input.at(0)); ++i) {
        val += static_cast<double>(input.at(0) - '0') / static_cast<double>(intPower(i + 1));
        input.remove_prefix(1);
      }
      return val;
    }

    signed long parseWholeNumber(std::string_view& input)
    {
      int strLength = 0;
      for (; std::isdigit(input.at(strLength)); ++strLength);

      signed long val = 0;
      for (int i = 0; std::isdigit(input.at(0)); ++i) {
        val += (input.at(0) - '0') * intPower(strLength - i - 1);
        input.remove_prefix(1);
      }
      return val;
    }

    std::any parseLiteral(std::string_view& input)
    {
      removeWhitespace(input);

      if (!std::isalpha(input.at(0)))
        return {};

      std::any output;

      int size = 0;
      for (size = 0; std::isalpha(input.at(size)); ++size);

      std::string_view val(input.data(), size);
      if ("true" == val) {
        output = true;
      } else if ("false" == val) {
        output = false;
      } else if ("null" == val) {
        output = nullptr;
      }

      input.remove_prefix(size);

      removeWhitespace(input);
      return output;
    }

    std::any parseNumber(std::string_view& input)
    {
      removeWhitespace(input);

      int sign = 1;
      if ('-' == input.at(0)) {
        sign = -1;
        input.remove_prefix(1);
      }

      if (!std::isdigit(input.at(0)))
        return {};

      long number = parseWholeNumber(input); // removes digits

      if ('.' != input.at(0) && 'e' != input.at(0) && 'E' != input.at(0)) {
        // end of number, return the value
        removeWhitespace(input);
        return number * sign;
      }

      if ('.' == input.at(0)) {
        // here comes decimal part
        input.remove_prefix(1); // remove '.'
        auto decimal = parseDecimal(input);
        return static_cast<double>(number) + decimal;
      }

      if ('e' == input.at(0) || 'E' == input.at(0)) {
        // here comes exponential part
        input.remove_prefix(1); // remove 'e' || 'E'
        int expSign = 1;
        if ('-' == input.at(0)) {
          expSign = -1;
          input.remove_prefix(1); // remove '-'
        }

        long val = intPower(parseWholeNumber(input));
        if (1 == expSign)
          return static_cast<double>(number) * static_cast<double>(val);
        else
          return static_cast<double>(number) / static_cast<double>(val);
      }

      return {};
    }

    std::string parseString(std::string_view& input)
    {
      removeWhitespace(input);

      if ('\"' != input.at(0))
        return {};

      input.remove_prefix(1); // remove first '\"'

      unsigned long size = 0;
      unsigned long continueIndex = 0;
      while (true) {
        size = input.find_first_of('\"', continueIndex);
        if (std::string_view::npos != size) {
          if (size == 0)
            break;
          if ('\\' == input.at(size - 1)) {
            continueIndex = size;
            continue;
          } else break;
        } else {
          break;
        }
      }

      if (std::string_view::npos == size)
        return {};

      std::string output(input.data(), size);
      input.remove_prefix(size + 1);

      removeWhitespace(input);
      return output;
    }

    std::any parseValue(std::string_view& input)
    {
      /* can be string, number, object, array, true, false, null */

      removeWhitespace(input);

      std::any val;
      if ('\"' == input.at(0)) { // string
        val = parseString(input);
      } else if (std::isdigit(input.at(0)) || '-' == input.at(0)) { // number
        val = parseNumber(input);
      } else if ('{' == input.at(0)) { // object
        val = parseObject(input);
      } else if ('[' == input.at(0)) { // array
        val = parseArray(input);
      } else {
        val = parseLiteral(input);
      }

      removeWhitespace(input);
      return val;
    }

    std::vector<std::any> parseArray(std::string_view& input)
    {
      removeWhitespace(input);

      if ('[' != input.at(0))
        return {};

      input.remove_prefix(1); // remove first '['

      std::vector<std::any> array;
      while (']' != input.at(0)) {
        removeWhitespace(input);
        std::any element = parseValue(input);
        if (element.has_value())
          array.push_back(element);
        else return {};

        if (',' == input.at(0))
          input.remove_prefix(1); // remove ','

        removeWhitespace(input);
      }
      input.remove_prefix(1); // remove ']'

      removeWhitespace(input);
      return array;
    }

    std::map<std::string, std::any> parseObject(std::string_view& input)
    {
      removeWhitespace(input);

      if ('{' == input.at(0))
        input.remove_prefix(1); // remove '{'
      else return {};

      removeWhitespace(input);
      if ('}' == input.at(0)) {
        input.remove_prefix(1); // remove '}'
        return std::map<std::string, std::any>{}; // empty json
      }
      if ('\"' != input.at(0)) // if first char is not '\"', then wrong json
        return {};

      std::map<std::string, std::any> object;
      while ('}' != input.at(0)) {
        if ('\"' == input.at(0)) { // key-value pair
          auto key = parseString(input);
          removeWhitespace(input);
          if (':' != input.at(0))
            return {};
          input.remove_prefix(1); // remove ':'
          removeWhitespace(input);
          auto value = parseValue(input);
          object[key] = value;
        } else if (',' == input.at(0)) { // next
          input.remove_prefix(1); // remove ','
        } else {
          return {};
        }

        removeWhitespace(input);
      }
      input.remove_prefix(1); // remove '}'

      removeWhitespace(input);
      return object;
    }
  }

  std::any parse(std::string& input)
  {
    std::string_view sv = input;
    return parse(sv);
  }

  std::any parse(std::string_view input)
  {
    removeWhitespace(input);

    std::any content;
    if ('{' == input.at(0)) {
      content = parseObject(input);
    } else if ('[' == input.at(0)) {
      content = parseArray(input);
    } else {
      return nullptr;
    }

    removeWhitespace(input);

    if (!input.empty())
      return {};

    return content;
  }

}
