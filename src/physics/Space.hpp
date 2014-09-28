/**
 * @file   Space.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPACE_HPP_
#define _IWBAN_SPACE_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>

#include <set>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Body;


typedef char Group;

const Group GROUP_WORLD         = 0x01;
const Group GROUP_OBJECTS       = 0x02;
const Group GROUP_PROJECTILES   = 0x04;
const Group GROUP_TRIGGERS      = 0x08;

const Group GROUP_SPECIAL_1     = 0x10;
const Group GROUP_SPECIAL_2     = 0x20;
const Group GROUP_SPECIAL_3     = 0x40;
const Group GROUP_SPECIAL_4     = 0x80;

const unsigned LAYER_COUNT      = 8;

// ---- ---- ---- ----

class TraceRayQuery
{
public:
    typedef ut::Vector  Vector;

    static const Group DEFAULT_RAY_MASK    = GROUP_WORLD | GROUP_OBJECTS;


    Vector  origin;
    Vector  direction;
    float   distance;

    Group   group_mask  = DEFAULT_RAY_MASK;

};
// class TraceRayQuery

class TraceRayResult
{
public:
    typedef ut::Vector  Vector;

    bool    hit;
    float   t;
    Vector  point;
    Vector  normal;
    Body *  body;

};
// class TraceRayResult

// ---- ---- ---- ----

class Space
{
private:
    typedef std::set<Body *> BodySet;


    // Data members
    BodySet _bodies;

    BodySet _to_attach;
    BodySet _to_detach;


public:
    typedef ut::Vector      Vector;


    // Functions
    void attach(Body * body);
    void detach(Body * body) noexcept;

    void update();


    // Trace functions
    Body * tracePoint(const Vector & point, Group group_mask) const;

    //Body * traceBox(const ut::Rectangle & box, Group group_mask) const;

    TraceRayResult traceRay(const TraceRayQuery & query) const;

    //Contact traceBody(const Body & body) const;

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug) const;
#endif

};
// class Space

}
// namespace phy

#endif // _IWBAN_SPACE_HPP_
