/**
 * @file   EntityFilter.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITYFILTER_HPP_
#define _IWBAN_ENTITYFILTER_HPP_

#include <Global.hpp>

namespace logic
{

class EntityFilter
{
public:
    virtual ~EntityFilter() {}

    virtual bool match(const Entity & entity) const = 0;

};
// class EntityFilter

}
// namespace logic

#endif // _IWBAN_ENTITYFILTER_HPP_
