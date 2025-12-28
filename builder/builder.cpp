#include "builder.h"


Builder::Builder() {
    //config 읽기
}
Builder::~Builder() {}

void Builder::printColored(std::string text, char textColor, char bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD attributes = (bgColor << 4) | textColor;
    SetConsoleTextAttribute(hConsole, attributes);
    std::cout << text;
    SetConsoleTextAttribute(hConsole, (C_BLACK << 4) | C_LIGHTGRAY); // Reset to default
}


/////////////

std::string Builder::mdToHtml(const std::filesystem::path& mdPath) {
    std::ifstream inFile(mdPath);
    std::string content = "";
    std::string line;
    HtmlTag tag;

    while (std::getline(inFile, line)) {
        // Front Matter 처리
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
        //키, 값 추출
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
        if (line.empty()) continue;
        
        // content에 마크다운 저장
        if (line.size() >= 2 && line.rfind(startMarker + " ", 0) == 0) {    //= 만나면
            if(parseTag(line.substr(2), tag)) { //실존하는지 보고
                pushTag(tag);   //삽입
                content += "<" + line.substr(2) + ">\n";
                continue;
            }
        }
        else if (line.size() >= 2 && line.rfind(endMarker + " ", 0) == 0) { //- 만나면
            if (popTag(tag)) { //스택에서 꺼내서
                content += "</" + std::to_string(static_cast<int>(tag)) + ">\n"; //닫기 태그 삽입
                continue;
            }
        }

        else {
            content += "        <p class='text'>" + line + "</p>\n";
        }
    }
    return content;
}
void Builder::replace(std::string& s, const std::string& key, const std::string& value) {
    size_t pos;
    while ((pos = s.find(key)) != std::string::npos)
        s.replace(pos, key.length(), value);
}
std::string Builder::loadFile(const std::string& path) {
    //읽어서 일부는 스택 일부는 변수에 저장
    std::ifstream file(path);
    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}
bool Builder::parseTag(const std::string& str, HtmlTag& outTag) {
    if (str == "Div") {
        outTag = HtmlTag::DIV;
        return true;
    }
    if (str == "P") {
        outTag = HtmlTag::P;
        return true;
    }
    if (str == "Box") {
        outTag = HtmlTag::BOX;
        return true;
    }
    return false;
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