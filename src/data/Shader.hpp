/**
 * @file   Shader.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SHADER_HPP_
#define _IWBAN_SHADER_HPP_

#include <Global.hpp>

#include <SFML/Graphics/Shader.hpp>

namespace data
{

typedef sf::Shader * Shader;

Shader getShader(const std::string & filename);

}
// namespace data

#endif // _IWBAN_SHADER_HPP_
