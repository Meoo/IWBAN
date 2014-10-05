/**
 * @file   Map.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAP_HPP_
#define _IWBAN_MAP_HPP_

#include <Global.hpp>

#include <data/Texture.hpp>
#include <graphics/Drawable.hpp>
#include <logic/Lua.hpp>
#include <physics/Body.hpp>
#include <physics/meshes/AABoxMesh.hpp>
#include <utils/Rectangle.hpp>

#include <SFML/Graphics.hpp>

#include <boost/noncopyable.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace data
{

namespace map
{

typedef std::vector<sf::Texture *> TextureTable;

// ---- ---- ---- ----

class Chunk : public boost::noncopyable, public sf::Drawable
{
private:
    // Data members
    ut::Rectangle   _bounds;

    data::Texture   _texture    = nullptr;
    //data::Shader    _shader     = nullptr;
    sf::VertexArray _vertices   = sf::VertexArray(sf::Quads);


public:
    // Constructor
    Chunk(std::istream & data, const TextureTable & texture_table);

    // Getters / setters
    const ut::Rectangle & getBounds() const { return _bounds; }


protected:
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

};
// class Chunk

// ---- ---- ---- ----

class BodyDesc
{
public:
    typedef std::unique_ptr<phy::Body> BodyPtr;


private:
    // Data members
    phy::AABoxMesh _mesh;


public:
    // Constructor
    explicit BodyDesc(std::istream & data);

    // Functions
    BodyPtr makeBody() const;

};
// class BodyDesc

// ---- ---- ---- ----

class Layer : public boost::noncopyable
{
public:
    typedef std::unique_ptr<BodyDesc>   BodyDescPtr;
    typedef std::vector<BodyDescPtr>    BodyDescVector;

    typedef std::unique_ptr<gfx::Drawable> DrawablePtr;

private:
    class LayerDrawable;

    typedef std::unique_ptr<Chunk>      ChunkPtr;
    typedef std::vector<ChunkPtr>       ChunkVector;


    // Data members
    std::string     _name;
    int             _render_depth   = 0;

    ChunkVector     _chunks;
    BodyDescVector  _bodies;


public:
    // Constructor
    Layer(std::istream & data, const TextureTable & texture_table);

    // Getters / setters
    const std::string & getName() const { return _name; }
    const BodyDescVector & getBodies() const { return _bodies; }

    // Functions
    DrawablePtr makeDrawable() const;


private:
    friend class LayerDrawable;

    const ChunkVector & getChunks() const { return _chunks; }

};
// class Layer

}
// namespace map

// ---- ---- ---- ----

class Map : public boost::noncopyable
{
public:
    typedef std::unique_ptr<map::Layer> LayerPtr;
    typedef std::vector<LayerPtr>       LayerVector;


private:
    // Data members
    sf::Color           _ambient_light;

    LayerVector         _layers;

    logic::LuaScript    _lua_script;


public:
    // Constructor
    explicit Map(std::istream & data);

    // Getters / setters
    const LayerVector & getLayers() const { return _layers; }

};
// class Map

// ---- ---- ---- ----

Map * getMap(const std::string & filename);

}
// namespace data

#endif // _IWBAN_MAP_HPP_
