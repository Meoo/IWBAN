/**
 * @file   LanguageSelectionScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LANGUAGESELECTIONSCREEN_HPP_
#define _IWBAN_LANGUAGESELECTIONSCREEN_HPP_

#include <Global.hpp>

#include <config/Settings.hpp>

#include <data/Locale.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Button.hpp>
#include <gui/Label.hpp>
#include <gui/Navigation.hpp>

namespace sys
{

class LanguageSelectionScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Label  _title;

    gui::Button _lang_en, _lang_fr, _lang_it, _lang_de, _lang_es;

    gui::Button _quit;

    Screen * _previous_menu;


public:
    // Constructor
    LanguageSelectionScreen(Screen * previous_menu)
        : _previous_menu(previous_menu)
    {
        // Title
        _title.loadText("options.language");
        _title.setCharacterSize(50);
        _title.setPosition({320, 50});

        // Languages
        _lang_en.loadText("language.en");
        _lang_en.setCallback([this](){ onLanguage("en"); });
        _lang_en.setPosition({140, 140});

        _lang_fr.loadText("language.fr");
        _lang_fr.setCallback([this](){ onLanguage("fr"); });
        _lang_fr.setPosition({140, 180});

        _lang_it.loadText("language.it");
        _lang_it.setCallback([this](){ onLanguage("it"); });
        _lang_it.setPosition({140, 220});


        _lang_de.loadText("language.de");
        _lang_de.setCallback([this](){ onLanguage("de"); });
        _lang_de.setPosition({140, 260});


        _lang_es.loadText("language.es");
        _lang_es.setCallback([this](){ onLanguage("es"); });
        _lang_es.setPosition({140, 300});

        // Quit
        _quit.loadText("options.return");
        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 440});

        // Navigation
        _navi.addVertical(&_lang_en, &_lang_fr);
        _navi.addVertical(&_lang_fr, &_lang_it);
        _navi.addVertical(&_lang_it, &_lang_de);
        _navi.addVertical(&_lang_de, &_lang_es);
        _navi.addVertical(&_lang_es, &_quit);

    }

    // Destructor
    virtual ~LanguageSelectionScreen() {}


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
    {
        // If menu is pressed, return to previous menu
        if (getControls().getAction(ACT_MENU).isJustActivated())
        {
            onQuit();
            return;
        }

        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                _navi.dispatchAction((ActionId) i);
    }

    virtual void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(16, 16, 16));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        _title.draw(gui);

        _lang_en.draw(gui);
        _lang_fr.draw(gui);
        _lang_it.draw(gui);
        _lang_de.draw(gui);
        _lang_es.draw(gui);

        _quit.draw(gui);

        gui.close();
    }

    virtual void onShow()
    {
        if (cfg::language == "fr")
        {
            _navi.setHead(&_lang_fr);
        }
        else if (cfg::language == "it")
        {
            _navi.setHead(&_lang_it);
        }
        else if (cfg::language == "de")
        {
            _navi.setHead(&_lang_de);
        }
        else if (cfg::language == "es")
        {
            _navi.setHead(&_lang_es);
        }
        else
        {
            _navi.setHead(&_lang_en);
        }

        _navi.reset();
    }

    virtual void onMouseMove(const ut::Vector & position)
    {
        _navi.dispatchMouseMove(position);
    }

    virtual void onMouseClick(const ut::Vector & position)
    {
        _navi.dispatchMouseClick(position);
    }

    void onLanguage(const char * language)
    {
        cfg::language = language;
        data::getLocale().reload();
        onQuit();
    }

    void onQuit()
    {
        setNextScreen(_previous_menu);
    }

};
// class LanguageSelectionScreen

}
// namespace sys

#endif // _IWBAN_LANGUAGESELECTIONSCREEN_HPP_
