/**
 * @file   EntityFactory.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITYFACTORY_HPP_
#define _IWBAN_ENTITYFACTORY_HPP_

#include <Global.hpp>

namespace logic
{

class Entity;

class EntityAllocator
{
public:
    virtual ~EntityAllocator() {}

    virtual std::unique_ptr<Entity> && create() = 0;

protected:
    template<class T>
    Entity * allocate() { return new T; }

};

// ---- ---- ---- ----

class EntityFactory
{
private:
    std::map<std::string, EntityAllocator *> _allocators;


public:
    // allocator must remain valid for the lifetime of the factory.
    void    registerClass(const std::string & entity_class, EntityAllocator * allocator);

    std::unique_ptr<Entity> && createEntity(const std::string & entity_class) const;

};
// class EntityFactory

// ---- ---- ---- ----

inline
EntityFactory & getEntityFactory()
{
    static EntityFactory s_entityFactory;
    return s_entityFactory;
}

// ---- ---- ---- ----

/**
 * Class allowing to auto-register
 */
template<class T, const char * C>
class AutoLinkedAllocator final : public EntityAllocator
{
public:
    AutoLinkedAllocator()
    {
        getEntityFactory().registerClass(C, this);
    }

    std::unique_ptr<Entity> && create() override
    {
        return std::unique_ptr<Entity>(allocate<T>());
    }
};
// class AutoLinkedAllocator

#define LINK_ENTITY_TO_CLASS(entityclass, cclass) \
    static logic::AutoLinkedAllocator<cclass, #entityclass> __ ## entityclass ## _allocator

}
// namespace logic

#endif // _IWBAN_ENTITYFACTORY_HPP_
