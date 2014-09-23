/**
 * @file   Contact.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONTACT_HPP_
#define _IWBAN_CONTACT_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>

namespace phy
{

class Body;

class Contact
{
public:
    typedef ut::Vector  Vector;

    // Data members
    Body *  first;
    Body *  second;

    Vector  origin;
    Vector  normal;

    Vector  impulse;

    // Static function
    static
    Contact reverse(const Contact & contact)
    {
        Contact ret;

        ret.first   = contact.second;
        ret.second  = contact.first;

        ret.origin  = contact.origin;
        ret.normal  = - contact.normal;

        ret.impulse = - contact.impulse;

        return ret;
    }

};
// class Contact

}
// namespace phy

#endif // _IWBAN_CONTACT_HPP_
