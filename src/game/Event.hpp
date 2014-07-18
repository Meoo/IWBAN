/**
 * @file   Event.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_EVENT_HPP_
#define _IWBAN_EVENT_HPP_

#include <Global.hpp>

#include <game/Entity.hpp>

#include <utils/Vector.hpp>

#include <memory>

namespace game
{

enum EventId
{
    EVT_DAMAGE,

    EVT_ENABLE,
    EVT_DISABLE,

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
    // Base class for Event dynamic parameters
    class Param { public: virtual ~Param() {} };

    Event(const EventId & evt_id)
        : event_id(evt_id)
    {}

    EventId                 event_id;

    uint32_t                int_param   = 0;
    ut::Vector              vector_param;
    EntityHandle            entity_param;
    std::unique_ptr<Param>  ptr_param; // custom_param ?

};
// class Event

}
// namespace game

#endif // _IWBAN_EVENT_HPP_
