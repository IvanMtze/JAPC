//
// Created by wuser on 19/10/22.
//

#ifndef JAPC_FILE_H
#define JAPC_FILE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <memory>
#include <map>
#include <sys/stat.h>
#include <sstream>
class File{
  public:
    enum FileType{
        MAIN_PROGRAM,
        DEPENDENCY
    };
    File(const std::string &pathToSource, FileType fileType);
    std::string readFile();
    std::string getContent();
    bool verifyFile();
    FileType getFileType();
  private:
    std::string source;
    std::string pathToSource;
    FileType fileType;
};

class SourceManager{
  public:
    SourceManager();
    void insertFile(std::string path, File::FileType fileType);
    void getFile(std::string name);
    typedef std::map<std::shared_ptr<File>, std::string> SourcesMap;
  private:
    SourcesMap sourcesMap;
};
#endif // JAPC_FILE_H
