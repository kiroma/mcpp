#ifndef MINECRAFT_GUISCREEN_H
#define MINECRAFT_GUISCREEN_H

#include "Gui.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

class GuiScreen : public Gui
{
public:
    // Virtual destructor stub
    virtual ~GuiScreen()
    {}

    // Update
    virtual void DrawScreen() = 0;

    // Events
    virtual void MouseDown(sf::Mouse::Button button) = 0;
    virtual void MouseUp(sf::Mouse::Button button) = 0;
    virtual void KeyDown(sf::Keyboard::Key button) = 0;
    virtual void KeyUp(sf::Keyboard::Key button) = 0;
};

#endif //MINECRAFT_GUISCREEN_H
