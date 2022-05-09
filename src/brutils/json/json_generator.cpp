/**
 * JSON generator implementations
 */

#include "json_generator.h"
#include <charconv>
#include <sstream>
#include <map>
#include <vector>

namespace brutils::json
{

  namespace
  {
    std::string genTab(int tabCount, int indent);
    std::string genLineFeed();
    std::string getWhiteSpace(bool compact, int indent, int depth);
    std::string genNullptr(nullptr_t ptr);
    std::string genBoolean(bool val);
    std::string genNumber(int number);
    std::string genNumber(unsigned int number);
    std::string genNumber(long number);
    std::string genNumber(long long number);
    std::string genNumber(unsigned long number);
    std::string genNumber(unsigned long long number);
    std::string genNumber(float number);
    std::string genNumber(double number);
    std::string genNumber(long double number);
    std::string genString(std::string const& string);
    std::string genValue(std::any const& value, bool compact, int indent, int depth);
    std::string genObject(std::map<std::string, std::any> const& object, bool compact, int indent, int depth);
    std::string genArray(std::vector<std::any> const& list, bool compact, int indent, int depth);

    std::string genTab(int tabCount, int indent)
    {
      std::string output;
      output.reserve(tabCount + 1);
      for (; tabCount > 0; --tabCount)
        for (; indent > 0; --indent)
          output.append(" ");
      return output;
    }

    std::string genLineFeed()
    {
      return "\r\n";
    }

    std::string getWhiteSpace(bool compact, int indent, int depth)
    {
      if (compact)
        return "";
      return genLineFeed() + genTab(depth, indent);
    }

    std::string genNullptr(nullptr_t ptr)
    {
      return "null";
    }

    std::string genBoolean(bool val)
    {
      return val ? "true" : "false";
    }

    std::string genNumber(int number)
    {
      return std::to_string(number);
    }

    std::string genNumber(unsigned int number)
    {
      return std::to_string(number);
    }

    std::string genNumber(long number)
    {
      return std::to_string(number);
    }

    std::string genNumber(long long number)
    {
      return std::to_string(number);
    }

    std::string genNumber(unsigned long number)
    {
      return std::to_string(number);
    }

    std::string genNumber(unsigned long long number)
    {
      return std::to_string(number);
    }

    std::string genNumber(float number)
    {
      return std::to_string(number);
    }

    std::string genNumber(double number)
    {
      return std::to_string(number);
    }

    std::string genNumber(long double number)
    {
      return std::to_string(number);
    }

    std::string genString(std::string const& string)
    {
      return "\"" + string + "\"";
    }

    std::string genValue(std::any const& value, bool compact, int indent, int depth)
    {
      if (!value.has_value()) {
        return {};
      }

      if (value.type() == typeid(nullptr))
        return genNullptr(std::any_cast<nullptr_t>(value));
      else if (value.type() == typeid(bool))
        return genBoolean(std::any_cast<bool>(value));
      else if (value.type() == typeid(int))
        return genNumber(std::any_cast<int>(value));
      else if (value.type() == typeid(unsigned int))
        return genNumber(std::any_cast<unsigned int>(value));
      else if (value.type() == typeid(long))
        return genNumber(std::any_cast<long>(value));
      else if (value.type() == typeid(unsigned long))
        return genNumber(std::any_cast<unsigned long>(value));
      else if (value.type() == typeid(long long))
        return genNumber(std::any_cast<long long>(value));
      else if (value.type() == typeid(unsigned long long))
        return genNumber(std::any_cast<unsigned long long>(value));
      else if (value.type() == typeid(float))
        return genNumber(std::any_cast<float>(value));
      else if (value.type() == typeid(double))
        return genNumber(std::any_cast<double>(value));
      else if (value.type() == typeid(long double))
        return genNumber(std::any_cast<long double>(value));
      else if (value.type() == typeid(std::string))
        return genString(std::any_cast<std::string>(value));
      else if (value.type() == typeid(std::map<std::string, std::any>))
        return genObject(std::any_cast<std::map<std::string, std::any>>(value), compact, indent, depth);
      else if (value.type() == typeid(std::vector<std::any>))
        return genArray(std::any_cast<std::vector<std::any>>(value), compact, indent, depth);
      else {
        return "";
      }
    }

    std::string genObject(std::map<std::string, std::any> const& object, bool compact, int indent, int depth)
    {
      std::string output = "{";
      for (auto& item : object) {
        output.append(getWhiteSpace(compact, indent, depth + 1));
        output.append(genString(item.first));
        output.append(": ");
        output.append(genValue(item.second, compact, indent, depth + 1));
        output.append(", ");
      }
      if (output.size() > 1) {
        output.erase(output.size() - 2);
        output.shrink_to_fit();
      }
      return output + getWhiteSpace(compact, indent, depth) + "}";
    }

    std::string genArray(std::vector<std::any> const& list, bool compact, int indent, int depth)
    {
      std::string output = "[";
      for (auto& element : list) {
        output.append(getWhiteSpace(compact, indent, depth + 1));
        output.append(genValue(element, compact, indent, depth + 1));
        output.append(", ");
      }
      if (output.size() > 1) {
        output.erase(output.size() - 2);
        output.shrink_to_fit();
      }
      return output + getWhiteSpace(compact, indent, depth) + "]";
    }
  }

  std::string generate(std::any const& json, bool compact, int indent)
  {
    return genValue(json, compact, indent, 0);
  }

}
