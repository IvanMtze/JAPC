//
// Created by wuser on 10/10/22.
//

#ifndef JAPC_COMPILER_H
#define JAPC_COMPILER_H

#include "japc/AST/context.h"
#include "japc/AST/moduledef.h"
#include "japc/builder/builder.h"
#include <memory>
#include <vector>

namespace Pascal
{
class Compiler
{
  private:
    static Compiler *instance;
    std::shared_ptr<Context> rootContext;
    std::shared_ptr<Builder> rootBuilder;
    typedef std::map<std::string, ModuleDef> ModuleMap;
    ModuleMap moduleCache;
    std::vector<ModuleDef *> loadedModules;
    bool initialized;
    Compiler();
    bool init();

  public:
    typedef std::vector<std::string> StringVec;
    typedef StringVec::const_iterator StringVecIter;

    struct ModulePath
    {
        std::string path;
        bool found, isDir;
        ModulePath(std::string &path, bool found, bool isDir) : path(path), found(found), isDir(isDir)
        {
        }
    };

    std::vector<std::string> sourceLibPath;
    bool dump;
    int optimizeLevel;
    bool emitDebugInfo;
    bool noBootstrap;
    bool useGlobalLibs;

    static std::string joinName(const std::string &base, StringVecIter pathBegin, StringVecIter pathEnd,
                                const std::string &ext);
    static bool isFile(const std::string &name);
    static bool isDir(const std::string &name);
    static ModulePath searchPath(const StringVec &path, StringVecIter moduleNameBegin, StringVecIter modulePathEnd,
                                 const std::string &extension);
    static Compiler &getInstance();
    void addSourceToLibPath(const std::string &path);
    void parseModule(Context &moduleContext, ModuleDef *module, const std::string &path, std::istream &src);
    ModuleDef loadModule(StringVecIter moduleNameBegin, StringVecIter moduleNameEnd, std::string &canonicalName);
    bool loadBootstrapModules();
    void setArgv(int argc, char **argv);
    int run(std::istream &src, const std::string &name);
    static ModuleDef loadModule(const std::vector<std::string> &moduleName, std::string &canonicalName);
    void callModuleDestructors();
};
} // namespace Pascal
#endif // JAPC_COMPILER_H
