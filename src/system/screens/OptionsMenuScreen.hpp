/**
 * @file   OptionsMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OPTIONSMENUSCREEN_HPP_
#define _IWBAN_OPTIONSMENUSCREEN_HPP_

#include <Global.hpp>

#include <config/GamepadConfig.hpp>
#include <config/KeyboardConfig.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>
#include <system/Time.hpp>
#include <system/inputs/Gamepad.hpp>
#include <system/inputs/Keyboard.hpp>

#include <gui/ButtonLabel.hpp>
#include <gui/Choice.hpp>
#include <gui/Frame.hpp>
#include <gui/KeyLabel.hpp>
#include <gui/Line.hpp>
#include <gui/Menu.hpp>
#include <gui/Separator.hpp>
#include <gui/Slider.hpp>

#include <resources/Locale.hpp>

namespace sys
{

class OptionsMenuScreen : public Screen
{
private:
    // Data members
    gui::Menu * _menu;
    gui::Menu * _video_menu;
    gui::Menu * _keyboard_menu;
    gui::Menu * _gamepad_menu;

    sf::Time     _gamepad_state_timer;
    gui::Label * _gamepad_state;

    gui::KeyLabel * _key_up;
    gui::KeyLabel * _key_down;
    gui::KeyLabel * _key_left;
    gui::KeyLabel * _key_right;
    gui::KeyLabel * _key_jump;
    gui::KeyLabel * _key_fire;
    gui::KeyLabel * _key_retry;

    // Slider for axis threshold (sensibility)
    // Value is between 5 and 95, with a step of 9
    // Since the label says "sensibility", the threshold is the invert, 100 - value
    gui::Slider      * _thresh;
    gui::ButtonLabel * _btn_jump;
    gui::ButtonLabel * _btn_fire;
    gui::ButtonLabel * _btn_retry;

    // Block controls if waiting for an input
    bool            _waiting_input = false;

    enum State
    {
        ST_OPTIONS,
        ST_VIDEO,
        ST_KEYBOARD,
        ST_GAMEPAD

    } _state = ST_OPTIONS;


public:
    // Constructor
    OptionsMenuScreen()
    {
        const ut::Vector choice_box(300, 40);

        res::getLocale().loadFile("system/gamepad.txt");
        res::getLocale().loadFile("system/keyboard.txt");


        // ################################################################
        // Options menu
        // ################################################################

        _menu = new gui::Menu();
        _menu->setPosition(ut::Vector(0, 20));
        _menu->setSize(ut::Vector(640, 400));
        _menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("menu.options");
            title->setCharacterSize(40);
            _menu->add(new gui::Frame(ut::Vector(400, 70), title));
            _menu->add(new gui::Separator(ut::Vector(400, 10)));
        }

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("options.language")),
            new gui::Frame(ut::Vector(400, 40), new gui::Choice("language"))
        }));

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("options.audio")),
            new gui::Frame(ut::Vector(400, 40),
                        new gui::Slider(ut::Vector(250, 22), 0, 100, 10, 50))
        }));

        {
            gui::Choice * video = new gui::Choice("options.video");
            video->setAction([this](){ setState(ST_VIDEO); });
            _menu->add(new gui::Line({
                new gui::Frame(ut::Vector(200, 40), video),
                new gui::Separator(ut::Vector(400, 40))
            }));
        }

        {
            gui::Choice * keyboard = new gui::Choice("options.keyboard");
            keyboard->setAction([this](){ setState(ST_KEYBOARD); });
            _menu->add(new gui::Line({
                new gui::Frame(ut::Vector(200, 40), keyboard),
                new gui::Separator(ut::Vector(400, 40))
            }));
        }

        {
            gui::Choice * gamepad = new gui::Choice("options.gamepad");
            gamepad->setAction([this](){ setState(ST_GAMEPAD); });
            _gamepad_state = new gui::Label();
            _gamepad_state->setCharacterSize(22);
            _menu->add(new gui::Line({
                new gui::Frame(ut::Vector(200, 40), gamepad),
                new gui::Frame(ut::Vector(200, 40), _gamepad_state),
                new gui::Separator(ut::Vector(200, 40))
            }));
        }

        _menu->add(new gui::Separator(ut::Vector(400, 40)));
        _menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("options.save")));
        _menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("options.cancel")));


        // ################################################################
        // Video menu
        // ################################################################

        _video_menu = new gui::Menu();
        _video_menu->setPosition(ut::Vector(0, 20));
        _video_menu->setSize(ut::Vector(640, 400));
        _video_menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("options.video");
            title->setCharacterSize(40);
            _video_menu->add(new gui::Frame(ut::Vector(400, 70), title));
        }

        /* TODO _mode = new gui::Selector();
        _mode->addEntry(0, "video.windowed");
        _mode->addEntry(1, "video.fullscreen");
        _mode->addEntry(2, "video.borderless");
        _mode->setAction([this](int mode){ cfg::fullscreen = mode; });
        _video_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("video.mode")),
            new gui::Frame(ut::Vector(400, 40), _mode)
        }));*/

        {
            gui::Choice * reset = new gui::Choice("options.defaults");
            reset->setAction([this](){ /* TODO Video defaults */ });
            _video_menu->add(new gui::Separator(ut::Vector(400, 10)));
            _video_menu->add(new gui::Frame(ut::Vector(400, 40), reset));
        }

        {
            gui::Choice * cancel = new gui::Choice("options.return");
            cancel->setAction([this](){ setState(ST_OPTIONS); });
            _video_menu->add(new gui::Frame(ut::Vector(400, 40), cancel));
        }


        // ################################################################
        // Keyboard menu
        // ################################################################

        _keyboard_menu = new gui::Menu();
        _keyboard_menu->setPosition(ut::Vector(0, 20));
        _keyboard_menu->setSize(ut::Vector(640, 400));
        _keyboard_menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("options.keyboard");
            title->setCharacterSize(40);
            _keyboard_menu->add(new gui::Frame(ut::Vector(400, 70), title));
        }

        _key_up = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_UP));
        _key_up->setAction([this](){ hookKeyboard(_key_up, ACT_UP); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.up")),
            new gui::Frame(ut::Vector(400, 40), _key_up)
        }));

        _key_left = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_LEFT));
        _key_left->setAction([this](){ hookKeyboard(_key_left, ACT_LEFT); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.left")),
            new gui::Frame(ut::Vector(400, 40), _key_left)
        }));

        _key_down = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_DOWN));
        _key_down->setAction([this](){ hookKeyboard(_key_down, ACT_DOWN); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.down")),
            new gui::Frame(ut::Vector(400, 40), _key_down)
        }));

        _key_right = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_RIGHT));
        _key_right->setAction([this](){ hookKeyboard(_key_right, ACT_RIGHT); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.right")),
            new gui::Frame(ut::Vector(400, 40), _key_right)
        }));

        _key_jump = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_JUMP));
        _key_jump->setAction([this](){ hookKeyboard(_key_jump, ACT_JUMP); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.jump")),
            new gui::Frame(ut::Vector(400, 40), _key_jump)
        }));

        _key_fire = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_FIRE));
        _key_fire->setAction([this](){ hookKeyboard(_key_fire, ACT_FIRE); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.fire")),
            new gui::Frame(ut::Vector(400, 40), _key_fire)
        }));

        _key_retry = new gui::KeyLabel(getKeyboard().getKeyFromAction(ACT_RETRY));
        _key_retry->setAction([this](){ hookKeyboard(_key_retry, ACT_RETRY); });
        _keyboard_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.retry")),
            new gui::Frame(ut::Vector(400, 40), _key_retry)
        }));

        {
            gui::Choice * reset = new gui::Choice("options.defaults");
            reset->setAction([this](){ getKeyboard().reloadDefaults(); updateKeyboardKeys(); });
            _keyboard_menu->add(new gui::Separator(ut::Vector(400, 10)));
            _keyboard_menu->add(new gui::Frame(ut::Vector(400, 40), reset));
        }

        gui::Choice * kcancel = new gui::Choice("options.return");
        kcancel->setAction([this](){ setState(ST_OPTIONS); });
        _keyboard_menu->add(new gui::Frame(ut::Vector(400, 40), kcancel));


        // ################################################################
        // Gamepad menu
        // ################################################################

        _gamepad_menu = new gui::Menu();
        _gamepad_menu->setPosition(ut::Vector(0, 20));
        _gamepad_menu->setSize(ut::Vector(640, 400));
        _gamepad_menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("options.gamepad");
            title->setCharacterSize(40);
            _gamepad_menu->add(new gui::Frame(ut::Vector(400, 70), title));
        }

        _thresh = new gui::Slider(ut::Vector(250, 22), 5, 95, 9,
                                    100 - getGamepad().getAxisThreshold());
        _thresh->setAction([this](unsigned value){ updateThresh(value); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("gamepad.sensibility")),
            new gui::Frame(ut::Vector(400, 40), _thresh)
        }));

        _btn_jump = new gui::ButtonLabel(getGamepad().getButtonFromAction(ACT_JUMP));
        _btn_jump->setAction([this](){ hookGamepad(_btn_jump, ACT_JUMP); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.jump")),
            new gui::Frame(ut::Vector(400, 40), _btn_jump)
        }));

        _btn_fire = new gui::ButtonLabel(getGamepad().getButtonFromAction(ACT_FIRE));
        _btn_fire->setAction([this](){ hookGamepad(_btn_fire, ACT_FIRE); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.fire")),
            new gui::Frame(ut::Vector(400, 40), _btn_fire)
        }));

        _btn_retry = new gui::ButtonLabel(getGamepad().getButtonFromAction(ACT_RETRY));
        _btn_retry->setAction([this](){ hookGamepad(_btn_retry, ACT_RETRY); });
        _gamepad_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("actions.retry")),
            new gui::Frame(ut::Vector(400, 40), _btn_retry)
        }));

        {
            gui::Choice * reset = new gui::Choice("options.defaults");
            reset->setAction([this](){ getGamepad().reloadDefaults(); updateGamepadButtons(); });
            _gamepad_menu->add(new gui::Separator(ut::Vector(400, 10)));
            _gamepad_menu->add(new gui::Frame(ut::Vector(400, 40), reset));
        }


        gui::Choice * gcancel = new gui::Choice("options.return");
        gcancel->setAction([this](){ setState(ST_OPTIONS); });
        _gamepad_menu->add(new gui::Frame(ut::Vector(400, 40), gcancel));

    }

    // Destructor
    virtual ~OptionsMenuScreen()
    {
        // Choices are released by _menu
        delete _menu;
        delete _keyboard_menu;
        delete _gamepad_menu;
    }


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
    {
        if (_state == ST_OPTIONS && _gamepad_state_timer < update_time)
        {
            if (sf::Joystick::isConnected(0))
            {
                _gamepad_state->setText("gamepad.connected");
                _gamepad_state->setTextColor(sf::Color(64, 128, 64));
            }
            else
            {
                _gamepad_state->setText("gamepad.disconnected");
                _gamepad_state->setTextColor(sf::Color(128, 64, 64));
            }

            _gamepad_state_timer = update_time + sf::seconds(2);
            _menu->updateChilds();
        }

        if (!_waiting_input)
        {
            // If menu is pressed, return to previous menu
            if (getControls().getAction(ACT_MENU).isJustActivated())
            {
                switch (_state)
                {
                case ST_OPTIONS:
                    // TODO Go back to main menu
                    break;

                case ST_VIDEO:
                    setState(ST_OPTIONS);
                    break;

                case ST_KEYBOARD:
                    setState(ST_OPTIONS);
                    break;

                case ST_GAMEPAD:
                    setState(ST_OPTIONS);
                    break;
                }
                return;
            }

            // Send actions to active menu
            for (unsigned i = 0; i < ACT_COUNT; ++i)
            {
                if (getControls().getAction((ActionId) i).isJustActivated())
                {
                    switch (_state)
                    {
                    case ST_OPTIONS:
                        _menu->dispatchAction((ActionId) i);
                        break;

                    case ST_VIDEO:
                        _video_menu->dispatchAction((ActionId) i);
                        break;

                    case ST_KEYBOARD:
                        _keyboard_menu->dispatchAction((ActionId) i);
                        break;

                    case ST_GAMEPAD:
                        _gamepad_menu->dispatchAction((ActionId) i);
                        break;
                    }
                }
            }
        } // if (!_waiting_input)
        else
        {
            // If we catch an action while a hook is active, it means
            // another device sent it.
            if (getControls().getAction(ACT_MENU).isJustActivated())
            {
                if (_state == ST_GAMEPAD)
                {
                    // If we are waiting for a gamepad input and the user presses escape
                    // we need to handle it properly
                    getGamepad().catchNextButton(nullptr);
                    _waiting_input = false;
                    _btn_jump->updateColor();
                    _btn_fire->updateColor();
                    _btn_retry->updateColor();
                    updateGamepadButtons();
                }
                else if (_state == ST_KEYBOARD)
                {
                    // Same thing if we are waiting for keyboard input and the user presses
                    // start on his gamepad
                    getKeyboard().catchNextKey(nullptr);
                    _waiting_input = false;
                    _key_up->updateColor();
                    _key_down->updateColor();
                    _key_left->updateColor();
                    _key_right->updateColor();
                    _key_jump->updateColor();
                    _key_fire->updateColor();
                    _key_retry->updateColor();
                    updateKeyboardKeys();
                }
            }
        } // else if (!_waiting_input)
    }

    virtual void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(16, 16, 16));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        switch (_state)
        {
        case ST_OPTIONS:
            _menu->draw(gui);
            break;

        case ST_VIDEO:
            _video_menu->draw(gui);
            break;

        case ST_KEYBOARD:
            _keyboard_menu->draw(gui);
            break;

        case ST_GAMEPAD:
            _gamepad_menu->draw(gui);
            break;
        }

        gui.close();
    }

    virtual void onShow()
    {
    }

    virtual void onHide()
    {
    }

    void setState(State state)
    {
        _state = state;
    }

    void hookKeyboard(gui::KeyLabel * key_label, ActionId action)
    {
        _waiting_input = true;
        key_label->setText("keyboard.presskey");
        key_label->setTextColor(sf::Color::Red);
        getKeyboard().catchNextKey([this, key_label, action](sf::Keyboard::Key key)
            {
                // Map a key, or unmap if Menu is pressed
                if (key == IWBAN_KEYBOARD_MENU)
                {
                    sf::Keyboard::Key prev_key = getKeyboard().getKeyFromAction(action);
                    if (prev_key != sf::Keyboard::KeyCount)
                        getKeyboard().unmapKey(prev_key);
                }
                else
                    getKeyboard().mapKeyToAction(key, action);

                // Update button text
                key_label->setTextColor(sf::Color::White);
                updateKeyboardKeys();

                // Reset controls, just in case
                getControls().reset();
                _waiting_input = false;
            });
    }

    void updateKeyboardKeys()
    {
        _key_up->setKey(getKeyboard().getKeyFromAction(ACT_UP));
        _key_down->setKey(getKeyboard().getKeyFromAction(ACT_DOWN));
        _key_left->setKey(getKeyboard().getKeyFromAction(ACT_LEFT));
        _key_right->setKey(getKeyboard().getKeyFromAction(ACT_RIGHT));
        _key_jump->setKey(getKeyboard().getKeyFromAction(ACT_JUMP));
        _key_fire->setKey(getKeyboard().getKeyFromAction(ACT_FIRE));
        _key_retry->setKey(getKeyboard().getKeyFromAction(ACT_RETRY));

        _keyboard_menu->updateChilds();
    }

    void updateThresh(unsigned value)
    {
        getGamepad().setAxisThreshold(100 - value);
    }

    void hookGamepad(gui::ButtonLabel * button_label, ActionId action)
    {
        _waiting_input = true;
        button_label->setText("gamepad.pressbutton");
        button_label->setTextColor(sf::Color::Red);
        getGamepad().catchNextButton([this, button_label, action](Gamepad::Button button)
            {
                // Map a button
                getGamepad().mapButtonToAction(button, action);

                // Update button text
                button_label->setTextColor(sf::Color::White);
                updateGamepadButtons();

                // Reset controls, just in case
                getControls().reset();
                _waiting_input = false;
            });
    }

    void updateGamepadButtons()
    {
        _thresh->setValue(100 - getGamepad().getAxisThreshold());
        _btn_jump->setButton(getGamepad().getButtonFromAction(ACT_JUMP));
        _btn_fire->setButton(getGamepad().getButtonFromAction(ACT_FIRE));
        _btn_retry->setButton(getGamepad().getButtonFromAction(ACT_RETRY));

        _gamepad_menu->updateChilds();
    }

};
// class OptionsMenuScreen

}
// namespace sys

#endif // _IWBAN_OPTIONSMENUSCREEN_HPP_
