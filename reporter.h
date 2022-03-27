#pragma once
#include <string>
#include <vector>
#include <ostream>

class TexReporter
{
public:
    static void MakeReport(std::ostream& os, const std::vector<std::string>& pict_paths);
};