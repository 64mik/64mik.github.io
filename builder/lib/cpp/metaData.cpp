#include "../h/metaData.h"

metaData::metaData() {}
metaData::~metaData() {}


void  metaData::sortMetaVector(std::ifstream& infile, std::vector<pageData>& v){
    std::string line;
    while(std::getline(infile, line)){
    }
}
void metaData::pushMeta(pageData& pd){
    metaVector.push_back(pd);
}
metaData::pageData metaData::getMeta(const std::string &fileName){
    for(const auto& pd : metaVector){
        if(pd.fileName == fileName){
            return pd;
        }
    }
    throw std::invalid_argument("[0003] error: File not found -> " + fileName);
}