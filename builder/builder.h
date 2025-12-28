#ifndef BUILDER_H
#define BUILDER_H
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <filesystem>
#include <vector>
class Builder {
    public:
        enum class HtmlTag {
            SECTION,
            ARTICLE,
            DATE,
            
            BOX,
            LINK,
            TEXT,
            TITLE,
            IMG,
            UL,
            LI,
            NONE,
            
        };
        Builder();
        ~Builder();
        void replace(std::string& s, const std::string& key, const std::string& value);
        std::string mdToHtml(const std::filesystem::path& mdPath);
        std::string loadFile(const std::string& path);

    private:
        void appendHtml(std::string& content, const std::string& line, size_t depth=0);
        void pushTag(HtmlTag& tag);
        bool popTag(HtmlTag& tag);
        std::string getIndent(size_t depth);
        std::string trim(const std::string& s);

        std::string elementToHtml(HtmlTag tag, bool isClosing= false);
        HtmlTag parseTag(const std::string& str);
        HtmlTag suitableTag(HtmlTag tag);
        
        int defaultIndent = 3;
        bool inFrontMatter = false;
        bool frontMatterDone = false;
        std::string startMarker = "<=";
        std::string endMarker = "=>";
        std::string dataMarker = "::";
        std::string title;
        std::string date;
        std::vector<HtmlTag> tagStack;
};
#endif // BUILDER_H
