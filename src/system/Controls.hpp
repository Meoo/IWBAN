/**
 * @file   Controls.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONTROLS_HPP_
#define _IWBAN_CONTROLS_HPP_

#include <Global.hpp>

#include <system/Time.hpp>

#include <SFML/System.hpp>

namespace sys
{

enum ActionId
{
    // Game actions
    ACT_JUMP = 0,
    ACT_FIRE,
    ACT_RETRY,

    // Menu actions
    ACT_ACCEPT,
    ACT_MENU,

    // Navigation actions
    ACT_UP,
    ACT_DOWN,
    ACT_LEFT,
    ACT_RIGHT,

    // Number of actions, keep last
    ACT_COUNT,

};
// enum ActionId

// ---- ---- ---- ----

class Controls
{
public:
    class Action
    {
        friend class Controls;

    private:
        // Data members
        unsigned    _activity   = 0;

        FTime       _uptime     = 0;
        FTime       _downtime   = 0;


    public:
        // Constructor
                    Action() = default;

        // Accessors
        bool        isActive() const            { return _activity > 0; }

        sf::Time    getUptime() const           { return sf::Int64(_uptime) * IWBAN_UPDATE_TIME; }
        sf::Time    getDowntime() const         { return sf::Int64(_downtime) * IWBAN_UPDATE_TIME; }

        bool        isJustActivated() const     { return isActive() && _uptime == 0; }
        bool        isJustDeactivated() const   { return !isActive() && _downtime == 0; }

        // Functions
        void        activate();
        void        deactivate();

    private:
        // Private functions for Controls
        // Call this every update to increase logic up/down time
        void        update();

        // Force deactivation
        void        reset();

    };
    // class Action


private:
    // Data members
    Action  _actions[ACT_COUNT];


public:
    // Functions
    Action & getAction(ActionId action) { return _actions[action]; }

    // Call this right before every update
    void    update();

    // Deactivate all actions
    // Use this when the focus is lost to prevent inconsistent state
    void    reset();

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
