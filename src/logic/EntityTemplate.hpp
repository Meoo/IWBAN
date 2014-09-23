/**
 * @file   EntityTemplate.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITYTEMPLATE_HPP_
#define _IWBAN_ENTITYTEMPLATE_HPP_

#include <Global.hpp>

namespace logic
{

/**
 * @brief Class allowing to build entities with predefined parameters.
 */
class EntityTemplate
{
private:
    std::string _entity_class;


public:
    explicit EntityTemplate(const std::string & entity_class);

    std::unique_ptr<Entity> && createEntity(EntityFactory & factory) const;

};
// class EntityTemplate

}
// namespace logic

#endif // _IWBAN_ENTITYTEMPLATE_HPP_
