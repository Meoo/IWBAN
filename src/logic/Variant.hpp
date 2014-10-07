/**
 * @file   Variant.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_VARIANT_HPP_
#define _IWBAN_VARIANT_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>

#include <string>

namespace logic
{

class Lua;

class Variant
{
public:
    typedef int         Int;
    typedef float       Float;
    typedef ut::Vector  Vector;
    typedef std::string String;


private:
    // Type
    enum Type
    {
        TYPE_NULL,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_BOOL,
        TYPE_VECTOR,
        TYPE_STRING,

    }           _type;
    // enum Type

    // Primitive types and POD
    union
    {
        Int     _int_value;
        Float   _float_value;
        bool    _bool_value;
        Vector  _vector_value;
    };

    // Complex classes
    String      _string_value;


public:
    // Constructors
                Variant();
                Variant(Int int_value);
                Variant(Float float_value);
                Variant(bool bool_value);
                Variant(const Vector & vector_value);
                Variant(const String & string_value);
                Variant(const char * string_value);

    // Type check functions
    bool        isNull() const      { return _type == TYPE_NULL; }
    bool        isInt() const       { return _type == TYPE_INT; }
    bool        isFloat() const     { return _type == TYPE_FLOAT; }
    bool        isBool() const      { return _type == TYPE_BOOL; }
    bool        isVector() const    { return _type == TYPE_VECTOR; }
    bool        isString() const    { return _type == TYPE_STRING; }

    // Conversion functions
    Int         toInt() const;
    Float       toFloat() const;
    bool        toBool() const;
    Vector      toVector() const;
    String      toString() const;

};
// class Entity

}
// namespace logic

#endif // _IWBAN_VARIANT_HPP_
