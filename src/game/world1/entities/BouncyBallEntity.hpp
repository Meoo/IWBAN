/**
 * @file   BouncyBallEntity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BOUNCYBALLENTITY_HPP_
#define _IWBAN_BOUNCYBALLENTITY_HPP_

#include <Global.hpp>

#include <logic/Entity.hpp>

#include <physics/Body.hpp>
#include <physics/Controller.hpp>

namespace game
{

class BouncyBallController : public phy::Controller
{
private:
    float gravity   = 0;
    float side      = -4.5f;

    ut::Vector movement;


public:
    void preUpdate(const phy::Space & space, phy::Body & body) override;

    void step(const phy::Space & space, phy::Body & body) override;

    void resolveContact(const phy::Space & space, phy::Body & body,
                                const phy::Contact & contact) override;

    void postUpdate(const phy::Space & space, phy::Body & body) override;

};
// class BouncyBallController

class BouncyBallEntity final : public logic::Entity
{
private:
    BouncyBallController    _controller;
    phy::Body               _body;


public:
    BouncyBallEntity();
    ~BouncyBallEntity();


protected:
    void doSpawn() override;

};
// class BouncyBallEntity

}
// namespace game

#endif // _IWBAN_BOUNCYBALLENTITY_HPP_
