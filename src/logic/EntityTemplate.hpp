/**
 * @file   EntityTemplate.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITYTEMPLATE_HPP_
#define _IWBAN_ENTITYTEMPLATE_HPP_

#include <Global.hpp>

#include <logic/Variant.hpp>

#include <map>
#include <memory>
#include <string>

namespace logic
{

class Entity;
class EntityFactory;

/**
 * @brief Class allowing to build entities with predefined parameters.
 */
class EntityTemplate
{
private:
    std::string                     _entity_class;
    std::map<std::string, Variant>  _properties;


public:
    explicit EntityTemplate(const std::string & entity_class);

    void setValue(const std::string & key, const Variant & value);

    std::unique_ptr<Entity> createEntity(EntityFactory & factory) const;

};
// class EntityTemplate

}
// namespace logic

#endif // _IWBAN_ENTITYTEMPLATE_HPP_
