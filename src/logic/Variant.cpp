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

    default: break;
    }

    return Vector();
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

        // TODO Vector ?

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

namespace ut
{

template<>
logic::Variant read<logic::Variant>(std::istream & stream)
{
    typedef logic::Variant  Variant;
    typedef Variant::Type   Type;

    Type t = (Type) ut::read<std::uint8_t>(stream);

    switch (t)
    {
    case Type::TYPE_INT:
        return Variant(ut::read<Variant::Int>(stream));

    case Type::TYPE_FLOAT:
        return Variant(ut::read<Variant::Float>(stream));

    case Type::TYPE_BOOL:
        return Variant(ut::read<bool>(stream));

    case Type::TYPE_VECTOR:
        return Variant(ut::read<ut::Vector>(stream));

    case Type::TYPE_STRING:
        return Variant(ut::read<Variant::String>(stream));

    default: break;
    }

    return Variant();
}

template<>
void write<logic::Variant>(std::ostream & stream, const logic::Variant & value)
{
    typedef logic::Variant  Variant;
    typedef Variant::Type   Type;

    ut::write<std::uint8_t>(stream, (std::uint8_t) value.getType());

    switch (value.getType())
    {
    case Type::TYPE_INT:
        ut::write<Variant::Int>(stream, value.toInt());
        break;

    case Type::TYPE_FLOAT:
        ut::write<Variant::Float>(stream, value.toFloat());
        break;

    case Type::TYPE_BOOL:
        ut::write<bool>(stream, value.toBool());
        break;

    case Type::TYPE_VECTOR:
        ut::write<ut::Vector>(stream, value.toVector());
        break;

    case Type::TYPE_STRING:
        ut::write<Variant::String>(stream, value.toString());
        break;

    default: break;
    }
}

}
// namespace ut
