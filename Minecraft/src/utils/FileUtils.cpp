#include "FileUtils.h"

#include <iostream>
#include <mem.h>

namespace FileUtils
{
    const char* ReadFile(const char *filename)
    {
        if (!FileExists(filename)) {
            std::cerr << "Unable to read \"" << filename << "\". File doesn't exist.";
            return nullptr;
        }

        // Open file for reading in text mode
        FILE *file = fopen(filename, "rb");

        if (file == NULL) {
            std::cerr << "Unable to read \"" << filename << "\".";
            return nullptr;
        }

        // Get file length
        fseek(file, 0, SEEK_END);
        unsigned long length = ftell(file);
        char *data = new char[length + 1];

        // Read file
        memset(data, 0, length + 1);
        fseek(file, 0, SEEK_SET);
        fread(data, 1, length, file);
        fclose(file);

        // Return the result
        return data;
    }

    bool FileExists(const char *filename)
    {
        FILE *file;
        if ((file = fopen(filename, "r"))) {
            fclose(file);
            return true;
        }

        return false;
    }
}
