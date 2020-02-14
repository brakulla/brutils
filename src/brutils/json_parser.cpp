/**
 * @file json_parser.cpp
 *
 * Description
 */

#include "brutils/json_parser.h"
#include "cmath"

using namespace brutils;

variant json_parser::parse(std::string &input)
{
    std::string_view sv = input;
    return parse(sv);
}

variant json_parser::parse(std::string_view input)
{
    removeWhitespace(input);

    variant content;
    if ('{' == input.at(0)) {
        content = variant(parseObject(input));
    }
    else if ('[' == input.at(0)) {
        content = variant(parseArray(input));
    }
    else {
        return variant();
    }

    removeWhitespace(input);

    if (!input.empty())
        return variant();

    return content;
}

variant_map json_parser::parseObject(std::string_view &input)
{
    removeWhitespace(input);

    if ('{' == input.at(0))
        input.remove_prefix(1); // remove '{'
    else return variant_map();

    removeWhitespace(input);
    if ('}' == input.at(0)) {
        input.remove_prefix(1); // remove '}'
        return variant_map(); // empty json
    }
    if ('\"' != input.at(0)) // if first char is not '\"', then wrong json
        return variant_map();

    variant_map object;
    while ('}' != input.at(0)) {
        if ('\"' == input.at(0)) { // key-value pair
            auto key = parseString(input);
            if (!key.isValid())
                return variant_map();
            removeWhitespace(input);
            if (':' != input.at(0))
                return variant_map();
            input.remove_prefix(1); // remove ':'
            removeWhitespace(input);
            auto value = parseValue(input);
            object[key.toString()] = value;
        }
        else if (',' == input.at(0)) { // next
            input.remove_prefix(1); // remove ','
        }
        else {
            return variant_map();
        }

        removeWhitespace(input);
    }
    input.remove_prefix(1); // remove '}'

    removeWhitespace(input);
    return object;
}

variant_list json_parser::parseArray(std::string_view &input)
{
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
    return array;
}

variant json_parser::parseValue(std::string_view &input)
{
    /* can be string, number, object, array, true, false, null */

    removeWhitespace(input);

    variant val;

    if ('\"' == input.at(0)) { // string
        val = parseString(input);
    }
    else if (std::isdigit(input.at(0)) || '-' == input.at(0)) { // number
        val = parseNumber(input);
    }
    else if ('{' == input.at(0)) { // object
        val = variant(parseObject(input));
    }
    else if ('[' == input.at(0)) { // array
        val = variant(parseArray(input));
    }
    else {
        val = parseLiteral(input);
    }

    removeWhitespace(input);
    return val;
}

variant json_parser::parseString(std::string_view &input)
{
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
    return variant(output);
}

variant json_parser::parseNumber(std::string_view &input)
{
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
        return variant(number);
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
    return variant(number);
}

variant json_parser::parseLiteral(std::string_view &input)
{
    removeWhitespace(input);

    if (!std::isalpha(input.at(0)))
        return variant();

    variant output;

    int size = 0;
    for (size = 0; std::isalpha(input.at(size)); ++size);

    std::string_view val(input.data(), size);
    if ("true" == val) {
        output = variant(true);
    }
    else if ("false" == val) {
        output = variant(false);
    }
    else if ("null" == val) {
        output = variant(nullptr);
    }

    input.remove_prefix(size);

    removeWhitespace(input);
    return output;
}

float json_parser::parseInt(std::string_view &input)
{
    int strLength = 0;
    for (; isdigit(input.at(strLength)); ++strLength);

    float val = 0;
    for (int i = 0; isdigit(input.at(0)); ++i) {
        val += (input.at(0) - '0') * pow(10, strLength - i - 1);
        input.remove_prefix(1);
    }
    return val;
}

float json_parser::parseAfterInt(std::string_view &input)
{
    float val = 0;
    for (int i = 0; isdigit(input.at(0)); ++i) {
        val += (input.at(0) - '0') / pow(10, i + 1);
        input.remove_prefix(1);
    }
    return val;
}

void json_parser::removeWhitespace(std::string_view &input)
{
    while (!input.empty() && std::isspace(input.at(0))) {
        input.remove_prefix(1);
    }
}