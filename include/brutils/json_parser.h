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
    json_parser() = default;
    ~json_parser() = default;

    variant parse(std::string &input);
    variant parse(std::string_view input);

private:
    variant_map parseObject(std::string_view &input);
    variant_list parseArray(std::string_view &input);
    variant parseValue(std::string_view &input);

    variant parseString(std::string_view &input);
    variant parseNumber(std::string_view &input);
    variant parseLiteral(std::string_view &input);

    float parseInt(std::string_view &input);
    float parseAfterInt(std::string_view &input);

    void removeWhitespace(std::string_view &input);
};

} // namespace brutils

#endif //JSON_PARSER_H
