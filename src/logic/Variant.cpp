/**
 * @file   Variant.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <logic/Lua.hpp>
#include <logic/Variant.hpp>

#include <boost/lexical_cast.hpp>

namespace logic
{

Variant::Variant()
    : _type(TYPE_NULL)
{
}

Variant::Variant(Int int_value)
    : _type(TYPE_INT), _int_value(int_value)
{
}

Variant::Variant(Float float_value)
    : _type(TYPE_FLOAT), _float_value(float_value)
{
}

Variant::Variant(bool bool_value)
    : _type(TYPE_BOOL), _bool_value(bool_value)
{
}

Variant::Variant(const Vector & vector_value)
    : _type(TYPE_VECTOR), _vector_value(vector_value)
{
}

Variant::Variant(const String & string_value)
    : _type(TYPE_STRING), _string_value(string_value)
{
}

Variant::Variant(const char * string_value)
    : _type(TYPE_STRING), _string_value(string_value)
{
}

// ---- ---- ---- ----

Variant::Int Variant::toInt() const
{
    try
    {
        switch (_type)
        {
        case TYPE_INT:
            return _int_value;

        case TYPE_FLOAT:
            return static_cast<Int>(_float_value);

        case TYPE_BOOL:
            return static_cast<Int>(_bool_value);

        case TYPE_STRING:
            return boost::lexical_cast<Int>(_string_value);

        default: break;
        }
    }
    catch(const boost::bad_lexical_cast &) {}

    return 0;
}

Variant::Float Variant::toFloat() const
{
    try
    {
        switch (_type)
        {
        case TYPE_INT:
            return static_cast<Float>(_int_value);

        case TYPE_FLOAT:
            return _float_value;

        case TYPE_BOOL:
            return static_cast<Float>(_bool_value);

        case TYPE_STRING:
            return boost::lexical_cast<Float>(_string_value);

        default: break;
        }
    }
    catch(const boost::bad_lexical_cast &) {}

    return 0;
}

bool Variant::toBool() const
{
    try
    {
        switch (_type)
        {
        case TYPE_INT:
            return static_cast<bool>(_int_value);

        case TYPE_FLOAT:
            return static_cast<bool>(_float_value);

        case TYPE_BOOL:
            return _bool_value;

        case TYPE_STRING:
            return boost::lexical_cast<bool>(_string_value);

        default: break;
        }
    }
    catch(const boost::bad_lexical_cast &) {}

    return false;
}

Variant::Vector Variant::toVector() const
{
    switch (_type)
    {
    case TYPE_VECTOR:
        return _vector_value;

    default:
        return Vector();
    }
}

Variant::String Variant::toString() const
{
    try
    {
        switch (_type)
        {
        case TYPE_INT:
            return boost::lexical_cast<String>(_int_value);

        case TYPE_FLOAT:
            return boost::lexical_cast<String>(_float_value);

        case TYPE_BOOL:
            return boost::lexical_cast<String>(_bool_value);

        case TYPE_STRING:
            return _string_value;

        default: break;
        }
    }
    catch(const boost::bad_lexical_cast &) {}

    return String();
}

}
// namespace logic
