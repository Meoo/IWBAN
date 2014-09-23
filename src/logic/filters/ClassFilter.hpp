/**
 * @file   ClassFilter.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CLASSFILTER_HPP_
#define _IWBAN_CLASSFILTER_HPP_

#include <Global.hpp>

namespace logic
{

class ClassFilter final : public EntityFilter
{
public:
    ClassFilter(const std::string & entity_class);

    bool match(const Entity & entity) const override;

};
// class ClassFilter

}
// namespace logic

#endif // _IWBAN_CLASSFILTER_HPP_
