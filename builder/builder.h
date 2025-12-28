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
        enum ConsoleColor {
        C_BLACK = 0,
        C_BLUE = 1,
        C_GREEN = 2,
        C_CYAN = 3,
        C_RED = 4,
        C_MAGENTA = 5,
        C_YELLOW = 6,
        C_LIGHTGRAY = 7,
        C_DARKGRAY = 8,
        C_LIGHTBLUE = 9,
        C_LIGHTGREEN = 10,
        C_LIGHTCYAN = 11,
        C_LIGHTRED = 12,
        C_LIGHTMAGENTA = 13,
        C_LIGHTYELLOW = 14,
        C_WHITE = 15
    };
        enum class HtmlTag {
            DIV,
            SPAN,
            H1,
            H2,
            H3,
            P,
            A,
            IMG,
            UL,
            LI,
            HEADER,
            FOOTER,
            NAV,
            SECTION,
            ARTICLE,
            TITLE,
            DATE,
            CONTENT,

            BOX,
            LINK,
            TEXT
        };
        std::vector<HtmlTag> tagStack;

        bool inFrontMatter = false;
        bool frontMatterDone = false;
        std::string title;
        std::string date;
        std::string base;

        void printColored(std::string text, char textColor, char bgColor = 0);
        Builder();
        ~Builder();
        std::string mdToHtml(const std::filesystem::path& mdPath);
        void replace(std::string& s, const std::string& key, const std::string& value);
        std::string loadFile(const std::string& path);

    private:

        bool parseTag(const std::string& str, HtmlTag& outTag);
        void pushTag(HtmlTag& tag);
        bool popTag(HtmlTag& tag);
        std::string startMarker = "=";
        std::string endMarker = "-";
    
};
#endif // BUILDER_H
