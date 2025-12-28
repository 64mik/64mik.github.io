#include "builder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
int main() {
    std::filesystem::create_directories("./src");
    std::filesystem::create_directories("./temp");
    Builder b; 
    std::string content;
    b.base = b.loadFile("./base.html");

    std::string selection;
    while(true) {
        b.printColored("\n-----------\n", Builder::C_YELLOW);
        b.printColored("1. md to html.\n", Builder::C_LIGHTCYAN);
        b.printColored("2. publish posts.\n", Builder::C_LIGHTCYAN);
        b.printColored("3. exit.\n", Builder::C_LIGHTCYAN);
        b.printColored(">> ", Builder::C_WHITE);
        std::getline(std::cin, selection);
        if(selection == "md to html" || selection == "1") {
            for (const auto& entry : std::filesystem::directory_iterator("./src")) {
                if (entry.path().extension() == ".md") {
                    b.printColored("Processing: " + entry.path().string() + "\n", Builder::C_LIGHTGREEN);
                    content = b.mdToHtml(entry.path());
                    b.replace(b.base, "{{content}}", content);
                    std::ofstream outFile("./temp/" + entry.path().stem().string() + ".html");
                    outFile << b.base;
                    outFile.close();
                }
            }
            b.printColored("Finished processing markdown files.\n", Builder::C_LIGHTGREEN);
        }
        else if(selection == "pushing staging htmls" || selection == "2") break;
        else if(selection == "exit" || selection == "3") return 0;
        else {
            b.printColored("Unknown command: " + selection + "\n", Builder::C_LIGHTRED);
        }
    }

    return 0;
}