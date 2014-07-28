/**
 * @file   LayerEntity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LAYERENTITY_HPP_
#define _IWBAN_LAYERENTITY_HPP_

#include <Global.hpp>

#include <game/entities/SolidEntity.hpp>

namespace game
{

class LayerEntity : public SolidEntity
{
public:
    // Constructor
                    LayerEntity() = default;

    // Virtual destructor
    virtual         ~LayerEntity() {}

};
// class LayerEntity

}
// namespace game

#endif // _IWBAN_LAYERENTITY_HPP_
