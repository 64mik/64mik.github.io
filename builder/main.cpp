#include "builder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>

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
void startMsg(){
    printColored("\n-----------\n", C_YELLOW);
    printColored("1. md to html.\n", C_LIGHTCYAN);
    printColored("2. publish posts.\n", C_LIGHTCYAN);
    printColored("3. exit.\n", C_LIGHTCYAN);
    printColored(">> ", C_WHITE);
}

int main() {
    std::string folders[] = {"./src", "./temp", "../posts"};
    for(std::string s : folders){
        std::filesystem::create_directories(s);
    }
    Builder b;
    std::string buildCacheFile = "build_cache.txt";
    std::string metaCacheFile = "meta.txt";
    std::string basePath = "./base.html";
    std::string selection;
    std::unordered_map<std::string, uint64_t> cacheMap;
    std::unordered_map<std::string, uint64_t> metaMap;
    b.loadCacheFile(buildCacheFile, cacheMap);
    b.loadCacheFile(metaCacheFile, metaMap);
    while(true) {
        startMsg();
        std::getline(std::cin, selection);
        if(selection == "md to html" || selection == "1") {
            for (const auto& entry : std::filesystem::directory_iterator("./src/")) {
                std::string path = entry.path().string();
                if (entry.path().extension() == ".md" && (b.isChanged(path, cacheMap) || !std::filesystem::exists("./temp/" + entry.path().stem().string() + ".html"))) {
                    printColored("Processing: " + entry.path().string() + "\n", C_LIGHTGREEN);
                    cacheMap[path] = b.hashing(path);
                    std::ofstream outFile("./temp/" + entry.path().stem().string() + ".html");
                    outFile << b.makePost(entry.path().string(), basePath);
                    outFile.close();
                }
            }
            b.saveCacheFile(buildCacheFile, cacheMap);
            b.saveCacheFile(metaCacheFile, metaMap);
            printColored("Finished processing markdown files.\n", C_LIGHTGREEN);
        }
        else if(selection == "publish posts" || selection == "2"){
            for (const auto& entry : std::filesystem::directory_iterator("./temp/")) {
                std::string path = entry.path().string();
                if (entry.path().extension() == ".html" && b.isChanged(path, cacheMap)) {
                    printColored("Publishing: /posts/" + entry.path().stem().string() + ".html" + "\n", C_LIGHTGREEN);
                    std::string postContent = b.loadFile(entry.path().string());
                    std::ofstream outFile("../posts/" + entry.path().stem().string() + ".html");
                    outFile << postContent;
                    outFile.close();
                    cacheMap[path] = b.hashing(path);;
                }
            }
            b.saveCacheFile(buildCacheFile, cacheMap);
            printColored("Finished publishing posts.\n", C_LIGHTGREEN);
        }
        else if(selection == "exit" || selection == "3"){
            return 0;
        }
        else {
            printColored("Unknown command: " + selection + "\n", C_LIGHTRED);
        }
    }

    return 0;
}