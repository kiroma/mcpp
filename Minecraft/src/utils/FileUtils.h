#ifndef MINECRAFT_FILEUTILS_H
#define MINECRAFT_FILEUTILS_H

#include <string>

namespace FileUtils
{
    const char *ReadFile(const char *filename);
    bool FileExists(const char *filename);
}

#endif //MINECRAFT_FILEUTILS_H
