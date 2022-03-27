#include "reporter.h"

using namespace std;

void TexReporter::MakeReport(std::ostream& os, const std::vector<std::string>& pict_paths)
{
    if (pict_paths.empty()) return;

    os << "\\documentclass{article}\n";
    os << "\\usepackage{graphicx}\n";
    os << "\\title{\\textbf{Step by step NFA to DFA conversion}}\n";
    os << "\\author{Generated automatically}\n";
    os << "\\begin{document}\n";
    os << "\\maketitle\n";

    for (int i = 0; i < pict_paths.size(); ++i) {
        os << "\\section{";
        if (i == 0) os << "Input NFA";
        else        os << "Step " + to_string(i+1);
        os << "}\n";

        os << "\\begin{center}\n";
        os << "\\includegraphics[width=\\textwidth]{" << pict_paths[i] << "}\n";
        os << "\\end{center}\n";
    }

    os << "\\end{document}";
}
