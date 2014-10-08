/**
 * @file   Map.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/MapConfig.hpp>
#include <data/Map.hpp>
#include <data/Texture.hpp>
#include <resources/File.hpp>
#include <resources/FileStream.hpp>
#include <system/exceptions/FileCorrupted.hpp>
#include <utils/StreamIO.hpp>

#include <map>

namespace
{

static std::map<std::string, std::unique_ptr<data::Map> > s_mapDict;

std::unique_ptr<data::Map> loadMap(const std::string & filename)
{
    try
    {
        res::File file(res::openFile(filename));
        res::FileStream istr(file);

        istr.exceptions(std::ios::eofbit
                      | std::ios::failbit
                      | std::ios::badbit);

        std::unique_ptr<data::Map> map(new data::Map(istr));

        if (istr.tellg() != (int) file.getSize())
            IWBAN_LOG_WARNING("Data left after reading '%s'", filename.c_str());

        return map;
    }
    catch (const sys::DataCorrupted & e)
    {
        throw sys::FileCorrupted(filename.c_str(), e);
    }
    catch (const std::ios::failure & e)
    {
        throw sys::FileCorrupted(filename.c_str(), "IO failure");
    }
}
// loadMap()

}
// namespace

namespace data
{

namespace map
{

Chunk::Chunk(std::istream & data, const TextureTable & texture_table)
{
    _texture = texture_table.at(ut::read<uint8_t>(data));

    _bounds.left    = ut::read<uint16_t>(data);
    _bounds.top     = ut::read<uint16_t>(data);
    _bounds.right   = _bounds.left + ut::read<uint16_t>(data);
    _bounds.bottom  = _bounds.top + ut::read<uint16_t>(data);

    // Vertices
    uint32_t vertex_count = ut::read<uint16_t>(data);
    _vertices.resize(vertex_count);
    // TODO Check maximum vertex count

    for (uint32_t i = 0; i < vertex_count; ++i)
    {
        _vertices[i].position.x = ut::read<uint16_t>(data);
        _vertices[i].position.y = ut::read<uint16_t>(data);
        _vertices[i].texCoords.x = ut::read<uint16_t>(data);
        _vertices[i].texCoords.y = ut::read<uint16_t>(data);
    }
}

void Chunk::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.texture = _texture;
    target.draw(_vertices, states);
}

// ---- ---- ---- ----

BodyDesc::BodyDesc(std::istream & data)
{
    ut::Rectangle rec;

    rec.left    = ut::read<uint16_t>(data);
    rec.top     = ut::read<uint16_t>(data);
    rec.right   = rec.left + ut::read<uint16_t>(data);
    rec.bottom  = rec.top + ut::read<uint16_t>(data);

    _mesh.setBounds(rec);
}

BodyDesc::BodyPtr BodyDesc::makeBody() const
{
    BodyPtr p (new phy::Body(&_mesh));
    p->setGroup(phy::GROUP_WORLD);
    p->setCollisionMask(phy::GROUP_NONE);
    return p;
}

// ---- ---- ---- ----

class Layer::LayerDrawable final : public gfx::Drawable
{
private:
    const Layer * _layer;


public:
    LayerDrawable(const Layer * layer)
        : _layer(layer) {}


protected:
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override
    {
        // TODO Cull chunks based on camera view
        for (const auto & chunk : _layer->getChunks())
            target.draw(*chunk, states);
    }

};
// class LayerDrawable

Layer::Layer(std::istream & data, const TextureTable & texture_table)
{
    _name = ut::read<std::string>(data);
    _render_depth = ut::read<int32_t>(data);


    // Chunks
    uint32_t chunk_count = ut::read<uint8_t>(data);
    _chunks.reserve(chunk_count);
    // TODO Check maximum chunk count

    for (uint32_t i = 0; i < chunk_count; ++i)
    {
        ChunkPtr p(new Chunk(data, texture_table));
        _chunks.push_back(std::move(p));
    }


    // Bodies
    uint32_t body_count = ut::read<uint8_t>(data);
    _bodies.reserve(body_count);
    // TODO Check maximum bodies count

    for (uint32_t i = 0; i < body_count; ++i)
    {
        BodyDescPtr b(new BodyDesc(data));
        _bodies.push_back(std::move(b));
    }
}
Layer::DrawablePtr Layer::makeDrawable() const
{
    DrawablePtr d(new LayerDrawable(this));
    return d;
}

}
// namespace map

// ---- ---- ---- ----

Map::Map(std::istream & data)
{
    // Header
    if (ut::read<uint32_t>(data) != IWBAN_MAP_MAGIC)
        throw sys::DataCorrupted("Invalid magic");

    uint32_t version = ut::read<uint8_t>(data);
    if (version != IWBAN_MAP_VERSION)
        throw sys::DataCorrupted("Invalid version");


    // Texture table
    uint32_t textures_count = ut::read<uint8_t>(data);
    // TODO Check maximum texture count

    map::TextureTable texture_table;
    texture_table.reserve(textures_count);

    for (uint32_t i = 0; i < textures_count; ++i)
        texture_table.push_back(data::getTexture(ut::read<std::string>(data)));


    // Layers
    uint32_t layer_count = ut::read<uint8_t>(data);
    _layers.reserve(layer_count);
    // TODO Check maximum layer count

    for (uint32_t i = 0; i < layer_count; ++i)
    {
        LayerPtr l(new map::Layer(data, texture_table));
        _layers.push_back(std::move(l));
    }


    // Lua script
    std::string script = ut::read<std::string>(data);
    if (!script.empty())
        _lua_script = logic::LuaScript::fromFile(script);
}

// ---- ---- ---- ----

Map * getMap(const std::string & filename)
{
    // Try to find map
    auto it = ::s_mapDict.find(filename);
    if (it != ::s_mapDict.end())
        return it->second.get();

    // Map not found, load it
    std::unique_ptr<Map> map(::loadMap(filename));

    Map * mapptr = map.get();
    ::s_mapDict[filename] = std::move(map);
    return mapptr;
}

}
// namespace data
