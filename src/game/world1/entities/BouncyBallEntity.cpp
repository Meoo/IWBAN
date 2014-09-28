/**
 * @file   BouncyBallEntitycpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/world1/entities/BouncyBallEntity.hpp>

#include <physics/meshes/BallMesh.hpp>

namespace
{

// TODO Use macros / config instead?
const float BALL_SIZE = 24;
const phy::BallMesh BALL_MESH(BALL_SIZE / 2);

// Normalization function, to get 60 FPS values from 50 FPS ones
constexpr float N(float x)
{
    return (x * 50.0f) / IWBAN_UPDATE_RATE;
}

// Gravity force
// Double normalization since it's in px/s^2
const float GRAVITY  = N(N(0.40f));

// Maximum gravity speed
const float GRAVMAX  = N(9.0f);

// Factor applied to gravity and side speed when touching the ground
const float DAMPING  = .9f;

// Overall gravity multiplier
// Can be set to a negative value
const float G        = 1.f;

}
// namespace

namespace game
{

void BouncyBallController::preUpdate(const phy::Space & space, phy::Body & body)
{
    gravity += GRAVITY;
    if (gravity > GRAVMAX)
        gravity = GRAVMAX;

    movement = ut::Vector(side, gravity * G);
}

void BouncyBallController::step(const phy::Space & space, phy::Body & body)
{
    body.setPosition(body.getPosition() + movement / 4); // FIXME Constant
}

void BouncyBallController::resolveContact(
        const phy::Space & space, phy::Body & body, const phy::Contact & contact)
{
    ut::Vector impulse = contact.impulse;

    if (impulse.x != 0 && impulse.y != 0)
    {
        if (std::abs(impulse.x) < std::abs(impulse.y))
            impulse.y = 0;

        else
            impulse.x = 0;
    }

    body.setPosition(body.getPosition() + impulse / 2);

    if (impulse.y != 0
        && (impulse.y < 0) != (movement.y < 0))
    {
        if ((movement.y < 0) == (gravity * G < 0))
        {
            gravity = - gravity * DAMPING;
            side = side * DAMPING;
        }

        movement.y = - movement.y;
    }

    if (impulse.x != 0
        && (impulse.x < 0) != (movement.x < 0))
    {
        if ((movement.x < 0) == (side < 0))
        {
            side = - side;
        }

        movement.x = - movement.x;
    }
}

void BouncyBallController::postUpdate(const phy::Space & space, phy::Body & body)
{
}

// ---- ---- ---- ----

BouncyBallEntity::BouncyBallEntity()
    : _body(&BALL_MESH, &_controller)
{
    _body.setGroup(phy::GROUP_NONE);
    _body.setCollisionMask(phy::GROUP_WORLD);

    // FIXME Debug
    _body.setPosition(ut::Vector(300, 300));
}

BouncyBallEntity::~BouncyBallEntity()
{
    removeBody(&_body);
}

void BouncyBallEntity::doSpawn()
{
    addBody(&_body);
}

}
// namespace game
