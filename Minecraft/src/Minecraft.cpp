#include "Minecraft.h"

#include "render/RenderMaster.h"
#include "world/World.h"
#include "world/Block.h"

#include <GL/glew.h>
#include <iostream>
#include <mem.h>

Minecraft *instance = nullptr;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam)
{
    //std::cout << "OpenGL debug callback: \"" << message << "\"" << std::endl;
}

Minecraft::Minecraft()
        : running(false), masterRenderer(nullptr), world(nullptr)
{
    instance = this;
    memset(input, 0, sizeof(input));
}

Minecraft::~Minecraft()
{
    instance = nullptr;

    delete masterRenderer;
    delete world;
}

// --------------------------------------------------------------
//  Run game:
//      Calls StartGame() and enters a loop rendering
//      and waiting for events
// --------------------------------------------------------------
int Minecraft::Run()
{
    // Start game
    running = true;
    gameFocus = true;
    int i = StartGame();
    if (!i) return i;

    // Game loop
    sf::Clock clock;
    while (running) {
        // Event handling
        sf::Event event;
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    Shutdown();
                    return false;
                case sf::Event::Resized:
                    UpdateProjection();
                    std::cout << "Window resized to " << (float) event.size.width << " * " << (float) event.size.height
                              << std::endl;
                    break;
                case sf::Event::KeyPressed:
                    Input(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    Input(event.key.code, false);
                    break;
                case sf::Event::MouseButtonPressed:
                    gameFocus = true;
                    break;
                case sf::Event::LostFocus:
                    gameFocus = false;
                    break;
                default:
                    break;
            }
        }

        // Record the time the last tick took
        deltaTime = clock.restart().asSeconds();

        // Player input
        auto mousePos = glm::vec2((float) sf::Mouse::getPosition().x, (float) sf::Mouse::getPosition().y);

        if (GetInput(sf::Keyboard::Key::Escape)) {
            gameFocus = false;
        }

        if (gameFocus) {
            window->setMouseCursorVisible(false);
            sf::Mouse::setPosition(sf::Vector2i(window->getPosition().x + window->getSize().x / 2,
                                                window->getPosition().y + window->getSize().y / 2));
        } else {
            window->setMouseCursorVisible(true);
        }

        auto mouseNew = glm::vec2((float) sf::Mouse::getPosition().x, (float) sf::Mouse::getPosition().y);
        mousePosition += mousePos - mouseNew;

        // Game logic
        world->Tick();

        // :D
        masterRenderer->RenderWorld(*world);
        window->display();
    }

    return 0;
}

void Minecraft::Shutdown()
{
    std::cout << "Shutting down..." << std::endl;
    running = false;
}

void Minecraft::Input(int vkey, bool state)
{
    if (input[vkey] != state) input[vkey] = state;
}

bool Minecraft::GetInput(int vkey) const
{
    return input[vkey];
}

glm::vec2 Minecraft::GetMousePosition() const
{
    return mousePosition;
}

void Minecraft::UpdateProjection() const
{
    masterRenderer->SubmitProjection(
            glm::perspective(glm::radians(80.0f), ((float) window->getSize().x) / ((float) window->getSize().y), 0.01f,
                             1000.0f));
    glViewport(0, 0, (float) window->getSize().x, (float) window->getSize().y);
}

// --------------------------------------------------------------
//  Initialize game window, pixel format,
//  OpenGL and modern OpenGL
// --------------------------------------------------------------
int Minecraft::StartGame()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    window = new sf::RenderWindow;
    window->create(sf::VideoMode(1440, 720), std::move("Minecraft C++"),
                   sf::Style::Default, settings);   // 2:1 aspect ratio
    window->setFramerateLimit(120);

    GLenum error = glewInit();
    if (error != GLEW_NO_ERROR) {
        std::cerr << "Unable to initialize GLEW: " << error << " \""
                  << std::string((const char *) glewGetErrorString(error)) << "\".";
        return error;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << " " << glGetString(GL_RENDERER) << " "
              << glGetString(GL_VENDOR) << " GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

#ifdef MINECRAFT_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
#endif

    // Welcome message
    std::cout << "==============================================" << std::endl;
    std::cout << "                MINECRAFT C++                 " << std::endl;
    std::cout << "      https://github.com/jlagarespo/mcpp      " << std::endl;
    std::cout << "==============================================" << std::endl;

    // Initialize stuff
    masterRenderer = new RenderMaster;
    Block::Database::Initialize();
    world = new World;

    // Spawn the camera
    masterRenderer->LoadCamera(new Camera(glm::vec3(0, world->GetHighestPoint(glm::ivec2(0, 0)) + 2, 0)));

    // Make sure we are in a consistent state
    UpdateProjection();
}

// --------------------------------------------------------------
//  Get a static reference to the running game
// --------------------------------------------------------------
Minecraft &Minecraft::GetInstance()
{
    return *instance;
}
