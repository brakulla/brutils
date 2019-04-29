/**
 * @file json_generator.cpp
 *
 * Description
 */

#include "brutils/json_generator.h"
#include <charconv>
#include <sstream>

using namespace brutils;

json_generator::json_generator(bool compact)
    : _compact(compact)
{}

std::string json_generator::generate(variant &json)
{
    return genValue(json);
}

std::string json_generator::genObject(brutils::variant_map object, int depth)
{
    std::string output = "{";
    for (auto &item: object) {
        output.append(getWhiteSpace(depth + 1));
        output.append(genString(item.first));
        output.append(": ");
        output.append(genValue(item.second, depth + 1));
        output.append(", ");
    }
    if (output.size() > 1) {
        output.erase(output.size() - 2);
        output.shrink_to_fit();
    }
    return output + getWhiteSpace(depth) + "}";
}

std::string json_generator::genArray(brutils::variant_list list, int depth)
{
    std::string output = "[";
    for (auto &element: list) {
        output.append(getWhiteSpace(depth + 1));
        output.append(genValue(element, depth + 1));
        output.append(", ");
    }
    if (output.size() > 1) {
        output.erase(output.size() - 2);
        output.shrink_to_fit();
    }
    return output + getWhiteSpace(depth) + "]";
}

std::string json_generator::genValue(brutils::variant value, int depth)
{
    if (value.isValid() && value.isNull())
        return genLiteral(value);
    else if (value.isBool())
        return genLiteral(value);
    else if (value.isInt() || value.isFloat() || value.isDouble())
        return genNumber(value);
    else if (value.isString())
        return genString(value);
    else if (value.isMap())
        return genObject(value.toMap(), depth);
    else if (value.isList())
        return genArray(value.toList(), depth);
    else {
        printf("json_gen :: empty value\n");
        return "";
    }
}

std::string json_generator::genString(brutils::variant string)
{
    if (!string.isString())
        return "";
    return "\"" + string.toString() + "\"";
}

std::string json_generator::genNumber(brutils::variant number)
{
    std::ostringstream ss;
    if (number.isInt()) {
        ss << number.toInt();
    }
    else if (number.isFloat()) {
        ss << number.toFloat();
    }
    else if (number.isDouble()) {
        ss << number.toDouble();
    }
    return ss.str();
}

std::string json_generator::genLiteral(brutils::variant literal)
{
    if (!literal.isValid())
        return "";
    if (literal.isNull())
        return "null";
    if (literal.isBool()) {
        if (literal.toBool())
            return "true";
        else return "false";
    }
    return "";
}

std::string json_generator::getWhiteSpace(int depth)
{
    if (_compact)
        return "";
    return genLineFeed() + genTab(depth);
}

std::string json_generator::genLineFeed()
{
    return "\r\n";
}

std::string json_generator::genTab(int tabCount)
{
    std::string output;
    output.reserve(tabCount + 1);
    for (; tabCount > 0; --tabCount)
        output.append("    ");
    return output;
}
