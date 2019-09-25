#ifndef MINECRAFT_MINECRAFT_H
#define MINECRAFT_MINECRAFT_H

#include <SFML/Graphics.hpp>
#include <ext.hpp>

class RenderMaster;

class World;

class Minecraft
{
public:
    Minecraft();
    ~Minecraft();

    int Run();
    void Shutdown();

    void Input(int vkey, bool state);
    bool GetInput(int vkey) const;
    glm::vec2 GetMousePosition() const;
    bool HasFocus()
    { return gameFocus; }

    void UpdateProjection() const;

    // Getters
    static Minecraft &GetInstance();
    bool IsRunning() const volatile
    { return running; }
    const RenderMaster& GetMasterRenderer() const
    { return *masterRenderer; }
    const World& GetCurrentWorld() const
    { return *world; }

private:
    int StartGame();

private:
    bool running;
    bool input[4096];
    glm::vec2 mousePosition;
    bool gameFocus;

    RenderMaster *masterRenderer;
    World *world;

    sf::RenderWindow *window;
};

#endif //MINECRAFT_MINECRAFT_H
