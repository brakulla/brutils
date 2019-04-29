/**
 * @file json_generator.h
 *
 * Description
 */

#ifndef JSON_GENERATOR_H
#define JSON_GENERATOR_H

#include <string>
#include "variant.h"

namespace brutils
{

class json_generator
{
public:
    json_generator(bool compact = false);
    ~json_generator() = default;

    std::string generate(variant &json);

private:
    std::string genObject(variant_map object, int depth = 0);
    std::string genArray(variant_list list, int depth = 0);
    std::string genValue(variant value, int depth = 0);

    std::string genString(variant string);
    std::string genNumber(variant number);
    std::string genLiteral(variant literal);

    std::string getWhiteSpace(int depth);
    std::string genLineFeed();
    std::string genTab(int tabCount = 0);

private:
    bool _compact;
};

}

#endif //JSON_GENERATOR_H
