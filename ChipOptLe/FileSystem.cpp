#include "FileSystem.hpp"

const std::string soundsDirectory = "sounds";
const std::string testROMsDirectory = "testROMs";
const std::string programsDirectory = "programs";


FileSystem::FileSystem()
{
    //
}


std::vector<std::string> FileSystem::getDirectoryContents(const std::string& directory)
{
    std::vector<std::string> contents;

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        contents.push_back(entry.path().filename().string());
    }

    return contents;
}

std::string FileSystem::getCurrentDirectory()
{
    return std::filesystem::current_path().string();
}

std::string FileSystem::appendToCurrentPath(const std::string& path)
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path newPath = currentPath / path;
    return newPath.string();
}

std::string FileSystem::appendToSoundsPath(const std::string& path)
{
    std::filesystem::path soundsPath = std::filesystem::current_path() / soundsDirectory / path;
    return soundsPath.string();
}

std::string FileSystem::appendToTestROMsPath(const std::string& path)
{
    std::filesystem::path testROMsPath = std::filesystem::current_path() / testROMsDirectory / path;
    return testROMsPath.string();
}

std::string FileSystem::appendToProgramsPath(const std::string& path)
{
    std::filesystem::path programsPath = std::filesystem::current_path() / programsDirectory / path;
    return programsPath.string();
}

std::string FileSystem::getSoundsDirectory()
{
    std::filesystem::path soundsPath = std::filesystem::current_path() / soundsDirectory;
    return soundsPath.string();
}
std::string FileSystem::getTestDirectory()
{
    std::filesystem::path testPath = std::filesystem::current_path() / testROMsDirectory;
    return testPath.string();
}

std::string FileSystem::getGamesDirectory()
{
    std::filesystem::path gamesPath = std::filesystem::current_path() / programsDirectory;
    return gamesPath.string();
}
