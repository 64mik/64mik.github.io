#ifndef BUILDER_H
#define BUILDER_H
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <cstdint>
class Builder {
    public:
         Builder();
        ~Builder();
        
        void loadCacheFile(const std::string& cacheFilePath, std::unordered_map<std::string, uint64_t>& map);
        void saveCacheFile(const std::string& cacheFilePath, std::unordered_map<std::string, uint64_t>& map);
        bool isChanged(const std::string& path, const std::unordered_map<std::string, uint64_t>& map);
        std::string makePost(const std::string& mdPath, const std::string& templatePath);
        std::string loadFile(const std::string& path);
        uint64_t hashing(const std::string& path);
        
    private:
        std::unordered_map<std::string, std::string> configMap;    
        std::unordered_map<std::string, std::string> templateMap;    
        std::vector<std::string> tagStack;
        
        int defaultIndent = 3;

        void applyConfig(std::string& content);
        void resetConfig(std::unordered_map<std::string, std::string>& map, const std::string& configFileName = "config.conf");
        void appendHtml(std::string& content, const std::string& line, size_t depth=0);
        void pushTag(std::string& tag);
        bool popTag(std::string& tag);
        void replace(std::string& s, const std::string& key, const std::string& value);
        std::string elementToHtml(std::string str, bool isClosing= false);
        std::string getConfig(const std::string& key);
        std::string getIndent(size_t depth);
        std::string mdToHtml(const std::filesystem::path& mdPath);
        std::string trim(const std::string& s);
        
};
#endif // BUILDER_H
