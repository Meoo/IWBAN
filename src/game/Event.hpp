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
    EVT_DAMAGE,
    EVT_KILL,

    EVT_ENABLE,
    EVT_DISABLE,

    EVT_TOUCH,

    EVT_ACTION1,
    EVT_ACTION2,
    EVT_ACTION3,
    EVT_ACTION4,

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
