/**
 * @file json_parser.h
 *
 * Description
 */

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "variant.h"

namespace brutils
{

class json_parser
{
public:
    static variant parse(std::string &input);
    static variant parse(std::string_view input);

private:
    static variant_map parseObject(std::string_view &input);
    static variant_list parseArray(std::string_view &input);
    static variant parseValue(std::string_view &input);

    static variant parseString(std::string_view &input);
    static variant parseNumber(std::string_view &input);
    static variant parseLiteral(std::string_view &input);

    static float parseInt(std::string_view &input);
    static float parseAfterInt(std::string_view &input);

    static void removeWhitespace(std::string_view &input);
};

} // namespace brutils

#endif //JSON_PARSER_H
