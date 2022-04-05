#include <fstream>

#include "system.h"

using namespace std;

void CSystem::PrepareOutDirectory()
{
    system(string("if not exist " + out_dir_name + " mkdir " + out_dir_name).c_str());
    system(string("del /Q " + out_dir_name).c_str());
}

string CSystem::GetOutputDir()
{
    return out_dir_name;
}

std::string CSystem::CreateFileInOutputDir(const std::string& filename)
{
    const string full_name = out_dir_name + "\\" + filename;
    ofstream dummy(full_name);
    return full_name;
}

std::string CSystem::DotCompile(const std::string& file)
{
    static const string DOT_COMPILE = "dot -Tpdf -O ";

    if (file.empty()) return "";
    system(string(DOT_COMPILE + string(file)).c_str());
    system(string(("del ") + string(file)).c_str());    
    return string(file) + ".pdf";
}
