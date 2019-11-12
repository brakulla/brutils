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
    static std::string generate(const variant &json, bool compact = false);

private:
    static std::string genObject(const variant_map &object, bool compact, int depth = 0);
    static std::string genArray(const variant_list &list, bool compact, int depth = 0);
    static std::string genValue(const variant &value, bool compact, int depth = 0);

    static std::string genString(const variant &string);
    static std::string genNumber(const variant &number);
    static std::string genLiteral(const variant &literal);

    static std::string getWhiteSpace(bool compact, int depth);
    static std::string genLineFeed();
    static std::string genTab(int tabCount = 0);

private:
    bool _compact;
};

}

#endif //JSON_GENERATOR_H
