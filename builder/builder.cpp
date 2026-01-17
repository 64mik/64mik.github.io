#include "builder.h"

//done
/*public*/
Builder::Builder() : Builder("config.txt"){}
Builder::Builder(const std::string& configPath){
    resetConfig(configMap, configPath);
    resetConfig(templateMap, getConfig("templates_file"));  // 없을 때 기본 설정 필요
}
Builder::~Builder() {}

void Builder::loadCacheFile(const std::string& cacheFilePath, std::unordered_map<std::string, uint64_t>& map){
    std::ifstream infile(cacheFilePath);
    if(!infile.is_open()){
        return;
    }
    std::string line;
    size_t pos;
    while(std::getline(infile, line)){
        pos = line.find("|");
        if(pos !=std::string::npos){
            auto value = trim(line.substr(pos+1));
            if (value.empty()) continue;
            map[line.substr(0, pos)] = std::stoull(value);
        }
    }
}
void Builder::saveCacheFile(const std::string& cacheFilePath, std::unordered_map<std::string, uint64_t>& map){
    std::ofstream outFile(cacheFilePath);
    for(auto it : map){
        outFile << it.first << "|" << it.second << '\n';
    }
}
bool Builder::isChanged(const std::string& path, const std::unordered_map<std::string, uint64_t>& map){
    bool changed = true;
    auto it = map.find(path);
    if(it != map.end()){        
        changed = (hashing(path) != it->second);
    }
    return changed;
}
std::string Builder::makePost(std::ofstream& metaFile, const std::string& mdPath, const std::string& templatePath){ //목적 = 그냥 간편하게 만드는 용도
    std::string base = loadFile(templatePath);
    std::string metaStr=configMap["del_start"]+"post_path:"+mdPath;
    auto fr = parseFrontMarker(mdPath);
    std::string line;
    std::istringstream iss(fr.first);
    while(std::getline(iss, line)){
        auto meta = parser(line, getConfig("meta_marker"));
        if(meta.first != ""){
            replace(base, configMap["del_start"]+meta.first+configMap["del_end"], meta.second);
            if(metaFile.is_open()){
                metaStr += ","+meta.first +getConfig("meta_marker")+ meta.second;
            }
        }
    }

    replace(base, configMap["del_start"]+"post_content"+configMap["del_end"], mdToHtml(fr.second));
    applyConfig(base);
    if(metaFile.is_open()){
        metaFile<< metaStr << configMap["del_end"] <<"\n";
    }
    return base;
}
std::string Builder::loadFile(const std::string& path) {
    std::ifstream file(path);
    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}
uint64_t Builder::hashing(const std::string& path) {
    constexpr uint64_t FNV_OFFSET = 14695981039346656037ULL;
    constexpr uint64_t FNV_PRIME  = 1099511628211ULL;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return 0;
    uint64_t hash = FNV_OFFSET;
    char c;

    while (file.get(c)) {
        hash ^= static_cast<uint8_t>(c);
        hash *= FNV_PRIME;
    }
    return hash;
}

/*private*/
void Builder::applyConfig(std::string& base){ 
    for(auto it : configMap){
        replace(base, configMap["del_start"]+it.first+configMap["del_end"], it.second);
    }
}
void Builder::resetConfig(std::unordered_map<std::string, std::string>& map, const std::string& configFileName){
    std::string line;
    std::string key;
    std::string value;
    std::ifstream infile(configFileName);

    if(!infile.is_open()){
        //std::cerr << "error: failed open file -> " << configFileName;
        return;
    }
    while(std::getline(infile, line)){
        if(line.empty() || line[0] == '#') continue;
        size_t pos = line.find("=");
        if(pos != std::string::npos){
            key =line.substr(0, pos);
            value = line.substr(pos+1);
            map[key]=value;
        }
    }
}
void Builder::appendHtml(std::string& content, const std::string& line, size_t depth) {
    content += getIndent(depth) + line + "\n";
}
void Builder::pushTag(std::string& tag) {
    tagStack.push_back(tag);
}
bool Builder::popTag(std::string& tag) {
    if (!tagStack.empty()) {
        tag = tagStack.back();
        tagStack.pop_back();
        return true;
    }
    else {
        return false;
    }
}
void Builder::replace(std::string& s, const std::string& key, const std::string& value) {
    size_t pos;
    while ((pos = s.find(key)) != std::string::npos)
        s.replace(pos, key.length(), value);
}
std::string Builder::elementToHtml(std::string& str, bool isClosing) {
    auto it = templateMap.find(str);
    if (it != templateMap.end()) {    //존재하면
        std::string s = it->second;
        size_t pos = s.find("|");
        if(pos != std::string::npos){   //앞뒤 파싱
            if(!isClosing){
                return s.substr(0, pos);
            }
            else{
                return s.substr(pos+1);
            }
        }
        else{   //npos
            //std::cerr<<"error: there is no |";
            return "";
        }
    }
    else{
        if(str != "none")
        //std::cerr << "error: Unknown tag -> " << str << '\n';
        return "";
    }
    return "";
}
std::string Builder::getConfig(const std::string& key) {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    }
    //std::cout << "error: unknown key";
    return "";
}
std::string Builder::getIndent(size_t depth) {
    return std::string((depth + defaultIndent) * 4, ' ');
}
std::string Builder::mdToHtml(const std::string& str) {
    std::istringstream iss(str);
    std::string content="";
    std::string staging="";
    std::string line;
    std::string element;

    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty()) continue;
        
        if(staging != ""){ //stagging 비우기  
            size_t depth = tagStack.empty() ? 0 : tagStack.size() - 1;
            if (line.size() > 2 && line.rfind(getConfig("data_marker") + " ", 0) == 0) { //데이터 라인인지 확인
                replace(staging, "{{data}}", trim(line.substr(getConfig("data_marker").length())));
                appendHtml(content, staging, depth);
                staging = "";
                continue;
            }
            else { //데이터 라인이 아니라면
                appendHtml(content, staging, depth);
                staging = "";
            }
        }

        
        if (line.size() > 2 && line.rfind(getConfig("start_marker") + " ", 0) == 0) { //시작 태그 처리
            element = trim(line.substr(getConfig("start_marker").length())); 
            std::string elementFront = elementToHtml(element);
            if(elementFront != ""){// 존재하는 태그라면 스테이징  
                staging = elementFront;
                pushTag(element);
                continue;
            }
            else { //존재하지 않는 태그라면 경고 출력 및 p로 처리
                //std::cout <<"Unknown tag: "<< line.substr(2) << std::endl;
            }
        }
        else if (trim(line) == getConfig("end_marker")) {
            if (popTag(element)) {
                appendHtml(content, elementToHtml(element, true), tagStack.size());
                continue;
            }
        } 

        //현 테그에 적합한 메시지 테그 부여
        if(tagStack.empty())
            element = "p";
        else{
            std::string s = tagStack.back();
            if(s == "list"){ element = "li"; }
            else if(s == "link"){ element = "none";}
            else if(s == "title"){ element = "none";}
            else if(s == "img"){ element = "none";}
            else if(s == "box"){ element = "p";}
            else { element = "p";}

        }

        appendHtml(content, elementToHtml(element) + line + elementToHtml(element, true), tagStack.size());
        
    }
    return content;
}
std::string Builder::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start + 1);
}


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
        //std::cerr<<"error: file isn't open";
        return {"", ""};
    }
    std::string line;
    std::string front="";
    std::string rear="";
    
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
            front += line + '\n';
            continue;
        }
        rear += line + '\n';
    }
    return {front, rear};
}


void sortMetaVector(std::ifstream& infile, std::vector<Builder::PostInfo>& posts){
    std::string line;
    while(std::getline(infile, line)){
        Builder::PostInfo post;
        post.isPost=true;
        posts.push_back(post);//포스트 객체 만들고 값 넣어서 백터에 푸시, post인지 아닌지 확인 하는 코드 필요
    }
}