#include "builder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

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
void printColored(std::string text, char textColor, char bgColor=C_BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD attributes = (bgColor << 4) | textColor;
    SetConsoleTextAttribute(hConsole, attributes);
    std::cout << text;
    SetConsoleTextAttribute(hConsole, (C_BLACK << 4) | C_LIGHTGRAY); // Reset to default
}

int main() {
    std::filesystem::create_directories("./src");
    std::filesystem::create_directories("./temp");
    std::filesystem::create_directories("../posts");
    Builder b; 
    std::string content;

    std::string selection;
    while(true) {
        printColored("\n-----------\n", C_YELLOW);
        printColored("1. md to html.\n", C_LIGHTCYAN);
        printColored("2. publish posts.\n", C_LIGHTCYAN);
        printColored("3. exit.\n", C_LIGHTCYAN);
        printColored(">> ", C_WHITE);
        std::getline(std::cin, selection);
        if(selection == "md to html" || selection == "1") {
            for (const auto& entry : std::filesystem::directory_iterator("./src/")) {
                if (entry.path().extension() == ".md") {
                    std::string base = b.loadFile("./base.html");
                    printColored("Processing: " + entry.path().string() + "\n", C_LIGHTGREEN);
                    content = b.mdToHtml(entry.path());
                    b.replace(base, "{{content}}", content);
                    std::ofstream outFile("./temp/" + entry.path().stem().string() + ".html");
                    outFile << base;
                    outFile.close();
                }
            }
            printColored("Finished processing markdown files.\n", C_LIGHTGREEN);
        }
        else if(selection == "publish posts" || selection == "2"){
            for (const auto& entry : std::filesystem::directory_iterator("./temp/")) {
                if (entry.path().extension() == ".html") {
                    printColored("Publishing: /posts/" + entry.path().stem().string() + ".html" + "\n", C_LIGHTGREEN);
                    std::string postContent = b.loadFile(entry.path().string());
                    std::ofstream outFile("../posts/" + entry.path().stem().string() + ".html");
                    outFile << postContent;
                    outFile.close();
                }
            }
            printColored("Finished publishing posts.\n", C_LIGHTGREEN);
        }
        else if(selection == "exit" || selection == "3") return 0;
        else {
            printColored("Unknown command: " + selection + "\n", C_LIGHTRED);
        }
    }

    return 0;
}