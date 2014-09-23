/**
 * @file   Controller.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONTROLLER_HPP_
#define _IWBAN_CONTROLLER_HPP_

#include <Global.hpp>

namespace phy
{

class Contact;

class Controller
{
public:
    // Virtual destructor
    virtual ~Controller() {}

    // Virtual functions
    virtual void preUpdate(const Space & space, Body & body)    = 0;

    virtual void step(const Space & space, Body & body)         = 0;

    virtual void resolveContact(const Space & space, Body & body,
                                const Contact & contact)        = 0;

    virtual void postUpdate(const Space & space, Body & body)   = 0;

};
// class Controller

}
// namespace phy

#endif // _IWBAN_CONTROLLER_HPP_
