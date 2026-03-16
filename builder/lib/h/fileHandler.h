#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <fstream>
#include <string>
#include <unordered_map>
class FileHandler {
    public:
        static std::string loadFile(const std::string& path);
        static void fileToUmap(std::unordered_map<std::string, std::string>& map, const std::string& configFileName);
};
#endif // FILEHANDLER_H