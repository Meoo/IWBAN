/**
 * @file   Entity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITY_HPP_
#define _IWBAN_ENTITY_HPP_

#include <Global.hpp>

#include <game/Event.hpp>

#include <graphics/Renderer.hpp>

#include <system/Time.hpp>

namespace game
{

class World;

class Entity
{
public:
    friend class World;

    typedef uint16_t Id;
    typedef uint16_t Serial;

    static const Id  INVALID_ID = -1;


private:
    // Data members
    World *         _world  = nullptr;
    Id              _id     = INVALID_ID;
    Serial          _serial;

    std::string     _name;

    bool            _valid  = false;
    bool            _alive  = false;

    sys::FTime      _next_update;


public:
    // Constructor
                    Entity() = default;

    // Virtual destructor
    virtual         ~Entity() {}

    // Functions
    void            sendEvent(Entity * source, EventId id)
    {
        BOOST_ASSERT(isValid());
        // TODO getWorld().queueEvent(source, this, id);
    }

    void            sendDelayedEvent(Entity * source, EventId id, sys::FTimeOffset delay)
    {
        BOOST_ASSERT(isValid());
        // TODO getWorld().queueDelayedEvent(source, this, id, delay);
    }

    /**
     * Kill the Entity.
     *
     * The function #onDeath will be called if the Entity was alive.
     */
    void            kill()
    {
        if (_alive)
        {
            onDeath();
            _alive = false;
        }
    }

    /**
     * Exile the Entity from the World.
     *
     * The function #onDeath will not be called, but #onDespawn will.
     */
    void            exile()
    {
        _alive = false;
    }

    // Accessors
    World &         getWorld()  const { BOOST_ASSERT(isValid()); return *_world; }

    /**
     * Get the unique identifier for this Entity.
     *
     * This identifier is unique only within the World this Entity is in.
     * An identifier is recycled when the corresponding Entity is despawned.
     * This value is undefined if the Entity is not valid.
     */
    Id              getId()     const { return _id; }

    /**
     * Get this Entity's serial number.
     *
     * This number is used in EntityHandle to differentiate an old despawned
     * Entity with a newly spawned one with the same unique identifier.
     * This value is undefined if the Entity is not valid.
     */
    Serial          getSerial() const { return _serial; }

    /**
     * Get this Entity's name.
     *
     * A name is used to target an Entity without knowing it's identifier.
     */
    const std::string getName() const { return _name; }

    /**
     * Set the name of this Entity.
     *
     * You cannot change the name of an Entity once it has been spawned in a World.
     */
    void            setName(const std::string & name) { BOOST_ASSERT(isValid()); _name = name; }

    /**
     * Return true if the Entity is in a valid state.
     *
     * An Entity is valid if it exists within a World, dead or alive.
     * You cannot set an EntityHandle to point to an invalid Entity.
     */
    bool            isValid()   const { return _valid; }

    /**
     * Return true if the Entity is alive.
     *
     * A dead entity will be despawned automatically.
     */
    bool            isAlive()   const { return _alive; }


protected:
    // Callbacks
    /**
     * Update function.
     *
     * Use #scheduleNextUpdate or this function will not be called again.
     */
    virtual void    onUpdate() = 0;

    /**
     * Function called when the Entity just spawned into a World.
     */
    virtual void    onSpawn() = 0;

    /**
     * Function called when an Entity is removed from a World.
     *
     * This function should leave the Entity in an invalid state.
     * #revive may be called in order to prepare the entity to be respawned.
     */
    virtual void    onDespawn() = 0;

    /**
     * Function called soon after the Entity is spawned.
     *
     * This function is always called on the same frame the Entity is created.
     * Use #scheduleNextUpdate in this function to schedule the first update.
     */
    virtual void    onBirth() = 0;

    /**
     * Function called the first time #kill is called.
     *
     * This function is not called if the Entity is killed using #exile.
     */
    virtual void    onDeath() = 0;


    virtual void    onEvent(const Event & event) = 0 ;

    // Protected functions for child classes
    /**
     * Schedule the next update of this Entity.
     */
    void            scheduleNextUpdate(sys::FTimeOffset delay)
    {
        BOOST_ASSERT(isValid());
        // TODO _next_update = getWorld().getClock() + delay;
    }


private:
    // Private functions for World
    void            spawn(World * world, Id id, Serial serial)
    {
        BOOST_ASSERT_MSG(!isValid(), "Entity is already spawned");

        _world = world;
        _serial = serial;
        _id = id;
        _valid = true;
        onSpawn();
    }

    void            despawn()
    {
        onDespawn();
        _valid = false;
        _alive = false;
        _world = nullptr;
    }

    void            birth()
    {
        _alive = true;
        onBirth();
    }

};
// class Entity

// ---- ---- ---- ----

class EntityHandle
{
private:
    // Data members
    World *         _world;
    Entity::Id      _id;
    Entity::Serial  _serial;


public:
    EntityHandle(Entity * entity = nullptr) { reset(entity); }

    void reset(Entity * entity = nullptr);

          Entity * get();
    const Entity * get() const;

};
// class EntityHandle

}
// namespace game

#endif // _IWBAN_ENTITY_HPP_
