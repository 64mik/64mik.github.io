#ifndef METADATA_H
#define METADATA_H
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
class metaData {
    public:
        metaData();
        ~metaData();
        struct pageData{
            std::unordered_map<std::string, std::string> meta = {
                {"title", "untitled"}, 
                {"date", "untracked"},
                {"lore", "none"},
                {"tags", "none"}, 
                {"category", "none"},
                {"isPost", "false"},
                {"content", "none"},
            };
            std::string version = "v1.1.1";
            std::string path="";
            std::string fileName="";
        };
        void pushMeta(pageData& pd);
        pageData getMeta(const std::string &fileName);
        void sortMetaVector(std::ifstream& infile, std::vector<pageData>& v);
        
        void writeMetaToFile(const std::string& filePath);
        void readMetaFromFile(const std::string& filePath);
        
        std::vector<pageData> metaVector;
};
#endif // METADATA_H