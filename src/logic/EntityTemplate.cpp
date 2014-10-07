/**
 * @file   EntityTemplate.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/EntityFactory.hpp>
#include <logic/EntityTemplate.hpp>

namespace logic
{

EntityTemplate::EntityTemplate(const std::string & entity_class)
    : _entity_class(entity_class)
{
}

void EntityTemplate::setValue(const std::string & key, const Variant & value)
{
    if (value.isNull())
    {
        // Try to remove if the value is Null
        auto it = _properties.find(key);
        if (it != _properties.end())
            _properties.erase(it);
    }
    else
        // Otherwise, set it normally
        _properties[key] = value;
}

std::unique_ptr<Entity> EntityTemplate::createEntity(EntityFactory & factory) const
{
    std::unique_ptr<Entity> entity = factory.createEntity(_entity_class);

    for (const auto & it : _properties)
        entity->setValue(it.first, it.second);

    return entity;
}

}
// namespace logic