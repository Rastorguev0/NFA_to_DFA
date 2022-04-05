#include <iostream>
#include <fstream>
#include <string>

#include "stream_io.h"
#include "fa.h"
#include "reporter.h"
#include "system.h"
#include "presenter.h"
#include "ut.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (!RunUnitTesting()) {
        cout << "Testing failed";
        return 0;
    }

    if (argc < 2) {
        cout << "Missing file name of NFA [parameter 1]. Terminating.";
    }
    else {
        ifstream in(argv[1]);
        if (!in) {
            cout << "Missing file: " << argv[1];
            return 0;
        }

        FSAReader reader;
        string line;
        while (getline(in,line)) {
            reader.ReadLine(move(line));
        }

        if (!reader.IsCorrect()) {
            cout << "Error while parsing the file " << argv[1];
            return 0;
        }

        FSA nfsa;
        for (const auto& con : reader.GetConnections()) {
            if (FSAReader::IsLambdaConnection(con))
                nfsa.AddTransition({ con.from, con.to, FSA::GetLambdaLabel() });
            else
                nfsa.AddTransition(con);
        }
        nfsa.AssignStart(reader.GetStartNode());
        for (const auto& term : reader.GetTermNodes()) {
            nfsa.AssignTerminal(term);
        }

        ConversionRecorder lrec;
        FSA nfsa_no_lambda = Converter::DeleteLambdaTransitions(nfsa, &lrec);

        ConversionRecorder crec;
        FSA dfsa = Converter::ConvertToDFA(nfsa_no_lambda, &crec);

        CSystem::PrepareOutDirectory();
        string base_nfsa_path;
        vector<string> lpaths;
        vector<string> cpaths;
        const string base_dot_filename = "step";
        {
            {
                base_nfsa_path = CSystem::CreateFileInOutputDir(base_dot_filename + "0.dot");
                ofstream of(base_nfsa_path);
                FSAPresenter::Present(nfsa, of);
            }

            FSA lbuild;
            for (int i = 0; i < lrec.StepsCount(); ++i)
            {
                lpaths.push_back(CSystem::CreateFileInOutputDir(base_dot_filename + to_string(i + 1) + ".dot"));
                ofstream of(lpaths.back());

                lbuild.AddTransition(lrec.GetStep(i));
                for (const auto& terminal : nfsa_no_lambda.GetTerminals()) {
                    lbuild.AssignTerminal(terminal);
                }
                lbuild.AssignStart(dfsa.GetStart());
                FSAPresenter::Present(lbuild, of);
            }

            FSA cbuild;
            for (int i = 0; i < crec.StepsCount(); ++i)
            {
                cpaths.push_back(CSystem::CreateFileInOutputDir(base_dot_filename + to_string(i + 1 + lrec.StepsCount()) + ".dot"));
                ofstream of(cpaths.back());

                cbuild.AddTransition(crec.GetStep(i));
                for (const auto& terminal : dfsa.GetTerminals()) {
                    cbuild.AssignTerminal(terminal);
                }
                cbuild.AssignStart(dfsa.GetStart());
                FSAPresenter::Present(cbuild, of);
            }
        }

        base_nfsa_path = CSystem::DotCompile(base_nfsa_path);
        base_nfsa_path.erase(0, CSystem::GetOutputDir().size() + 1);

        for (auto&& file : lpaths) {
            file = CSystem::DotCompile(file);
            file.erase(0, CSystem::GetOutputDir().size() + 1);
        }

        for (auto&& file : cpaths) {
            file = CSystem::DotCompile(file);
            file.erase(0, CSystem::GetOutputDir().size() + 1);
        }

        const string tex_result = "report.tex";
        ofstream result(CSystem::CreateFileInOutputDir(tex_result));
        TexReporter::MakeReport(result, base_nfsa_path, lpaths, cpaths);
    }

    return 0;
}