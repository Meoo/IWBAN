/**
 * @file   Texture.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_TEXTURE_HPP_
#define _IWBAN_TEXTURE_HPP_

#include <Global.hpp>

#include <SFML/Graphics/Texture.hpp>

namespace data
{

typedef sf::Texture * Texture;

Texture getTexture(const std::string & filename);

}
// namespace data

#endif // _IWBAN_TEXTURE_HPP_
