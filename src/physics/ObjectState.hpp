/**
 * @file   ObjectState.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OBJECTSTATE_HPP_
#define _IWBAN_OBJECTSTATE_HPP_

#include <Global.hpp>

namespace phy
{

class ObjectState
{
private:
    Vector2     position;
    float       rotation;
    void *      userdata;

};
// class ObjectState

}
// namespace phy

#endif // _IWBAN_OBJECTSTATE_HPP_
