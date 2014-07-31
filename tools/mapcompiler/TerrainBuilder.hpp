/**
 * @file   TerrainBuilder.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_TERRAINBUILDER_HPP_
#define _MAPC_TERRAINBUILDER_HPP_

#include "MapRawData.hpp"

#include <SFML/Graphics.hpp>

int build_terrain(const Map & map)
{
    // TODO Hardcoded screen size :(
    unsigned hori_sections = (map.width * map.tile_width + 320) / 320 - 1;
    unsigned vert_sections = (map.height * map.tile_height + 320) / 240 - 1;

    std::cerr << map.width * map.tile_width << " " << map.height * map.tile_height << std::endl;
    std::cerr << hori_sections << " " << vert_sections << std::endl;

    for (const std::unique_ptr<Layer> & layer : map.layers)
    {
        std::cerr << layer->name << std::endl;

        // Compute min and max x/y for each section
        for (unsigned ys = 0; ys < vert_sections; ++ys)
        {
            unsigned min_y = (ys * map.height) / vert_sections;
            unsigned max_y = ((ys + 1) * map.height) / vert_sections;

            for (unsigned xs = 0; xs < hori_sections; ++xs)
            {
                unsigned min_x = (xs * map.width) / hori_sections;
                unsigned max_x = ((xs + 1) * map.width) / hori_sections;

                std::map<Tileset*, sf::VertexArray> vertices_table;

                // Process a single section
                for (unsigned y = min_y; y < max_y; ++y)
                {
                    for (unsigned x = min_x; x < max_x; ++x)
                    {
                        Tile * tile = layer->data[x + y * map.width];

                        if (!tile) continue;

                        // TODO Group per tileset
                        sf::VertexArray & vertices = vertices_table[tile->tileset];

                        unsigned left       = x * map.tile_width + tile->tileset->offset_x;
                        unsigned right      = left + tile->tileset->tile_width;
                        unsigned top        = y * map.tile_height + tile->tileset->offset_y;
                        unsigned bot        = top + tile->tileset->tile_height;

                        unsigned tex_left   = tile->u;
                        unsigned tex_right  = tex_left + tile->tileset->tile_width;
                        unsigned tex_top    = tile->v;
                        unsigned tex_bot    = tex_top + tile->tileset->tile_height;

                        // Counter clockwise winding
                        // Top left
                        vertices.append(sf::Vertex(sf::Vector2f(left, top), sf::Vector2f(tex_left, tex_top)));

                        // Bottom left
                        vertices.append(sf::Vertex(sf::Vector2f(left, bot), sf::Vector2f(tex_left, tex_bot)));

                        // Bottom right
                        vertices.append(sf::Vertex(sf::Vector2f(right, bot), sf::Vector2f(tex_right, tex_bot)));

                        // Top right
                        vertices.append(sf::Vertex(sf::Vector2f(right, top), sf::Vector2f(tex_right, tex_top)));
                    }
                }

                // Process per tileset and per section data
                for (auto pair : vertices_table)
                {
                    Tileset * tileset                   = pair.first;
                    const sf::VertexArray & vertices    = pair.second;

                    sf::IntRect bounds(vertices.getBounds());

                    // TODO Add to output map
                }
            }
        }
    }

    return 0;
}

#endif // _MAPC_TERRAINBUILDER_HPP_
