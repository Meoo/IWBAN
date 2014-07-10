/**
 * @file   Controls.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/Controls.hpp>

namespace sys
{

void Controls::Action::activate()
{
    if (!isActive())
        // Set uptime to -1, because it will get updated before next frame
        _uptime = -1;

    ++_activity;
}

void Controls::Action::deactivate()
{
    if (isActive())
    {
        --_activity;

        if (!isActive())
            // Set downtime to -1, because it will get updated before next frame
            _downtime = -1;
    }
    else
        IWBAN_LOG_WARNING("Deactivating an inactive action");
}

void Controls::Action::update()
{
    if (isActive())
        ++_uptime;
    else
        ++_downtime;
}

void Controls::Action::reset()
{
    _activity = 0;
    _uptime = 0;
    _downtime = 0;
}

// ---- ---- ---- ----

void Controls::update()
{
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
        _actions[i].update();
}

void Controls::reset()
{
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
        _actions[i].reset();
}

}
// namespace sys
