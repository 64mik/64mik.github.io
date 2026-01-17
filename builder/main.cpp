#include "builder.h"
#include <iostream>

void startMsg(){
    std::cout<<"\n-----------\n1. md to html.\n2. publish posts.\n3. exit.\n>> ";
}

int main() {
    std::string folders[] = {"./src", "./temp", "../posts"};
    for(std::string s : folders){
        std::filesystem::create_directories(s);
    }
    Builder b("config.conf");
    std::string selection;
    std::unordered_map<std::string, uint64_t> cacheMap;
    std::unordered_map<std::string, std::string> metaMap;
    b.loadCacheFile(b.getConfig("build_cache_file"), cacheMap);
    std::ofstream metaFile(b.getConfig("meta_info_file"));
    while(true) {
        startMsg();
        std::getline(std::cin, selection);
        if(selection == "md to html" || selection == "1") {
            for (const auto& entry : std::filesystem::recursive_directory_iterator("./src")) {
                std::string path = entry.path().string();
                if (entry.path().extension() == ".md" && (b.isChanged(path, cacheMap) || !std::filesystem::exists("./temp/" + entry.path().stem().string() + ".html"))) {
                    std::cout << "Processing: " + entry.path().string() + "\n";
                    cacheMap[path] = b.hashing(path);
                    std::ofstream outFile("./temp/" + entry.path().stem().string() + ".html");
                    outFile << b.makePost(metaFile ,entry.path().string(), b.getConfig("base_path"));
                    outFile.close();
                }
            }
            b.saveCacheFile(b.getConfig("build_cache_file"), cacheMap);
            std::cout << "Finished processing markdown files.\n";
        }
        else if(selection == "publish posts" || selection == "2"){
            for (const auto& entry : std::filesystem::directory_iterator("./temp/")) {
                std::string path = entry.path().string();
                if (entry.path().extension() == ".html" && b.isChanged(path, cacheMap)) {
                    std::cout << "Publishing: "+ entry.path().stem().string() + ".html" + "\n";
                    std::string postContent = b.loadFile(entry.path().string());
                    std::string outPath = "../" + entry.path().stem().string() + ".html";
                    if (entry.path().stem().string().find("post_") != std::string::npos)
                        outPath = "../posts/" + entry.path().stem().string() + ".html";
                    std::ofstream outFile(outPath);
                    outFile << postContent;
                    outFile.close();
                    cacheMap[path] = b.hashing(path);;
                }
            }
            b.saveCacheFile(b.getConfig("buildCacheFile"), cacheMap);
            std::cout << "Finished publishing posts.\n";
        }
        else if(selection == "exit" || selection == "3"){
            return 0;
        }
        else {
            std::cout<< "Unknown command: " + selection + "\n";
        }
    }

    return 0;
}