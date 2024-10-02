#include "IOUtils.h"
#include <fstream>
#include <stdexcept>
#include <iostream>


std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open())
        throw std::runtime_error("failed to open file!");

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    if (!file)
        throw std::runtime_error("failed to read the file!");

    file.close();

    return buffer;
}
