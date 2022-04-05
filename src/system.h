#pragma once
#include <string>
#include <cstdlib>

class CSystem
{
public:
    static void PrepareOutDirectory();
    static std::string GetOutputDir();
    static std::string CreateFileInOutputDir(const std::string& filename);

    static std::string DotCompile(const std::string& file);
private:
    inline static const std::string out_dir_name = "out";
};