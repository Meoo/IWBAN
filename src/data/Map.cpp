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

namespace data
{

Chunk::Chunk(std::istream & data, const std::vector<sf::Texture *> & texture_table)
    : _vertices(sf::Quads)
{
    _texture = texture_table.at(ut::read<uint32_t>(data));

    _bounds.x = ut::read<uint32_t>(data);
    _bounds.y = ut::read<uint32_t>(data);
    _bounds.w = ut::read<uint32_t>(data);
    _bounds.h = ut::read<uint32_t>(data);

    // Vertices
    uint32_t vertex_count = ut::read<uint32_t>(data);
    _vertices.resize(vertex_count);
    // TODO Check maximum vertex count

    for (uint32_t i = 0; i < vertex_count; ++i)
    {
        _vertices[i].position.x = ut::read<uint32_t>(data);
        _vertices[i].position.y = ut::read<uint32_t>(data);
        _vertices[i].texCoords.x = ut::read<uint32_t>(data);
        _vertices[i].texCoords.y = ut::read<uint32_t>(data);
    }
}

// ---- ---- ---- ----

Layer::Layer(std::istream & data, const std::vector<sf::Texture *> & texture_table)
{
    _name = ut::read<std::string>(data);
    _render_depth = ut::read<int32_t>(data);

    // Chunks
    uint32_t chunk_count = ut::read<uint32_t>(data);
    // TODO Check maximum texture count

    for (uint32_t i = 0; i < chunk_count; ++i)
        _chunks.push_back(std::unique_ptr<Chunk>(new Chunk(data, texture_table)));
}

// ---- ---- ---- ----

Map::Map(const std::string & filename)
{
    res::File map_file(res::openFile(filename));
    res::FileStream data(map_file);


    // Header
    if (ut::read<uint32_t>(data) != IWBAN_MAP_MAGIC)
        throw sys::FileCorrupted(filename.c_str());

    uint32_t version = ut::read<uint32_t>(data);
    if (version != IWBAN_MAP_VERSION)
        throw sys::FileCorrupted(filename.c_str());


    // Texture table
    uint32_t textures_count = ut::read<uint32_t>(data);
    // TODO Check maximum texture count

    std::vector<data::Texture> texture_table;
    texture_table.reserve(textures_count);

    for (uint32_t i = 0; i < textures_count; ++i)
        texture_table.push_back(data::getTexture(ut::read<std::string>(data)));


    // Layers
    uint32_t layer_count = ut::read<uint32_t>(data);
    _layers.reserve(layer_count);
    // TODO Check maximum layer count

    for (uint32_t i = 0; i < layer_count; ++i)
        _layers.push_back(std::unique_ptr<Layer>(new Layer(data, texture_table)));
}

void Map::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    for (const auto & layer : _layers)
    {
        for (const auto & chunk : layer->_chunks)
        {
            states.texture = chunk->_texture;
            target.draw(chunk->_vertices, states);
        }
    }

}

}
// namespace data
