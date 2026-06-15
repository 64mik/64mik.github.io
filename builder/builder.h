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
        std::pair<std::string, std::string> makePost(const std::string& mdFolder, const std::string& htmlFolder);
        std::string mdToHtml(const std::string& mdContent);

        void replace(std::string& s, const std::string& key, const std::string& value);
        std::string getConfig(const std::string& key);
        static std::pair<std::string, std::string> parser(const std::string& line,const std::string del);
        static std::string trim(const std::string& s);
        //빌더에선 설정이나, 각종 템플릿을 불러오는 역할만 하고, 실제 변환은 htmlConverter에서 담당하도록 해야할듯
        //경로같은것도 빌더에서 관리하는게 좋을듯
    private:
        std::unordered_map<std::string, std::string> configMap;   
        std::pair<std::string, std::string> parseFrontMarker(const std::string& path);
        
};

#endif // BUILDER_H
