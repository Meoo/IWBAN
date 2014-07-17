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

class SolidEntity : public Entity
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
    virtual void    onStartTouch() = 0;

    virtual void    onTouch() = 0;

    virtual void    onEndTouch() = 0;

    virtual void    onChildAdded(const phy::Body & parent, const phy::Body & child) = 0;

    virtual void    onChildRemoved(const phy::Body & parent, const phy::Body & child) = 0;

};
// class SolidEntity

}
// namespace game

#endif // _IWBAN_SOLIDENTITY_HPP_
