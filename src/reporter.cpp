#include "reporter.h"

using namespace std;

void TexReporter::MakeReport(std::ostream& os,
    const std::string& main_pict_path,
    const std::vector<std::string>& lambda_del_paths,
    const std::vector<std::string>& conversion_paths)
{
    if (main_pict_path.empty()) return;

    os << "\\documentclass{article}\n";
    os << "\\usepackage{graphicx}\n";
    os << "\\title{\\textbf{Step by step NFA to DFA conversion}}\n";
    os << "\\author{Generated automatically}\n";
    os << "\\begin{document}\n";
    os << "\\maketitle\n";

    os << "\\begin{center}\n";
    os << "\\section{Input NFA}\n";
    os << "\\includegraphics[width=\\textwidth]{" << main_pict_path << "}\n";
    os << "\\end{center}\n";

    if (!lambda_del_paths.empty())
    {
        os << "\\begin{center}\n\\section{Deleting lambda transitions}\n\\end{center}\n";
        os << "\\begin{enumerate}\n";
        for (int i = 0; i < lambda_del_paths.size(); ++i) {
            os << "\\item Step " + to_string(i + 1) + "\n";
            os << "\\begin{center}\n";
            os << "\\includegraphics[width=\\textwidth]{" << lambda_del_paths[i] << "}\n";
            os << "\\end{center}\n";
        }
        os << "\\end{enumerate}";
    }

    if (!conversion_paths.empty())
    {
        os << "\\begin{center}\n\\section{Determinating}\n\\end{center}\n";
        os << "\\begin{enumerate}\n";
        for (int i = 0; i < conversion_paths.size(); ++i) {
            os << "\\item Step " + to_string(i + 1) + "\n";
            os << "\\begin{center}\n";
            os << "\\includegraphics[width=\\textwidth]{" << conversion_paths[i] << "}\n";
            os << "\\end{center}\n";
        }
        os << "\\end{enumerate}";
    }

    os << "\\end{document}";
}
