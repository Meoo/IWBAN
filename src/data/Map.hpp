/**
 * @file   Map.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAP_HPP_
#define _IWBAN_MAP_HPP_

#include <Global.hpp>

#include <data/Texture.hpp>
#include <utils/Rectangle.hpp>

#include <SFML/Graphics.hpp>

#include <boost/noncopyable.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace data
{

class Chunk : public boost::noncopyable
{
private:
    ut::Rectangle   _bounds;

    data::Texture   _texture    = nullptr;
    //data::Shader    _shader     = nullptr;
    sf::VertexArray _vertices;

    // FIXME Remove me
    friend class Map;


public:
    explicit Chunk(std::istream & data, const std::vector<sf::Texture *> & texture_table);

};
// class Chunk

// ---- ---- ---- ----

class Layer : public boost::noncopyable
{
private:
    typedef std::vector<std::unique_ptr<Chunk>> ChunkVector;

    std::string _name;
    int         _render_depth   = 0;

    ChunkVector _chunks;

    // FIXME Remove me
    friend class Map;


public:
    explicit Layer(std::istream & data, const std::vector<sf::Texture *> & texture_table);

};
// class Layer

// ---- ---- ---- ----

class Map : public boost::noncopyable, public sf::Drawable
{
private:
    typedef std::vector<std::unique_ptr<Layer>> LayerVector;

    sf::Color   _ambient_light;

    LayerVector _layers;

    std::string _lua_script;


public:
    explicit Map(const std::string & filename);

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

};
// class Map

}
// namespace data

#endif // _IWBAN_MAP_HPP_
