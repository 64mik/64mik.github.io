#include "builder.h"
#include "mlog.h"
//#include "caching.h"
//#include "fileHandler.h"
//#include "metaData.h"
#include <iostream>
#include <functional>
#include <random>
#include <chrono>

int main(){
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    Builder b;
    //makePost를 템플릿 관련 이름으로 바꿔야할듯. 기능이 그 쪽에 더 가까우니
    //b.makePost("src/md","staging");  //md가 담긴 폴더 + html이 담길 폴더 설정
    //캐싱이나 메타데이터는 전부 빌더 안으로 넣기
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    return 0;
}

/*
struct folderPath {
    std::string source_dir;
    std::string page_dir;
    std::string post_dir;
};
const folderPath mdPaths = {".\\src\\md", ".\\staging", ".\\staging\\posts"};   //각 폴더 경로를 설정 파일에서 불러오는 것으로 수정 예정
const folderPath stagingPaths = {".\\staging", "..\\pages" ,"..\\pages\\posts"};    //이런 주소 없으면 그냥 다 현 디렉토리에 생성하는 방식으로 예외처리 필요
void processing(const folderPath& folder,const std::string& extension, Caching& cache, metaData& md, std::function<std::pair<metaData::pageData, std::string>(const std::filesystem::path&)> contentFunc) {
    uint64_t hash;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder.source_dir)) {
        std::string path = entry.path().string();
        std::filesystem::path filename = entry.path().stem();
        if (entry.path().extension() == extension && cache.isFileChanged(path, hash=cache.hashing(path))){ 
            std::cout << "Processing: " + path + "\n";
            auto [pd, content] = contentFunc(path);
            pd.fileName = filename.string();
            pd.path = path;
            md.pushMeta(pd);
            auto outPath = std::filesystem::path(folder.page_dir) / filename.replace_extension(".html");
            if(pd.meta.find("isPost") != pd.meta.end() && pd.meta.at("isPost") == "true")
                outPath = std::filesystem::path(folder.post_dir) / filename.replace_extension(".html");
            if(pd.meta.find("category") != pd.meta.end() && pd.meta.at("category") == "index")
                if(extension == ".html")
                    outPath = std::filesystem::path("..\\index.html");
            std::cout << "Output Path: " + outPath.string() + "\n";
            std::ofstream outFile(outPath, std::ios::trunc);
            if(!outFile.is_open()){
                MLOG_ERROR_S << "Error: Could not open file for writing: " + outPath.string() + "\n";
                continue;
            }
            outFile << content;
            outFile.close();
            cache.put(path, hash);
        }
    }
    cache.saveCacheFile();
}


int main() {
    Builder b;
    Caching cache("cache.txt");
    FileHandler f;
    metaData md;
    metaData::pageData pd;

    std::string content;
    std::string input;
    std::string options[4] = {"1. md to html", "2. publish posts", "3. clear cache", "4. exit"};
    int selection;
    while(true){
        std::cout << "Select an option:\n";
        for(auto n : options){
            std::cout << "- " + n + "\n";
        }
        if(!std::getline(std::cin, input)){
            MLOG_ERROR_S << "Error reading input. Please try again.\n";
            continue;
        }
        try{
            selection = std::stoi(input);
        } 
        catch(const std::exception& e){
            MLOG_ERROR_S << "Invalid input. Please enter a number.";
            continue;
        }          
        switch (selection){
            case 1:{
                std::string base=f.loadFile(b.getConfig("base_path"));
                processing(mdPaths, ".md", cache, md, [&](const std::filesystem::path& path){
                    auto fr= b.makePost(path.string(), base);
                    pd.meta.clear();
                    b.replace(fr.second, b.getConfig("del_start")+"version"+b.getConfig("del_end"), pd.version);
                    std::istringstream iss(fr.first);//프론트메타 파싱
                    for(std::string line; std::getline(iss, line); ) {
                        auto fr = b.parser(line, b.getConfig("meta_marker"));
                        if(fr.first != ""){
                            pd.meta[fr.first] = fr.second;
                        }
                    }
                    pd.meta["tags"]=b.tagToHtml(pd.meta["tags"]);
                    for(auto it : pd.meta){
                        b.replace(fr.second, b.getConfig("del_start")+it.first+b.getConfig("del_end"), it.second);
                    }
                    return std::make_pair(pd, fr.second);
                });
                MLOG_INFO_S << "Finished processing markdown files.";
                break;
            }
            case 2:{
                processing(stagingPaths, ".html", cache, md, [&](const std::filesystem::path& path){
                    std::string content = f.loadFile(path.string());
                    return std::make_pair(md.getMeta(path.stem().string()), content);
                });
                MLOG_INFO_S << "Finished publishing posts.";
                break;
            }
            case 3:
                cache.clearCache();
                MLOG_INFO_S << "Cache cleared.";
                break;
            case 4:
                MLOG_INFO_S << "exit.";
                return 0;
            default:
                MLOG_WARNING_S << "Unknown command: " + input;
                continue;
        }
  }
    return 0;
}

*/