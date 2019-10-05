#include "InputOutput.h"
#include "exceptions/ReportedException.h"

#include <iostream>
#include <cstring>
#include <cstdio>

std::vector<char> InputOutput::ReadFile(std::string filename)
{
    if (!FileExists(filename)) {
        throw ReportedException(filename, "Couldn't read file because it doesn't exist.",
                                    "There are no more details.");
    }

    // Open file for reading in text mode
    FILE *file = fopen(filename.c_str(), "rb");

    if (file == NULL) {
        throw ReportedException(filename, "Unable to read file for unknown reasons.",
                                    "Make sure that this process hsa access to the file you're trying to read.");
    }

    // Get file length
    fseek(file, 0, SEEK_END);
    unsigned long length = ftell(file) + 1;
    std::vector<char> data(length);
    std::fill(data.begin(), data.end(), 0);

    // Read file
    fseek(file, 0, SEEK_SET);
    fread(data.data(), 1, length, file);
    fclose(file);

    // Return the result
    return data;
}

bool InputOutput::FileExists(std::string filename)
{
    FILE *file;
    if ((file = fopen(filename.c_str(), "r"))) {
        fclose(file);
        return true;
    }

    return false;
}
