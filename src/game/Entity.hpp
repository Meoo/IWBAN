/**
 * @file   Entity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITY_HPP_
#define _IWBAN_ENTITY_HPP_

#include <Global.hpp>

#include <game/Event.hpp>

#include <graphics/Renderer.hpp>

namespace game
{

class World;

class Entity
{
public:
    friend class World;

    typedef uint16_t Id;
    typedef uint16_t Serial;

    static const Id INVALID_ID = -1;


private:
    // Data members
    Id              _id;
    Serial          _serial;

    bool            _alive;


public:
                    Entity();

    virtual         ~Entity() {}

    Id              getId()     const { return _id; }

    Serial          getSerial() const { return _serial; }

    bool            isAlive()   const { return _alive; }

    virtual void    send(const Event & event) = 0;


protected:
    // Protected functions for World
    virtual void    update() = 0;

    virtual void    spawn() = 0;

    virtual void    render(gfx::Renderer & renderer) const = 0;

    virtual void    renderLight(gfx::Renderer & renderer) const = 0;

    void            setId(Id id)                { _id = id; }

    void            setSerial(Serial serial)    { _serial = serial; }

};
// class Entity

}
// namespace game

#endif // _IWBAN_ENTITY_HPP_
