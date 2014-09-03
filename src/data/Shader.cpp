/**
 * @file   Shader.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <data/Shader.hpp>

#include <resources/File.hpp>
#include <resources/FileStream.hpp>

#include <system/exceptions/FileNotFound.hpp>

#include <map>

namespace
{

static std::map<std::string, std::unique_ptr<sf::Shader> > s_shaderDict;

std::unique_ptr<sf::Shader> loadShader(const std::string & filename)
{
    std::unique_ptr<sf::Shader> shader(new sf::Shader());

    std::string glfs; // Fragment shader
    std::string glvs; // Vertex shader
    std::string ctex; // Current texture parameter

    try
    {
        res::File shader_file = res::openFile(filename);
        res::FileStream f(shader_file);

        while (!f.eof())
        {
            std::string n;
            f >> n >> std::ws;

            if (n == "glfs") f >> glfs;
            else if (n == "glvs") f >> glvs;
            else if (n == "ctex") f >> ctex;
            else
                IWBAN_LOG_ERROR("Invalid key : '%s' in file '%s'",
                                n.c_str(), filename.c_str());

            if (f.bad() || f.fail()) // eof is not an error
            {
                IWBAN_LOG_ERROR("Unexpected error while reading value for key"
                                " : '%s' in file '%s'", n.c_str(), filename.c_str());
                break;
            }
            f >> std::ws;
        }
    }
    catch (const sys::FileNotFound & ignored)
    {
        IWBAN_LOG_ERROR("Shader descriptor file not found : '%s'", filename.c_str());
        return shader;
    }

    if (glfs.empty() && glvs.empty())
    {
        IWBAN_LOG_ERROR("No fragment or vertex shader : '%s'", filename.c_str());
        return shader;
    }

    try
    {
        if (!glfs.empty() && glvs.empty())
        {
            // Fragment only
            res::File glfs_file = res::openFile(glfs);
            if (!shader->loadFromMemory(
                    std::string((const char *) glfs_file.getData(), glfs_file.getSize()),
                    sf::Shader::Fragment))
            {
                IWBAN_LOG_ERROR("Failed to load fragment shader : '%s'", filename.c_str());
            }
        }
        else if (glfs.empty() && !glvs.empty())
        {
            // Vertex only
            res::File glvs_file = res::openFile(glfs);
            if (!shader->loadFromMemory(
                    std::string((const char *) glvs_file.getData(), glvs_file.getSize()),
                    sf::Shader::Vertex))
            {
                IWBAN_LOG_ERROR("Failed to load vertex shader : '%s'", filename.c_str());
            }
        }
        else
        {
            // Both are valid
            res::File glfs_file = res::openFile(glfs);
            res::File glvs_file = res::openFile(glfs);
            if (!shader->loadFromMemory(
                    std::string((const char *) glvs_file.getData(), glvs_file.getSize()),
                    std::string((const char *) glfs_file.getData(), glfs_file.getSize())))
            {
                IWBAN_LOG_ERROR("Failed to load combined shaders : '%s'", filename.c_str());
            }
        }
    }
    catch (const sys::FileNotFound & ignored)
    {
        IWBAN_LOG_ERROR("Vertex or fragment shader file not found : '%s'", filename.c_str());
        return shader;
    }

    // Set current texture
    if (!ctex.empty())
        shader->setParameter(ctex, sf::Shader::CurrentTexture);

    return shader;
}
// loadShader()

}
// namespace

namespace data
{

Shader getShader(const std::string & filename)
{
    // Try to find shader
    auto it = ::s_shaderDict.find(filename);
    if (it != ::s_shaderDict.end())
        return it->second.get();

    // Shader not found, load it
    std::unique_ptr<sf::Shader> shader(::loadShader(filename));

    sf::Shader * shaderptr = shader.get();
    ::s_shaderDict[filename] = std::move(shader);
    return shaderptr;
}

}
// namespace data
