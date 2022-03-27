#include <iostream>
#include <fstream>
#include <string>

#include "stream_io.h"
#include "fa.h"
#include "reporter.h"
#include "system.h"
#include "presenter.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Missing file name of NFA [parameter 1]. Terminating.";
    }
    else {
        ifstream in(argv[1]);
        if (!in) {
            cout << "Missing file: " << argv[1];
            return 0;
        }

        FAReader reader;
        string line;
        while (getline(in,line)) {
            reader.ReadLine(move(line));
        }

        if (!reader.IsCorrect()) {
            cout << "Error while parsing the file " << argv[1];
            return 0;
        }

        FA nfa;
        for (const auto& con : reader.GetConnections()) {
            nfa.AddTransition(con);
        }
        nfa.AssignStart(reader.GetStartNode());
        for (const auto& term : reader.GetTermNodes()) {
            nfa.AssignTerminal(term);
        }

        ConversionRecorder rec;
        FA dfa = Converter::ConvertToDFA(nfa, &rec);

        CSystem::PrepareOutDirectory();
        vector<string> filenames;
        const string base_dot_filename = "step";
        {
            {
                filenames.push_back(CSystem::CreateFileInOutputDir(base_dot_filename + "0.dot"));
                ofstream of(filenames.back());
                FAPresenter::Present(nfa, of);
            }

            FA build;
            for (int i = 0; i < rec.StepsCount(); ++i)
            {
                filenames.push_back(CSystem::CreateFileInOutputDir(base_dot_filename + to_string(i + 1) + ".dot"));
                ofstream of(filenames.back());

                build.AddTransition(rec.GetStep(i));
                for (const auto& terminal : dfa.GetTerminals()) {
                    build.AssignTerminal(terminal);
                }
                FAPresenter::Present(build, of);
            }
        }

        for (auto&& file : filenames) {
            file = CSystem::DotCompile(file);
            file.erase(0, CSystem::GetOutputDir().size() + 1);
        }

        const string tex_result = "report.tex";
        ofstream result(CSystem::CreateFileInOutputDir(tex_result));
        TexReporter::MakeReport(result, filenames);
    }

    return 0;
}