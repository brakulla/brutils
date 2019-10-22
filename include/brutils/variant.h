/**
 * @file variant_map.h
 *
 * Description
 */

#ifndef VARIANT_H
#define VARIANT_H

#include <string>
#include <map>
#include <vector>
#include <any>

namespace brutils
{

class variant;

typedef std::map<std::string, variant> variant_map;

typedef std::vector<variant> variant_list;

class variant
{
public:
    variant()
        : _valid(false)
    {}
    variant(std::nullptr_t aNullptr)
    {
        _valid = true;
    }
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
    variant(variant_list val)
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
        _valid = true;
    }

    std::type_info *getTypeInfo()
    {
        return _type;
    }

    bool isValid()
    {
        return _valid;
    }

    bool isNull()
    {
        return !_value.has_value();
    }

    bool isBool()
    {
        if (!isNull())
            return _value.type() == typeid(bool);
        return false;
    }

    bool isInt()
    {
        if (!isNull())
            return _value.type() == typeid(int);
        return false;
    }

    bool isFloat()
    {
        if (!isNull())
            return _value.type() == typeid(float);
        return false;
    }

    bool isDouble()
    {
        if (!isNull())
            return _value.type() == typeid(double);
        return false;
    }

    bool isString()
    {
        if (!isNull())
            return _value.type() == typeid(std::string);
        return false;
    }

    bool isMap()
    {
        if (!isNull())
            return _value.type() == typeid(variant_map);
        return false;
    }

    bool isList()
    {
        if (!isNull())
            return _value.type() == typeid(variant_list);
        return false;
    }

    bool isValue()
    {
        if (!isNull())
            return _value.type() != typeid(variant_map) && _value.type() != typeid(variant_list);
        return false;
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

    variant_list toList()
    {
        return value<variant_list>();
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
    bool _valid;
    std::type_info *_type;
    std::any _value;
};

} // namespace brutils

#endif //VARIANT_H
