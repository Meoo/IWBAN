/**
 * @file   Scene.hpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/Drawable.hpp>
#include <graphics/Light.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/Scene.hpp>
#include <graphics/ShadowVolume.hpp>

#include <list>

namespace gfx
{

void Scene::add(Drawable * drawable)
{
    IWBAN_PRE_PTR(drawable);

    drawable->init(this);

    _drawables.push_back(drawable);
}

void Scene::remove(Drawable * drawable) noexcept
{
    IWBAN_PRE_PTR(drawable);
    IWBAN_PRE(drawable->getScene() == this);

    drawable->deinit();

    for (auto it = _drawables.begin(); it != _drawables.end(); ++it)
    {
        if (*it == drawable)
        {
            _drawables.erase(it);
            return;
        }
    }

    IWBAN_LOG_ERROR("Could not remove Drawable from Scene");
}

void Scene::add(Light * light)
{
    IWBAN_PRE_PTR(light);

    light->init(this);

    _lights.push_back(light);
}

void Scene::remove(Light * light) noexcept
{
    IWBAN_PRE_PTR(light);
    IWBAN_PRE(light->getScene() == this);

    light->deinit();

    for (auto it = _lights.begin(); it != _lights.end(); ++it)
    {
        if (*it == light)
        {
            _lights.erase(it);
            return;
        }
    }

    IWBAN_LOG_ERROR("Could not remove Light from Scene");
}

void Scene::add(ShadowVolume * shadow)
{
    IWBAN_PRE_PTR(shadow);

    shadow->init(this);

    _shadows.push_back(shadow);
}

void Scene::remove(ShadowVolume * shadow) noexcept
{
    IWBAN_PRE_PTR(shadow);
    IWBAN_PRE(shadow->getScene() == this);

    shadow->deinit();

    for (auto it = _shadows.begin(); it != _shadows.end(); ++it)
    {
        if (*it == shadow)
        {
            _shadows.erase(it);
            return;
        }
    }

    IWBAN_LOG_ERROR("Could not remove ShadowVolume from Scene");
}

void Scene::draw(Renderer & renderer) const
{
    ut::Rectangle bounds = renderer.getCamera().getViewBounds();

    if (!_lights.empty())
    {
        // TODO Change ambient light, use shadows
        LightContext & light = renderer.openLightContext(sf::Color::Black);

        light.setShadowVolumeList(&_shadows);

        for (Light * it : _lights)
        {
            if (bounds.isIntersecting(it->getBounds()))
                light.draw(*it);
        }

        light.close();
    }

    if (!_drawables.empty())
    {
        DrawContext & draw = renderer.openDrawContext();

        for (Drawable * it : _drawables)
        {
            if (bounds.isIntersecting(it->getBounds()))
                draw.draw(*it);
        }

        draw.close();
    }
}

}
// namespace gfx
