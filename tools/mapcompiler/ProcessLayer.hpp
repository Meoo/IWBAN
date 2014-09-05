/**
 * @file   TerrainBuilder.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_TERRAINBUILDER_HPP_
#define _MAPC_TERRAINBUILDER_HPP_

#include "InputMap.hpp"

#include <config/DisplayConfig.hpp>
#include <utils/StreamIO.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>


int process_chunks(const InputMap & map, const Layer & layer, std::ostream & output)
{
    class Vertice
    {
    public:
        uint16_t x, y, u, v;
    };

    typedef std::vector<Vertice> VertVector;

    class Chunk
    {
    public:
        uint8_t texture_id;
        uint16_t x, y, w, h; // Bounds
        VertVector vertices;
    };

    // ---- ---- ---- ----

    std::vector<Chunk> chunk_table;

    const unsigned hori_sections = (map.width * map.tile_width + (IWBAN_FRAME_WIDTH/2)) / (IWBAN_FRAME_WIDTH/2) - 1;
    const unsigned vert_sections = (map.height * map.tile_height + (IWBAN_FRAME_HEIGHT/2)) / (IWBAN_FRAME_HEIGHT/2) - 1;

    // Compute min and max x/y for each section
    for (unsigned ys = 0; ys < vert_sections; ++ys)
    {
        const unsigned min_y = (ys * map.height) / vert_sections;
        const unsigned max_y = ((ys + 1) * map.height) / vert_sections;

        for (unsigned xs = 0; xs < hori_sections; ++xs)
        {
            const unsigned min_x = (xs * map.width) / hori_sections;
            const unsigned max_x = ((xs + 1) * map.width) / hori_sections;

            // We will group tiles per texture_id
            std::map<uint8_t, VertVector> vertices_table;

            // Process every tile in current section
            for (unsigned y = min_y; y < max_y; ++y)
            {
                for (unsigned x = min_x; x < max_x; ++x)
                {
                    Tile * tile = layer.data[x + y * map.width];

                    if (!tile) continue;

                    // Get the vertex table for texture_id (create it if needed)
                    VertVector & vertices = vertices_table[tile->tileset->texture_id];

                    uint16_t left       = x * map.tile_width + tile->tileset->offset_x;
                    uint16_t right      = left + tile->tileset->tile_width;
                    uint16_t top        = y * map.tile_height + tile->tileset->offset_y;
                    uint16_t bot        = top + tile->tileset->tile_height;

                    uint16_t tex_left   = tile->u;
                    uint16_t tex_right  = tex_left + tile->tileset->tile_width;
                    uint16_t tex_top    = tile->v;
                    uint16_t tex_bot    = tex_top + tile->tileset->tile_height;

                    // Counter clockwise winding
                    // Top left
                    vertices.push_back({left, top,
                                        tex_left, tex_top});

                    // Bottom left
                    vertices.push_back({left, bot,
                                        tex_left, tex_bot});

                    // Bottom right
                    vertices.push_back({right, bot,
                                        tex_right, tex_bot});

                    // Top right
                    vertices.push_back({right, top,
                                        tex_right, tex_top});
                }
            }

            // Process per texture_id and per section = a single chunk
            for (auto & pair : vertices_table)
            {
                Chunk chunk;
                chunk.texture_id = pair.first;
                chunk.vertices = std::move(pair.second);

                // Compute bounds the hard way
                uint32_t min_x, min_y;
                uint32_t max_x, max_y;

                max_x = min_x = chunk.vertices.at(0).x;
                max_y = min_y = chunk.vertices.at(0).y;

                for (const Vertice & vertice : chunk.vertices)
                {
                    if (vertice.x < min_x)
                        min_x = vertice.x;
                    else if (vertice.x > max_x)
                        max_x = vertice.x;

                    if (vertice.y < min_y)
                        min_y = vertice.y;
                    else if (vertice.y > max_y)
                        max_y = vertice.y;
                }

                chunk.x = min_x;
                chunk.y = min_y;
                chunk.w = max_x - min_x;
                chunk.h = max_y - min_y;

                chunk_table.push_back(std::move(chunk));
            }
        }
    }

    // Write chunks to file
    ut::write<uint8_t>(output, chunk_table.size());

    // TODO Can be re-ordered by texture for a tiny performance boost
    for (const Chunk & chunk : chunk_table)
    {
        ut::write<uint8_t>(output, chunk.texture_id);

        ut::write<uint16_t>(output, chunk.x);
        ut::write<uint16_t>(output, chunk.y);
        ut::write<uint16_t>(output, chunk.w);
        ut::write<uint16_t>(output, chunk.h);

        ut::write<uint16_t>(output, chunk.vertices.size());

        for (const Vertice & vertice : chunk.vertices)
        {
            ut::write<uint16_t>(output, vertice.x);
            ut::write<uint16_t>(output, vertice.y);
            ut::write<uint16_t>(output, vertice.u);
            ut::write<uint16_t>(output, vertice.v);
        }
    }

    return 0;
}
// process_chunks()

// ---- ---- ---- ----

int process_layer(const InputMap & map, const Layer & layer, std::ostream & output)
{
    // Layer header
    ut::write<std::string>(output, layer.name);

    // TODO Render depth
    ut::write<int32_t>(output, 0);

    int ret;

    // Chunks
    ret = process_chunks(map, layer, output);
    if (ret)
        return ret;

    // TODO Bodies

    // TODO Shadows

    return 0;
}

#endif // _MAPC_TERRAINBUILDER_HPP_
