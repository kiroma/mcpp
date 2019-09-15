#include "FileUtils.h"

#include <iostream>
#include <mem.h>

namespace FileUtils
{
    std::string ReadFile(const char *filename)
    {
        if (!FileExists(filename))
            return std::string("Unable to load file.");

        // Open file for reading in text mode
        FILE *file = fopen(filename, "rt");

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

        // Prepare result and free memory
        std::string result(data);
        delete[] data;

        // Return the result
        return result;
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
