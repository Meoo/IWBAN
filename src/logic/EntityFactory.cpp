/**
 * @file   EntityFactory.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/EntityFactory.hpp>

namespace logic
{

void EntityFactory::registerClass(const std::string & entity_class, EntityAllocator * allocator)
{
    _allocators.insert({entity_class, allocator});
}

std::unique_ptr<Entity> EntityFactory::createEntity(const std::string & entity_class)
{
    auto it = _allocators.find(entity_class);

    if (it != _allocators.end())
        return it->second->create();

    return std::unique_ptr<Entity>(nullptr);
}

}
// namespace logic
