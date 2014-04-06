/**
 * @file   ShadowVolume.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SHADOWVOLUME_HPP_
#define _IWBAN_SHADOWVOLUME_HPP_

#include <Global.hpp>

#include <boost/container/vector.hpp>
#include <boost/intrusive/set.hpp>

namespace gfx
{

class ShadowVolume : public boost::intrusive::set_base_hook<>
{
public:
    typedef boost::intrusive::set<Drawable> List;

    struct Line
    {
        Vector2 first;
        Vector2 second;
    };
    // struct Line


private:
    // Data members
    boost::container::vector<Line> _lines;


public:

};
// class ShadowVolume

}
// namespace gfx

#endif // _IWBAN_SHADOWVOLUME_HPP_
