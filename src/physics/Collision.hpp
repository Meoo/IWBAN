/**
 * @file   Collision.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_COLLISION_HPP_
#define _IWBAN_COLLISION_HPP_

#include <Global.hpp>

#include <physics/Contact.hpp>

#include <vector>

namespace phy
{

class Body;

void computeContacts(const Body & first, const Body & second,
                     std::vector<Contact> & output);

}
// namespace phy

#endif // _IWBAN_COLLISION_HPP_
