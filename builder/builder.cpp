#include "builder.h"

Builder::Builder() {
    //config 읽기
}
Builder::~Builder() {}
//done
void Builder::appendHtml(std::string& content, const std::string& line, size_t depth) {
    content += getIndent(depth) + line + "\n";
}
void Builder::replace(std::string& s, const std::string& key, const std::string& value) {
    size_t pos;
    while ((pos = s.find(key)) != std::string::npos)
        s.replace(pos, key.length(), value);
}
void Builder::pushTag(HtmlTag& tag) {
    tagStack.push_back(tag);
}
bool Builder::popTag(HtmlTag& tag) {
    if (!tagStack.empty()) {
        tag = tagStack.back();
        tagStack.pop_back();
        return true;
    }
    else {
        return false;
    }
}
std::string Builder::loadFile(const std::string& path) {
    std::ifstream file(path);
    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}
std::string Builder::getIndent(size_t depth) {
    return std::string((depth + defaultIndent) * 4, ' ');
}
std::string Builder::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start + 1);
}
//yet
std::string Builder::mdToHtml(const std::filesystem::path& mdPath) {
    std::string content = "";
    std::string line;
    std::string staging="";
    HtmlTag tag;
    std::ifstream inFile(mdPath);

    while (std::getline(inFile, line)) {
        if (line == "---") {
            if (!inFrontMatter && !frontMatterDone) {
                inFrontMatter = true;
                continue;
            }
            if (inFrontMatter) {
                inFrontMatter = false;
                frontMatterDone = true;
                continue;
            }
        }
        if (inFrontMatter) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                value.erase(0, value.find_first_not_of(" "));
                if (key == "title") title = value;
                if (key == "date")  date  = value;
            }
            continue;
        }
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty()) continue;
        
        //stagging 기존재 시 비우기
        if(staging != ""){  
            //데이터 라인인지 확인
            if (line.size() > 2 && line.rfind(dataMarker + " ", 0) == 0) {  
                replace(staging, "{{data}}", trim(line.substr(dataMarker.length())));
                appendHtml(content, staging, tagStack.size()-1);
                staging = "";
                continue;
            }
            else {  
                //데이터 라인이 아니라면
                appendHtml(content, staging, tagStack.size()-1);
                staging = "";
            }
        }

        //시작 태그 처리
        if (line.size() > 2 && line.rfind(startMarker + " ", 0) == 0) {
            std::string tagName = trim(line.substr(startMarker.length())); 
            tag = parseTag(tagName);
            
            // 존재하는 태그라면 스테이징
            if(tag != HtmlTag::TEXT) {   
                staging = elementToHtml(tag);
                pushTag(tag);
                continue;
            }
            else {
                
                //존재하지 않는 태그라면 경고 출력 및 p로 처리
                std::cout <<"Unknown tag: "<< line.substr(2) << std::endl;
            }
        }
        else if (line == endMarker) {
            if (popTag(tag)) {
                appendHtml(content, elementToHtml(tag, true), tagStack.size());
                continue;
            }
        } 

        //현 테그에 적합한 메시지 테그 부여
        if(tagStack.empty())
            tag = HtmlTag::TEXT;
        else
            tag = suitableTag(tagStack.back());

        appendHtml(content, elementToHtml(tag) + line + elementToHtml(tag, true), tagStack.size());
        
    }
    return content;
}
Builder::HtmlTag Builder::parseTag(const std::string& str) {
    std::cout << "parsing tag: "<< str << std::endl;
    if (str == "box") return HtmlTag::BOX;
    if (str == "title") return HtmlTag::TITLE;
    if (str == "link") return HtmlTag::LINK;
    if (str == "list") return HtmlTag::UL;
    return HtmlTag::TEXT;
}
std::string Builder::elementToHtml(HtmlTag tag, bool isClosing) {
    if(!isClosing){
        switch (tag) {
            case HtmlTag::BOX:    return "<div class=\"box\">";
            case HtmlTag::TITLE:  return "<h2 class=\"title\">";
            case HtmlTag::TEXT:   return "<p>";
            case HtmlTag::LINK:   return "<a class =\"link\" href=\"/{{data}}\">";
            case HtmlTag::UL:     return "<ul>";
            case HtmlTag::LI:     return "<li>";

            case HtmlTag::IMG:    return "<img src =\"{{data}}\">";
            default:              return "";
        }
    }
    else{
        switch (tag) {
            case HtmlTag::BOX:    return "</div>";
            case HtmlTag::TITLE:  return "</h2>";
            case HtmlTag::TEXT:   return "</p>";
            case HtmlTag::LINK:   return "</a>";
            case HtmlTag::UL:     return "</ul>";
            case HtmlTag::LI:     return "</li>";

            case HtmlTag::IMG:    return "</img>";
            default:              return "";
        }
    }
}
Builder::HtmlTag Builder::suitableTag(Builder::HtmlTag tag){
    switch (tag) {
        case HtmlTag::LINK: return HtmlTag::NONE;
        case HtmlTag::TITLE: return HtmlTag::NONE;
        case HtmlTag::UL: return HtmlTag::LI;
        default: return HtmlTag::TEXT;
    }
}