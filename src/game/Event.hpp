/**
 * @file   Event.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_EVENT_HPP_
#define _IWBAN_EVENT_HPP_

#include <Global.hpp>

namespace game
{

class Entity;

enum EventId
{
    EVENT_DAMAGE,
    EVENT_KILL,

    EVENT_ENABLE,
    EVENT_DISABLE,

    EVENT_TOUCH,

    EVENT_ACTION1,
    EVENT_ACTION2,
    EVENT_ACTION3,
    EVENT_ACTION4,

};
// enum EventId

// ---- ---- ---- ----

class Event
{
public:
    Event(const EventId & evt_id)
        : source(0), event_id(evt_id), param(0)
    {}

    Entity *    source;

    EventId     event_id;

    void *      param;

    // TODO Event delay ?

};
// class Event

}
// namespace game

#endif // _IWBAN_EVENT_HPP_
