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
        : _valid(false), _type()
    {}
    explicit variant(std::nullptr_t aNullptr)
    {
        setValue(aNullptr);
    }
    explicit variant(bool val)
    {
        setValue(val);
    }
    explicit variant(int val)
    {
        setValue(val);
    }
    explicit variant(float val)
    {
        setValue(val);
    }
    explicit variant(double val)
    {
        setValue(val);
    }
    explicit variant(std::string val)
    {
        setValue(std::move(val));
    }
    explicit variant(variant_list val)
    {
        setValue(std::move(val));
    }
    explicit variant(variant_map val)
    {
        setValue(std::move(val));
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

    [[nodiscard]] bool isValid() const
    {
        return _valid;
    }

    [[nodiscard]] bool isNull() const
    {
        return !_value.has_value();
    }

    [[nodiscard]] bool isBool() const
    {
        if (!isNull())
            return _value.type() == typeid(bool);
        return false;
    }

    [[nodiscard]] bool isInt() const
    {
        if (!isNull())
            return _value.type() == typeid(int);
        return false;
    }

    [[nodiscard]] bool isFloat() const
    {
        if (!isNull())
            return _value.type() == typeid(float);
        return false;
    }

    [[nodiscard]] bool isDouble() const
    {
        if (!isNull())
            return _value.type() == typeid(double);
        return false;
    }

    [[nodiscard]] bool isString() const
    {
        if (!isNull())
            return _value.type() == typeid(std::string);
        return false;
    }

    [[nodiscard]] bool isMap() const
    {
        if (!isNull())
            return _value.type() == typeid(variant_map);
        return false;
    }

    [[nodiscard]] bool isList() const
    {
        if (!isNull())
            return _value.type() == typeid(variant_list);
        return false;
    }

    [[nodiscard]] bool isValue() const
    {
        if (!isNull())
            return _value.type() != typeid(variant_map) && _value.type() != typeid(variant_list);
        return false;
    }

    [[nodiscard]] bool toBool() const
    {
        return value<bool>();
    }

    [[nodiscard]] int toInt() const
    {
        return value<int>();
    }

    [[nodiscard]] float toFloat() const
    {
        return value<float>();
    }

    [[nodiscard]] double toDouble() const
    {
        return value<double>();
    }

    [[nodiscard]] std::string toString() const
    {
        return value<std::string>();
    }

    [[nodiscard]] variant_list toList() const
    {
        return value<variant_list>();
    }

    [[nodiscard]] variant_map toMap() const
    {
        return value<variant_map>();
    }

    template<class T>
    [[nodiscard]] T value() const
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
