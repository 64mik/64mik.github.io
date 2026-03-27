#include "fileHandler.h"
std::string FileHandler::loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("[0002] error: Could not open file: " + path);
    }
    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}
void FileHandler::fileToUmap(std::unordered_map<std::string, std::string>& map, const std::string& configFileName){
    std::string line;
    std::string key;
    std::string value;
    std::ofstream outfile(configFileName, std::ios::app);
    if(!outfile.is_open()){
        throw std::runtime_error("[0001] error: failed open file -> " + configFileName);
    }
    outfile.close();
    std::ifstream infile(configFileName);
    while(std::getline(infile, line)){
        if(line.empty() || line[0] == '#') continue;
        size_t pos = line.find("=");
        if(pos != std::string::npos){
            key =line.substr(0, pos);
            value = line.substr(pos+1);
            map[key]=value;
        }
    }
    infile.close();
}