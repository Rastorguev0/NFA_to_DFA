#pragma once
#include <string>
#include <vector>
#include <ostream>

class TexReporter
{
public:
    static void MakeReport(std::ostream& os,
        const std::string& main_pict_path,
        const std::vector<std::string>& lambda_del_paths,
        const std::vector<std::string>& conversion_paths
        );
};