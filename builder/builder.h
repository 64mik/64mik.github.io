#ifndef BUILDER_H
#define BUILDER_H
#include <fstream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <sstream>
#include <vector>
class Builder {
    public:
        struct PostInfo {
            std::string title = "untitled";
            std::string date = "untracked";
            std::string lore = "-";
            std::string content ="none";
            std::string path;
            bool isPost = true;
        };    
        Builder();
        Builder(const std::string& configPath);
        ~Builder();
        
        void loadCacheFile(const std::string& cacheFilePath, std::unordered_map<std::string, uint64_t>& map);
        void saveCacheFile(const std::string& cacheFilePath, std::unordered_map<std::string, uint64_t>& map);
        bool isChanged(const std::string& path, const std::unordered_map<std::string, uint64_t>& map);
        std::string makePost(std::ofstream& outFile, const std::string& mdPath, const std::string& templatePath);
        std::string loadFile(const std::string& path);
        uint64_t hashing(const std::string& path);
        std::string getConfig(const std::string& key);
    private:

        std::unordered_map<std::string, std::string> configMap;    
        std::unordered_map<std::string, std::string> templateMap;
        std::vector<std::string> tagStack;
        

        int defaultIndent = 2;

        void applyConfig(std::string& content);
        void resetConfig(std::unordered_map<std::string, std::string>& map, const std::string& configFileName = "config.conf");
        void appendHtml(std::string& content, const std::string& line, size_t depth=0);
        void pushTag(std::string& tag);
        bool popTag(std::string& tag);
        void replace(std::string& s, const std::string& key, const std::string& value);
        std::string elementToHtml(std::string& str, bool isClosing= false);
        std::string getIndent(size_t depth);
        std::string mdToHtml(const std::string& str);
        std::string trim(const std::string& s);
        
        std::pair<std::string, std::string> parser(const std::string& line,const std::string del);
        std::pair<std::string, std::string> parseFrontMarker(const std::string& path);

        
        std::vector<std::string> v;

        void sortMetaVector(std::ifstream& infile, std::vector<PostInfo>& v);
};
#endif // BUILDER_H
