#include "../h/caching.h"

Caching::Caching() : Caching("cache.txt") {}
Caching::Caching(const std::string& filePath) {
    std::ifstream infile(filePath);
    if(!infile.is_open()){
        throw std::runtime_error("[0006] error: Failed to open cache file: " + filePath);
    }
    infile.close();
    cacheFilePath = filePath;
    loadCacheFile();
}
Caching::~Caching() {}

std::string Caching::get(std::string& key) {
    auto it = cacheMap.find(key);
    if (it != cacheMap.end()) {
        return std::to_string(it->second);
    }
    return "";
}
void Caching::put(std::string& key, const uint64_t& value) {
    cacheMap[key] = value;
}

void Caching::clearCache() {
    cacheMap.clear();
    std::ofstream outFile(cacheFilePath, std::ios::trunc);
    outFile.close();
}
void Caching::loadCacheFile(){
    std::ifstream infile(cacheFilePath);
    if(!infile.is_open()){
        throw std::runtime_error("Failed to open cache file: " + cacheFilePath);
    }
    std::string line;
    size_t pos;
    while(std::getline(infile, line)){
        pos = line.find("|");
        if(pos !=std::string::npos){
            auto key = trim(line.substr(0, pos));
            auto value = trim(line.substr(pos+1));
            if (key.empty() || value.empty()) continue;
            cacheMap[key] = std::stoull(value);
        }
    }
}
void Caching::saveCacheFile(){
    std::ofstream outFile(cacheFilePath);
    for(auto it : cacheMap){
        outFile << it.first << "|" << it.second << '\n';
    }
}
bool Caching::isFileChanged(const std::string& path, const uint64_t& hash) {
    bool changed = true;
    auto it = cacheMap.find(path);
    if(it != cacheMap.end()){        
        changed = (hash != it->second);
    }
    return changed;
}
uint64_t Caching::hashing(const std::string& path) {
    constexpr uint64_t FNV_OFFSET = 14695981039346656037ULL;
    constexpr uint64_t FNV_PRIME  = 1099511628211ULL;
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return 0;
    uint64_t hash = FNV_OFFSET;
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        for (std::streamsize i = 0; i < file.gcount(); ++i) {
            hash ^= static_cast<uint8_t>(buffer[i]);
            hash *= FNV_PRIME;
        }
    }
    return hash;
}
std::string Caching::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start + 1);
}