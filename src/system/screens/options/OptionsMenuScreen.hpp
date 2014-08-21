/**
 * @file   OptionsMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OPTIONSMENUSCREEN_HPP_
#define _IWBAN_OPTIONSMENUSCREEN_HPP_

#include <Global.hpp>

#include <config/Settings.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>
#include <system/Time.hpp>
#include <system/screens/options/GamepadOptionsScreen.hpp>
#include <system/screens/options/KeyboardOptionsScreen.hpp>
#include <system/screens/options/LanguageSelectionScreen.hpp>
#include <system/screens/options/VideoOptionsScreen.hpp>

#include <gui/Button.hpp>
#include <gui/Label.hpp>
#include <gui/Navigation.hpp>
#include <gui/Slider.hpp>

namespace sys
{

class OptionsMenuScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Label  _title;

    gui::Button _audio, _language_sub, _language, _video_sub, _keyboard_sub, _gamepad_sub, _quit;

    gui::Label  _gamepad_state;
    sf::Time    _gamepad_state_timer;

    gui::Slider _volume;

    LanguageSelectionScreen _language_selection = LanguageSelectionScreen(this);
    VideoOptionsScreen      _video_options = VideoOptionsScreen(this);
    KeyboardOptionsScreen   _keyboard_options = KeyboardOptionsScreen(this);
    GamepadOptionsScreen    _gamepad_options = GamepadOptionsScreen(this);


public:
    // Constructor
    OptionsMenuScreen()
    {
        // Title
        _title.loadText("menu.options");
        _title.setCharacterSize(50);
        _title.setPosition({320, 50});

        // Language
        _language_sub.loadText("options.language");
        _language_sub.setCallback([this](){ onLanguage(); });
        _language_sub.setPosition({140, 140});

        _language.loadText(std::string("language.") + cfg::language);
        _language.setCallback([this](){ onLanguage(); });
        _language.setPosition({440, 140});
        _language.setCharacterSize(22);

        // Audio
        _audio.loadText("options.audio");
        _audio.setPosition({140, 180});

        _volume.setSize({200, 22});
        _volume.setPosition({440, 180});
        _volume.setRange(0, 100, 10);
        _volume.setValue(cfg::volume);
        _volume.setCallback([](unsigned value){ cfg::volume = value; }); // TODO Update master volume

        // Video
        _video_sub.loadText("options.video");
        _video_sub.setCallback([this](){ onVideo(); });
        _video_sub.setPosition({140, 220});

        // Keyboard
        _keyboard_sub.loadText("options.keyboard");
        _keyboard_sub.setCallback([this](){ onKeyboard(); });
        _keyboard_sub.setPosition({140, 260});

        // Gamepad
        _gamepad_sub.loadText("options.gamepad");
        _gamepad_sub.setCallback([this](){ onGamepad(); });
        _gamepad_sub.setPosition({140, 300});

        _gamepad_state.setPosition({440, 300});
        _gamepad_state.setCharacterSize(22);

        // Quit
        _quit.loadText("options.return");
        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 440});

        // Navigation
        _navi.setHead(&_language_sub);

        _navi.addVertical(&_language_sub, &_audio);
        _navi.addVertical(&_audio, &_video_sub);
        _navi.addVertical(&_video_sub, &_keyboard_sub);
        _navi.addVertical(&_keyboard_sub, &_gamepad_sub);
        _navi.addVertical(&_gamepad_sub, &_quit);

        _navi.addSlave(&_audio, &_volume);
        _navi.addSlave(&_language_sub, &_language);
    }

    // Destructor
    virtual ~OptionsMenuScreen() {}


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
    {
        if (_gamepad_state_timer < update_time)
        {
            bool connected = sf::Joystick::isConnected(0);
            if (connected && !_gamepad_sub.isEnabled())
            {
                _gamepad_state.loadText("gamepad.connected");
                _gamepad_state.setTextColor(sf::Color(64, 128, 64));
                _gamepad_sub.enable();
            }
            else if (!connected && _gamepad_sub.isEnabled())
            {
                _gamepad_state.loadText("gamepad.disconnected");
                _gamepad_state.setTextColor(sf::Color(128, 64, 64));
                _gamepad_sub.disable();
            }

            _gamepad_state_timer = update_time + sf::seconds(2);
        }

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

        _language_sub.draw(gui);
        _audio.draw(gui);
        _video_sub.draw(gui);
        _keyboard_sub.draw(gui);
        _gamepad_sub.draw(gui);
        _quit.draw(gui);

        _language.draw(gui);
        _volume.draw(gui);
        _gamepad_state.draw(gui);

        gui.close();
    }

    virtual void onMouseMove(const ut::Vector & position)
    {
        _navi.dispatchMouseMove(position);
    }

    virtual void onMouseClick(const ut::Vector & position)
    {
        _navi.dispatchMouseClick(position);
    }

    void onLanguage()
    {
        setNextScreen(&_language_selection);
    }

    void onVideo()
    {
        setNextScreen(&_video_options);
    }

    void onKeyboard()
    {
        setNextScreen(&_keyboard_options);
    }

    void onGamepad()
    {
        setNextScreen(&_gamepad_options);
    }

    void onQuit()
    {
        // TODO Quit
        _quit.setTextColor(sf::Color::Red);
    }

};
// class OptionsMenuScreen

}
// namespace sys

#endif // _IWBAN_OPTIONSMENUSCREEN_HPP_
