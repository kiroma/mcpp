cmake_minimum_required(VERSION 3.14)
project(Minecraft)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(GLEW_STATIC)
add_compile_definitions(MINECRAFT_DEBUG)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -pedantic")

# TODO:
#   Implement proper linux CMakeLists.txt
#

if (WIN32)
    include_directories(../Common/lib/glew-2.1.0/include)
    include_directories(../Common/lib/glm/glm)
    include_directories(../Common/lib/sfml/include)
    include_directories(../Common/lib/stb)
    include_directories(../Common/lib/noise)

    link_directories(../Common/lib/glew-2.1.0/lib/Release/Win32)
    link_directories(../Common/lib/sfml/lib)

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        link_libraries(mingw32 glew32s opengl32 openal32 sfml-graphics-d sfml-window-d sfml-system-d sfml-audio-d)
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        link_libraries(mingw32 glew32s opengl32 openal32 sfml-graphics sfml-window sfml-system sfml-audio)
    endif ()
endif (WIN32)

add_executable(Minecraft src/main.cpp src/Minecraft.cpp src/Minecraft.h src/render/RenderMaster.cpp src/render/RenderMaster.h src/world/World.cpp src/world/World.h src/world/Chunk.cpp src/world/Chunk.h src/render/shaders/ShaderProgram.cpp src/render/shaders/ShaderProgram.h src/utils/InputOutput.cpp src/utils/InputOutput.h src/entity/Camera.cpp src/entity/Camera.h src/world/BlockFace.h src/utils/Vectors.h src/world/Block.cpp src/world/Block.h src/render/Texture.cpp src/render/Texture.h src/utils/math/DirectionVectors.cpp src/utils/math/DirectionVectors.h src/render/TextureAtlas.cpp src/render/TextureAtlas.h src/world/FullChunk.cpp src/world/FullChunk.h src/entity/Entity.cpp src/entity/Entity.h src/world/gen/noise/SimplexNoise.cpp src/world/gen/noise/SimplexNoise.h src/render/text/FontRenderer.cpp src/render/text/FontRenderer.h src/performance/ChunkStatistics.cpp src/performance/ChunkStatistics.h src/gui/Gui.h src/gui/ScaledResolution.cpp src/gui/ScaledResolution.h src/gui/GuiScreen.h src/gui/GuiDebug.cpp src/gui/GuiDebug.h src/world/gen/IChunkGenerator.h src/world/gen/OverworldGenerator.cpp src/world/gen/OverworldGenerator.h src/world/gen/biome/IBiome.h src/world/gen/biome/BiomePlains.cpp src/world/gen/biome/BiomePlains.h src/world/gen/biome/BiomeExtremeHills.cpp src/world/gen/biome/BiomeExtremeHills.h src/utils/math/MathHelper.cpp src/utils/math/MathHelper.h src/render/shaders/Shader.cpp src/render/shaders/Shader.h src/render/shaders/ShaderType.h src/utils/exceptions/ReportedException.h src/render/text/TextRenderer.cpp src/render/text/TextRenderer.h)
