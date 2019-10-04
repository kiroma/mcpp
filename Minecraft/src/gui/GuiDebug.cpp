#include "GuiDebug.h"

#include "../render/FontRenderer.h"
#include "../render/RenderMaster.h"
#include "../performance/ChunkStatistics.h"
#include "../entity/Camera.h"
#include "../Minecraft.h"

#include <cstdio>

void GuiDebug::DrawScreen()
{
    const Minecraft &minecraft = Minecraft::GetInstance();
    const Camera &camera = minecraft.GetMasterRenderer().GetViewCamera();
    ChunkStats::Statistics &chunkStats = minecraft.GetChunkStatistics();

    char buffer[65536];
    sprintf(buffer, "Minecraft C++ %d FPS (DT %.4f)", (int) minecraft.GetDebugFPS(),
            Minecraft::GetInstance().GetDeltaTime());
    FontRenderer::DrawString(buffer, glm::ivec2(1, 1));
    sprintf(buffer, "%d chunk updates (C %d D %d L %d R %d)", chunkStats.GetChunkUpdates(ChunkStats::ALL),
            chunkStats.GetChunkUpdates(ChunkStats::CREATE),
            chunkStats.GetChunkUpdates(ChunkStats::DESTROY),
            chunkStats.GetChunkUpdates(ChunkStats::LOAD),
            chunkStats.GetChunkUpdates(ChunkStats::REMESH));
    FontRenderer::DrawString(buffer, glm::ivec2(1, 8 + 1));
    sprintf(buffer, "X %.2f Y %.2f Z %.2f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    FontRenderer::DrawString(buffer, glm::ivec2(1, (2 * 8) + 1));
}

void GuiDebug::MouseDown(sf::Mouse::Button button)
{ (void) button; }
void GuiDebug::MouseUp(sf::Mouse::Button button)
{ (void) button; }
void GuiDebug::KeyDown(sf::Keyboard::Key key)
{ (void) key; }
void GuiDebug::KeyUp(sf::Keyboard::Key key)
{ (void) key; }
