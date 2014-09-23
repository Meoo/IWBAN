/**
 * @file   PlayerEntity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/core/entities/PlayerEntity.hpp>

#include <physics/meshes/AABoxMesh.hpp>

#include <system/Controls.hpp>

namespace
{

// TODO Use macros / config instead?
const phy::AABoxMesh PLAYER_MESH(ut::Rectangle(-5, -10, 6, 11));

const float JUMP     = 8.5f;
const float DJUMP    = 7.0f;
const float JUMPSTOP = 0.45f;
const float GRAVITY  = 0.40f;
const float GRAVMAX  = 9.0f;
const float SIDE     = 2.5f;

const float G        = 1.f;

}
// namespace

namespace game
{

void PlayerController::preUpdate(const phy::Space & space, phy::Body & body)
{
    sys::Controls & ctrl = sys::getControls();

    // Gravity & jumps
    if (ctrl.getAction(sys::ACT_JUMP).isJustActivated())
    {
        if (on_ground)
        {
            gravity = - JUMP;
            on_ground = false;
        }
        else if (double_jump)
        {
            gravity = - DJUMP;
            double_jump = false;
        }
    }

    if (ctrl.getAction(sys::ACT_JUMP).isJustDeactivated()
            && gravity < 0)
    {
        gravity *= JUMPSTOP;
    }

    gravity += GRAVITY;
    if (gravity > GRAVMAX)
        gravity = GRAVMAX;

    on_ground = false;

    // Side movements

    if (ctrl.getAction(sys::ACT_LEFT).isActive())
        movement = ut::Vector(- SIDE, gravity * G);

    else if (ctrl.getAction(sys::ACT_RIGHT).isActive())
        movement = ut::Vector(SIDE, gravity * G);

    else
        movement = ut::Vector(0, gravity * G);
}

void PlayerController::step(const phy::Space & space, phy::Body & body)
{
    body.setPosition(body.getPosition() + movement / 4); // FIXME Constant
}

void PlayerController::resolveContact(
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
        if (movement.y * G > 0)
        {
            on_ground = true;
            double_jump = true;
        }
        else
            movement.y = 0;

        gravity = 0;
    }

    if (impulse.x != 0
        && (impulse.x < 0) != (movement.x < 0))
    {
        movement.x = 0;
    }
}

void PlayerController::postUpdate(const phy::Space & space, phy::Body & body)
{
}

// ---- ---- ---- ----

PlayerEntity::PlayerEntity()
    : _body(&PLAYER_MESH, &_controller)
{
    _body.setPosition(ut::Vector(75, 75));
}

PlayerEntity::~PlayerEntity()
{
    removeBody(&_body);
}

void PlayerEntity::doSpawn()
{
    addBody(&_body);
}

}
// namespace game
