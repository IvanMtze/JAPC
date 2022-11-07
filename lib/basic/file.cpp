//
// Created by wuser on 19/10/22.
//
#include "japc/basic/file.h"
File::File(const std::string &pathToSource, File::FileType fileType) : pathToSource(pathToSource), fileType(fileType)
{
    this->pathToSource = pathToSource;
    this->fileType = fileType;
}
std::string File::readFile()
{
    if (this->pathToSource.empty())
    {
        return NULL;
    }
    if (!this->verifyFile())
    {
        return NULL;
    }
    std::ifstream inFile;
    inFile.open(pathToSource);
    std::stringstream strStream;
    strStream << inFile.rdbuf();    // read the file
    this->source = strStream.str(); // str holds the content of the file
    return this->source;
}
std::string File::getContent()
{
    return this->source;
}
bool File::verifyFile()
{
    struct stat buff;
    return (stat(pathToSource.c_str(), &buff) == 0);
}
File::FileType File::getFileType()
{
    return this->fileType;
}
SourceManager::SourceManager()
{
    this->sourcesMap = SourcesMap();
}
void SourceManager::insertFile(std::string path, File::FileType fileType)
{
    auto ptr_file = std::make_shared<File>(File(path, fileType));
    if(ptr_file->verifyFile())
    {
        this->sourcesMap.insert(std::pair<std::shared_ptr<File>, std::string>(ptr_file, path));
    }else{
        fprintf(stderr, "Unable to find %s", path.c_str());
        exit(-1);
    }

}
std::shared_ptr<File> SourceManager::getFile(std::string name)
{
    for(auto const& x: this->sourcesMap){
        if(x.second == name){
            return x.first;
        }
    }
    return nullptr;
}

std::vector<std::string> SourceManager::getFileNameByType(File::FileType fileType){
    std::vector<std::string> fileNames;
    for(auto const& x: this->sourcesMap){
        if(x.first->getFileType() == fileType){
            fileNames.push_back(x.second);
        }
    }
    return fileNames;
}