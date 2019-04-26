/**
 * @file variant_map.h
 *
 * Description
 */

#ifndef VARIANT_H
#define VARIANT_H

#include <string>
#include <map>
#include <any>

namespace brutils
{

class variant;

typedef std::map<std::string, variant> variant_map;

class variant
{
public:
    variant() = default;
    variant(bool val)
    {
        setValue(val);
    }
    variant(int val)
    {
        setValue(val);
    }
    variant(float val)
    {
        setValue(val);
    }
    variant(double val)
    {
        setValue(val);
    }
    variant(std::string val)
    {
        setValue(val);
    }
    variant(variant_map val)
    {
        setValue(val);
    }

    ~variant() = default;

    template<class T>
    void setValue(T t)
    {
        _type = const_cast<std::type_info *>(&typeid(t));
        _value = t;
    }

    std::type_info *getTypeInfo()
    {
        return _type;
    }

    bool toBool()
    {
        return value<bool>();
    }

    int toInt()
    {
        return value<int>();
    }

    float toFloat()
    {
        return value<float>();
    }

    double toDouble()
    {
        return value<double>();
    }

    std::string toString()
    {
        return value<std::string>();
    }

    variant_map toMap()
    {
        return value<variant_map>();
    }

    template<class T>
    T value()
    {
        if (typeid(T) == *_type)
            return std::any_cast<T>(_value);
        else return T();
    }

private:
    std::type_info *_type;
    std::any _value;
};

} // namespace brutils

#endif //VARIANT_H
