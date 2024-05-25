#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <iostream>
#include <vector>
#include <string>
#include <filesystem> // C++ 17 !

class FileSystem
{
public:
    FileSystem();
    std::vector<std::string> getDirectoryContents(const std::string& directory);
    std::string getCurrentDirectory();
    std::string getSoundsDirectory();
    std::string getTestDirectory();
    std::string getGamesDirectory();
    std::string appendToCurrentPath(const std::string& path);
    std::string appendToSoundsPath(const std::string& path);
    std::string appendToTestROMsPath(const std::string& path);
    std::string appendToProgramsPath(const std::string& path);


};

#endif // FILESYSTEM_HPP
