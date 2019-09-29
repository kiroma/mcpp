#ifndef MINECRAFT_MINECRAFT_H
#define MINECRAFT_MINECRAFT_H

#include <vec2.hpp>
#include <gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

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
    const RenderMaster &GetMasterRenderer() const
    { return *masterRenderer; }
    const World &GetCurrentWorld() const
    { return *world; }
    float GetDeltaTime() const
    { return deltaTime; }
    const sf::RenderWindow& GetWindow() const
    { return *window; }
    float GetDebugFPS() const
    { return 1.0f / deltaTime; }

private:
    int StartGame();

    bool running;
    bool input[4096];
    bool gameFocus;
    glm::vec2 mousePosition;
    float deltaTime;

    RenderMaster *masterRenderer;
    World *world;

    sf::RenderWindow *window;
};

#endif //MINECRAFT_MINECRAFT_H
