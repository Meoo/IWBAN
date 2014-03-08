/**
 * @file   EntityHandle.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITYHANDLE_HPP_
#define _IWBAN_ENTITYHANDLE_HPP_

#include <Global.hpp>

#include <game/Entity.hpp>

namespace game
{

class EntityHandle
{
private:
    // Data members
    Entity::Id      _id;
    Entity::Serial  _serial;


public:
    EntityHandle(Entity * entity = 0) { reset(entity); }

    void reset(Entity * entity = 0);

          Entity * get();
    const Entity * get() const;

};
// class EntityHandle

}
// namespace game

#endif // _IWBAN_ENTITYHANDLE_HPP_
