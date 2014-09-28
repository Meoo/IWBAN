/**
 * @file   PlayerEntity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PLAYERENTITY_HPP_
#define _IWBAN_PLAYERENTITY_HPP_

#include <Global.hpp>

#include <logic/Entity.hpp>

#include <physics/Body.hpp>
#include <physics/Controller.hpp>

namespace game
{

class PlayerController : public phy::Controller
{
private:
    float gravity   = 0;

    ut::Vector movement;

    bool on_ground = false;
    bool double_jump = false;


public:
    void preUpdate(const phy::Space & space, phy::Body & body) override;

    void step(const phy::Space & space, phy::Body & body) override;

    void resolveContact(const phy::Space & space, phy::Body & body,
                                const phy::Contact & contact) override;

    void postUpdate(const phy::Space & space, phy::Body & body) override;

};
// class PlayerController

class PlayerEntity final : public logic::Entity
{
private:
    PlayerController    _controller;
    phy::Body           _body;


public:
    PlayerEntity();
    ~PlayerEntity();

    const ut::Vector & getPosition() const { return _body.getPosition(); }
    void setPosition(const ut::Vector & position) { _body.setPosition(position); }


protected:
    void doSpawn() override;

};
// class PlayerEntity

}
// namespace game

#endif // _IWBAN_PLAYERENTITY_HPP_
