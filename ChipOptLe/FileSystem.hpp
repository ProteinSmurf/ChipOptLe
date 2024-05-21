//#ifndef FILESYSTEM_H
//#define FILESYSTEM_H
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <filesystem> //c++ 17 !
//
//class FileSystem
//{
//public:
//    static std::vector<std::string> getDirectoryContents(const std::string& directory)
//    {
//        std::vector<std::string> contents;
//
//        for (const auto& entry : std::filesystem::directory_iterator(directory))
//        {
//            contents.push_back(entry.path().filename().string());
//        }
//
//        return contents;
//    }
//};
//
//#endif // FILESYSTEM_H
