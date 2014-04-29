/**
 * @file   ShadowVolume.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SHADOWVOLUME_HPP_
#define _IWBAN_SHADOWVOLUME_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

#include <vector>

namespace gfx
{

class ShadowVolume
{
public:
    struct Line
    {
        Vector first;
        Vector second;
    };
    // struct Line


private:
    // Data members
    std::vector<Line> _lines;


public:
    // Functions
    ut::Rectangle getBoundingBox() const;

};
// class ShadowVolume

}
// namespace gfx

#endif // _IWBAN_SHADOWVOLUME_HPP_
