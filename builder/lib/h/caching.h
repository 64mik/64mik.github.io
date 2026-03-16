#ifndef CASHING_H
#define CASHING_H
#include <string>
#include <unordered_map>
#include <fstream>
#include <cstdint>
class Caching {
    public:
        Caching();
        Caching(const std::string& filePath);
        ~Caching();

        std::string get(std::string& key);
        void put(std::string& key, const uint64_t& value);
        void clearCache();
        void loadCacheFile();
        void saveCacheFile();
        bool isFileChanged(const std::string& path, const uint64_t& hash);
        uint64_t hashing(const std::string& path);      
    private:

        std::string trim(const std::string& s);

        std::string cacheFilePath;
        std::unordered_map<std::string, uint64_t> cacheMap;

};

#endif // CASHING_H