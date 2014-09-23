/**
 * @file   NameFilter.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_NAMEFILTER_HPP_
#define _IWBAN_NAMEFILTER_HPP_

#include <Global.hpp>

namespace logic
{

class NameFilter final : public EntityFilter
{
public:
    NameFilter(const std::string & name);

    bool match(const Entity & entity) const override;

};
// class NameFilter

}
// namespace logic

#endif // _IWBAN_NAMEFILTER_HPP_
