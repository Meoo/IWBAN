/**
 * @file   Texture.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <data/Texture.hpp>

#include <resources/File.hpp>
#include <resources/FileStream.hpp>

#include <system/exceptions/FileNotFound.hpp>

#include <SFML/Graphics/Image.hpp>

#include <map>

namespace
{

static std::map<std::string, std::unique_ptr<sf::Texture> > s_textureDict;

std::unique_ptr<sf::Texture> loadTexture(const std::string & filename)
{
    std::unique_ptr<sf::Texture> texture(new sf::Texture());

    try
    {
        res::File tex_file = res::openFile(filename);
        if (!texture->loadFromMemory(tex_file.getData(), tex_file.getSize()))
        {
            IWBAN_LOG_ERROR("Failed to load texture : '%s'", filename.c_str());

            // Create red / black dummy image
            sf::Image img;
            img.create(32, 32);
            for (unsigned x = 0; x < 32; ++x)
                for (unsigned y = 0; y < 16; ++y)
                    img.setPixel(x, (x < 16 ? y : y + 16), sf::Color::Red);

            texture->loadFromImage(img);
            texture->setRepeated(true);
        }
    }
    catch (const sys::FileNotFound & ignored)
    {
        IWBAN_LOG_ERROR("Texture file not found : '%s'", filename.c_str());

        // Create magenta / black dummy image
        sf::Image img;
        img.create(32, 32);
        for (unsigned x = 0; x < 32; ++x)
            for (unsigned y = 0; y < 16; ++y)
                img.setPixel(x, (x < 16 ? y : y + 16), sf::Color::Magenta);

        texture->loadFromImage(img);
        texture->setRepeated(true);
    }

    return texture;
}
// loadTexture()

}
// namespace

namespace data
{

Texture getTexture(const std::string & filename)
{
    // Try to find texture
    auto it = ::s_textureDict.find(filename);
    if (it != ::s_textureDict.end())
        return it->second.get();

    // Texture not found, load it
    std::unique_ptr<sf::Texture> texture(::loadTexture(filename));

    sf::Texture * textureptr = texture.get();
    ::s_textureDict[filename] = std::move(texture);
    return textureptr;
}

}
// namespace data
