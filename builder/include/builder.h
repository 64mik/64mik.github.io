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
        Builder();
        Builder(const std::string& configPath);
        ~Builder();
        
        std::pair<std::string, std::string> makePost(const std::string& mdPath, const std::string& base);
        std::string getConfig(const std::string& key);
        std::pair<std::string, std::string> parser(const std::string& line,const std::string del);
        void replace(std::string& s, const std::string& key, const std::string& value);
        std::string tagToHtml(const std::string& tagStr);
    private:
        std::unordered_map<std::string, std::string> configMap;    
        std::unordered_map<std::string, std::string> templateMap;
        std::vector<std::string> tagStack;
        int defaultIndent = 2;  //하드코딩 느낌 바꿔야함
        std::string trim(const std::string& s);
        void appendHtml(std::string& content, const std::string& line, size_t depth=0);
        void pushTag(std::string& tag);
        bool popTag(std::string& tag);
        std::string elementToHtml(const std::string& str, bool isClosing= false);
        std::string getIndent(size_t depth);
        std::string mdToHtml(const std::string& str);
        
        std::pair<std::string, std::string> parseFrontMarker(const std::string& path);
        
};
#endif // BUILDER_H
