/**
 * @file   SolidEntity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SOLIDENTITY_HPP_
#define _IWBAN_SOLIDENTITY_HPP_

#include <Global.hpp>

#include <game/Entity.hpp>

#include <physics/CollisionData.hpp>

namespace phy
{

class Body;

}
// namespace phy

namespace game
{

class SolidEntity
{
public:
    friend class World;


public:
    // Constructor
                    SolidEntity() = default;

    // Virtual destructor
    virtual         ~SolidEntity() {}


protected:
    // Callbacks
    /**
     * Function called when a Body owned by this entity collide with
     * another Body.
     *
     * Physics computations are processed after update and before events.
     */
    virtual void    onCollide(const phy::CollisionData & data) = 0;

    virtual void    onChildAdded(const phy::Body & parent, const phy::Body & child) = 0;

    virtual void    onChildRemoved(const phy::Body & parent, const phy::Body & child) = 0;

};
// class SolidEntity

}
// namespace game

#endif // _IWBAN_SOLIDENTITY_HPP_
