#ifndef MINECRAFT_GUIDEBUG_H
#define MINECRAFT_GUIDEBUG_H

#include "GuiScreen.h"

class GuiDebug : public GuiScreen
{
public:
    void DrawScreen() override;
    void MouseDown(sf::Event::MouseButtonEvent button) override;
    void MouseUp(sf::Event::MouseButtonEvent button) override;
    void KeyDown(sf::Event::KeyEvent key) override;
    void KeyUp(sf::Event::KeyEvent key) override;

private:
    bool extended = false;
};

#endif //MINECRAFT_GUIDEBUG_H
