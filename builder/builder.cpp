#include "builder.h"
#include "mlog.h"
#include "loggerFactory.h"
#include "logger.h"
//done
/*public*/
void setLogger(){
    mik64::Logger::getInstance().setWriters(mik64::LoggerFactory::getWriters({"console", "file"}));
}
Builder::Builder() : Builder("config.conf"){}
Builder::Builder(const std::string& configPath){
    const std::string folders[] = {"./src/md","./staging", "./staging/posts", "../pages" ,"../pages/posts", "../pages/categories"};
    for(std::string s : folders){
        std::filesystem::create_directories(s);
    }
    //FileHandler::fileToUmap(configMap, configPath);
}
Builder::~Builder() {}
std::pair<std::string, std::string> Builder::makePost(const std::string& mdFolder, const std::string& htmlFolder){
    try{
        auto fr = parseFrontMarker(mdFolder);
    std::string result = base;
    replace(result, getConfig("del_start") + "content" + getConfig("del_end"), mdToHtml(fr.second));
    return std::make_pair(fr.first, result);
    }
    catch(const std::exception& e){
        throw std::runtime_error("Error processing file: " + std::string(e.what()));
        return {"",""};
    }
}
/*private*/
//yet
std::pair<std::string, std::string> Builder::parser(const std::string& line, std::string del){
    trim(line);
    auto pos = line.find(del);
    if (pos != std::string::npos) {
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        return { key, value };
    }
    return {"",""};
}
std::pair<std::string, std::string> Builder::parseFrontMarker(const std::string& path){
    std::ifstream infile(path);
    if(!infile.is_open()){
        throw std::runtime_error("Could not open file: " + path);
    }
    std::string line;
    std::string frontMatter="";
    std::string content="";
    
    bool inFrontMatter = false;
    bool frontMatterDone = false;
    
    while(std::getline(infile, line)){
        if (line == "---" && !frontMatterDone) {    //분기 입장
            if (!inFrontMatter) {//첫 입장시
                inFrontMatter = true;
                continue;
            }
            if (inFrontMatter) {//종료
                inFrontMatter = false;
                frontMatterDone = true;
                continue;
            }
        }
        if (inFrontMatter) {
            frontMatter += line + '\n';
            continue;
        }
        content += line + '\n';
    }
    return {frontMatter, content};
}

std::string Builder::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start + 1);
}

