#include "builder.h"
#include "../../lib/h/fileHandler.h"
#include "../../lib/h/metaData.h"
//done
/*public*/
Builder::Builder() : Builder("config.conf"){}
Builder::Builder(const std::string& configPath){
    const std::string folders[] = {"./src/md","./staging", "./staging/posts", "../pages" ,"../pages/posts", "../pages/categories"};
    for(std::string s : folders){
        std::filesystem::create_directories(s);
    }
    FileHandler::fileToUmap(configMap, configPath);
    FileHandler::fileToUmap(templateMap, "templates.conf");
}
Builder::~Builder() {}

std::pair<std::string, std::string> Builder::makePost(const std::string& mdPath, const std::string& base){ //return html string
    try{
        auto fr = parseFrontMarker(mdPath);
    std::string result = base;
    replace(result, getConfig("del_start") + "content" + getConfig("del_end"), mdToHtml(fr.second));
    return std::make_pair(fr.first, result);
    }
    catch(const std::exception& e){
        throw std::runtime_error("Error processing file: " + std::string(e.what()));
        return {"",""};
    }
}
std::string Builder::tagToHtml(const std::string& tagStr){
    std::string str = tagStr;
    std::string result = elementToHtml("tags") + ""; //<ul class="tags">
    size_t pos = str.find(","); //일단 찾아봐
    if(str != "none"){  //none이면 걍 끝내
        while(pos != std::string::npos){    //존재하면 반복
            size_t pos = str.find(",");     //pos 갱신
            if(pos != std::string::npos){   // ,가 존재하면 -> 테그가 2개 이상이면            
                result += elementToHtml("tag") + str.substr(0, pos) + elementToHtml("tag",true);
                replace(result, "{{data}}", "../pages/categories/" + str.substr(0,pos) + ".html"); //templates.conf 참조
                str = str.substr(pos+1);
            }
            else break;
        }
    }
    if(str !="" && str != "none")
        result += elementToHtml("tag") + str + elementToHtml("tag",true);       //<li><a href="">str</a></li>
        replace(result, "{{data}}", "../pages/categories/" + str + ".html"); //templates.conf 참조
    return result + elementToHtml("tags", true);

}
/*private*/

void Builder::appendHtml(std::string& content, const std::string& line, size_t depth) {
    content += getIndent(depth) + line + "\n";
}
void Builder::pushTag(std::string& tag) {   //수정 필요 
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
std::string Builder::elementToHtml(const std::string& str, bool isClosing) {
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
                replace(staging, "{{data}}", trim(line.substr(getConfig("data_marker").length()))); //templates.conf 참조
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