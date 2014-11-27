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
class World;

/**
 * @brief Class allowing to build entities with predefined parameters.
 */
class EntityTemplate
{
private:
    std::string                     _entity_class;
    std::string                     _entity_name;
    std::map<std::string, Variant>  _properties;


public:
    explicit    EntityTemplate(const std::string & entity_class);

    void        setName(const std::string & name) { _entity_name = name; }
    void        setValue(const std::string & key, const Variant & value);

    std::unique_ptr<Entity> createEntity(World & world) const;
    Entity *                createAndOwnEntity(World & factory) const;

};
// class EntityTemplate

}
// namespace logic

#endif // _IWBAN_ENTITYTEMPLATE_HPP_
