cmake_minimum_required(VERSION 3.14)
project(Minecraft)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(GLEW_STATIC)
add_compile_definitions(MINECRAFT_DEBUG)

if (LINUX)
    # NOT TESTED: If you are compiling for linux, then you'll probably have to
    # manualy add compiler arguments for linking with other libraries.
    include_directories(/usr/include/glm)
endif (LINUX)

if (WIN32)
    include_directories(../Common/lib/glew-2.1.0/include)
    include_directories(../Common/lib/glm/glm)
    include_directories(../Common/lib/sfml/include)
    include_directories(../Common/lib/stb)

    link_directories(../Common/lib/glew-2.1.0/lib/Release/Win32)
    link_directories(../Common/lib/sfml/lib/)
    link_libraries(mingw32)
endif (WIN32)

link_libraries(glew32s opengl32 openal32 sfml-graphics sfml-window sfml-system)

add_executable(Minecraft src/main.cpp src/Minecraft.cpp src/Minecraft.h src/render/RenderMaster.cpp src/render/RenderMaster.h src/world/World.cpp src/world/World.h src/world/Chunk.cpp src/world/Chunk.h src/render/Shader.cpp src/render/Shader.h src/utils/FileUtils.cpp src/utils/FileUtils.h src/render/Camera.cpp src/render/Camera.h src/world/BlockFace.h src/utils/Vectors.h src/world/Block.cpp src/world/Block.h src/render/Texture.cpp src/render/Texture.h src/utils/TextureCoords.cpp src/utils/TextureCoords.h src/utils/DirectionVectors.cpp src/utils/DirectionVectors.h)
