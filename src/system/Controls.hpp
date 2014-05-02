/**
 * @file   Controls.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONTROLS_HPP_
#define _IWBAN_CONTROLS_HPP_

#include <Global.hpp>

#include <SFML/System.hpp>

namespace sys
{

enum ActionId
{
    ACT_JUMP = 0,
    ACT_ATTACK,

    ACT_UP,
    ACT_DOWN,
    ACT_LEFT,
    ACT_RIGHT,

    ACT_MENU,

    // Keep last
    ACT_MAX

};
// enum ActionId

// ---- ---- ---- ----

class Action
{
private:
    // Data members
    bool        _active;
    unsigned    _time; // uptime / downtime in updates


public:
    // Constructor
                Action() : _active(false), _time(0) {}

    // Accessors
    bool        isActive() const            { return _active; }

    sf::Time    getTime() const             { return sf::seconds(_time * IWBAN_UPDATE_TIME); }

    bool        isJustActivated() const     { return _active && _time == 0; }
    bool        isJustDeactivated() const   { return !_active && _time == 0; }

    // Functions
    void        activate();
    void        deactivate();

    // Force deactivation
    void        reset();

};
// class Action

// ---- ---- ---- ----

class Controls
{
private:
    // Data members
    Action _actions[ACT_MAX];


public:
    // Functions
    const Action & getAction(ActionId action) const { return _actions[action]; }

    // Deactivate all actions
    // Use this when the focus is lost to prevent inconsistent state
    void reset();

};
// class Controls

// ---- ---- ---- ----

inline
Controls & getControls()
{
    static Controls s_controls;
    return s_controls;
}

}
// namespace sys

#endif // _IWBAN_CONTROLS_HPP_
