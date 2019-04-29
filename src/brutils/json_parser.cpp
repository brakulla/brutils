/**
 * @file json_parser.cpp
 *
 * Description
 */

#include "json_parser.h"
#include "cmath"

using namespace brutils;

#include <iostream>

variant json_parser::parse(std::string &input)
{
    std::cout << "json_parser::parse" << std::endl;
    std::string_view sv = input;
    return parse(sv);
}

variant json_parser::parse(std::string_view input)
{
    std::cout << "json_parser::parse" << std::endl;
    removeWhitespace(input);

    variant content;
    if ('{' == input.at(0)) {
        content = parseObject(input);
    }
    else if ('[' == input.at(0)) {
        content = parseArray(input);
    }
    else {
        return variant();
    }
    std::cout << "json_parser::parse2" << std::endl;

    removeWhitespace(input);
    if (!input.empty())
        return variant();
    std::cout << "json_parser::parse3" << std::endl;

    return content;
}

variant_map json_parser::parseObject(std::string_view &input)
{
    std::cout << "json_parser::parseObject" << std::endl;
    removeWhitespace(input);

    if ('{' == input.at(0))
        input.remove_prefix(1); // remove '{'
    else return variant_map();

    removeWhitespace(input);
    if ('\"' != input.at(0)) // if first char is not '\"', then wrong json
        return variant_map();

    variant_map object;
    while ('}' != input.at(0)) {
        if ('\"' == input.at(0)) { // key-value pair
            auto key = parseString(input);
            std::cout << "key " << key.toString() << std::endl;
//            std::cout << "input.at(0)1 " << input.at(0) << std::endl;
            if (!key.isValid())
                return variant_map();
//            std::cout << "isv input.at(0) " << input.at(0) << std::endl;
            removeWhitespace(input);
            if (':' != input.at(0))
                return variant_map();
//            std::cout << "input.at(0)2 " << input.at(0) << std::endl;
            input.remove_prefix(1); // remove ':'
//            std::cout << "input.at(0)3 " << input.at(0) << std::endl;
            removeWhitespace(input);
            auto value = parseValue(input);
//            std::cout << "input.at(0)4 " << input.at(0) << std::endl;
//            std::cout << "value " << value.toString() << std::endl;
            object[key.toString()] = value;
        }
        else if (',' == input.at(0)) { // next
            input.remove_prefix(1); // remove ','
        }
        else {
            std::cout << "object not \" or , " << input.at(0) << std::endl;
            return variant_map();
        }

        removeWhitespace(input);
    }
    input.remove_prefix(1); // remove '}'

    removeWhitespace(input);
    std::cout << "exit json_parser::parseObject" << std::endl;
    return object;
}

variant_list json_parser::parseArray(std::string_view &input)
{
    std::cout << "json_parser::parseArray" << std::endl;
    removeWhitespace(input);

    if ('[' == input.at(0))
        input.remove_prefix(1); // remove first '['
    else return variant_list();

    variant_list array;
    while (']' != input.at(0)) {
        removeWhitespace(input);
        variant element = parseValue(input);
        if (element.isValid())
            array.push_back(element);
        else return variant_list();

        if (',' == input.at(0))
            input.remove_prefix(1); // remove ','

        removeWhitespace(input);
    }
    input.remove_prefix(1); // remove ']'

    removeWhitespace(input);
    std::cout << "exit json_parser::parseArray" << std::endl;
    return array;
}

variant json_parser::parseValue(std::string_view &input)
{
    std::cout << "json_parser::parseValue" << std::endl;
    /* can be string, number, object, array, true, false, null */

    removeWhitespace(input);

    variant val;

    if ('\"' == input.at(0)) { // string
        std::cout << "parseString " << std::endl;
        val = parseString(input);
        std::cout << "parseString " << val.toString() << std::endl;
    }
    else if (std::isdigit(input.at(0)) || '-' == input.at(0)) { // number
        std::cout << "parseNumber " << std::endl;
        val = parseNumber(input);
        std::cout << "parseNumber " << val.toFloat() << std::endl;
    }
    else if ('{' == input.at(0)) { // object
        std::cout << "parseObject " << std::endl;
        val = parseObject(input);
    }
    else if ('[' == input.at(0)) { // array
        std::cout << "parseArray " << std::endl;
        val = parseArray(input);
    }
    else {
        std::cout << "parseLiteral " << std::endl;
        val = parseLiteral(input);
    }

    removeWhitespace(input);
    std::cout << "exit json_parser::parseValue" << std::endl;
    return val;
}

variant json_parser::parseString(std::string_view &input)
{
    std::cout << "json_parser::parseString" << std::endl;
    removeWhitespace(input);

    if ('\"' == input.at(0))
        input.remove_prefix(1); // remove first '\"'
    else return variant();

    int size = 0;
    int continueIndex = 0;
    while (true) {
        size = input.find_first_of('\"', continueIndex);
        if (std::string_view::npos != size) {
            if (size == 0)
                break;
            if ('\\' == input.at(size - 1)) {
                continueIndex = size;
                continue;
            }
            else break;
        }
        else {
            break;
        }
    }

    if (std::string_view::npos == size)
        return variant();

    std::string output(input.data(), size);
    input.remove_prefix(size + 1);

    removeWhitespace(input);

    std::cout << output << std::endl;

    std::cout << "exit json_parser::parseString" << std::endl;
    return variant(output);
}

variant json_parser::parseNumber(std::string_view &input)
{
    std::cout << "json_parser::parseNumber" << std::endl;
    removeWhitespace(input);

    int coeff = 1;
    if ('-' == input.at(0)) {
        coeff = -1;
        input.remove_prefix(1);
    }

    float number = 0;
    if ('0' == input.at(0)) {
        input.remove_prefix(1); // remove '0'
    }
    else if (std::isdigit(input.at(0))) {
        number = parseInt(input); // removes digits
    }
    else {
        return variant();
    }

    if ('.' != input.at(0) && 'e' != input.at(0) && 'E' != input.at(0)) {
        removeWhitespace(input);
        return number;
    }

    if ('.' == input.at(0)) {
        input.remove_prefix(1); // remove '.'
        number += parseAfterInt(input);
    }

    if ('e' == input.at(0) || 'E' == input.at(0)) {
        input.remove_prefix(1); // remove 'e' || 'E'
        int coef = 1;
        if ('-' == input.at(0)) {
            coef = -1;
            input.remove_prefix(1); // remove '-'
        }

        float val = std::pow(10, parseInt(input));
        if (1 == coef)
            number *= val;
        else
            number /= val;
    }

    number *= coeff;

    std::cout << number << std::endl;

    std::cout << "exit json_parser::parseNumber" << std::endl;
    return variant(number);
}

variant json_parser::parseLiteral(std::string_view &input)
{
    std::cout << "json_parser::parseLiteral" << std::endl;
    removeWhitespace(input);

    if (!std::isalpha(input.at(0)))
        return variant();

    variant output;

    int size = 0;
    for (size = 0; std::isalpha(input.at(size)); ++size);

    std::string_view val(input.data(), size);
    if ("true" == val) {
        output = variant(true);
        std::cout << true << std::endl;
    }
    else if ("false" == val) {
        output = variant(false);
        std::cout << false << std::endl;
    }
    else if ("null" == val) {
        output = variant(nullptr);
        std::cout << "null" << std::endl;
    }

    input.remove_prefix(size);

    removeWhitespace(input);
    std::cout << "exit json_parser::parseLiteral" << std::endl;
    return output;
}

float json_parser::parseInt(std::string_view &input)
{
    std::cout << "json_parser::parseInt" << std::endl;
    int strLength = 0;
    for (; isdigit(input.at(strLength)); ++strLength);

    float val = 0;
    for (int i = 0; isdigit(input.at(0)); ++i) {
        val += (input.at(0) - '0') * pow(10, strLength - i - 1);
        input.remove_prefix(1);
    }
    std::cout << "int: " << val << std::endl;
    return val;
}

float json_parser::parseAfterInt(std::string_view &input)
{
    std::cout << "json_parser::parseAfterInt" << std::endl;
    float val = 0;
    for (int i = 0; isdigit(input.at(0)); ++i) {
        val += (input.at(0) - '0') / pow(10, i + 1);
        input.remove_prefix(1);
    }

    std::cout << "afterint: " << val << std::endl;
    return val;
}

void json_parser::removeWhitespace(std::string_view &input)
{
//    std::cout << "json_parser::removeWhitespace" << std::endl;
    while (!input.empty() && std::isspace(input.at(0))) {
        input.remove_prefix(1);
    }
}