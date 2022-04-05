#include "presenter.h"
#include "stream_io.h"

#include <string>

using namespace std;

void FSAPresenter::Present(const FSA& fsa, std::ostream& os)
{
    DotWriter writer(os);
    writer.Begin();
    for (const string& terminal : fsa.GetTerminals()) {
        writer.WriteTerm(terminal);
    }
    writer.SetNodesShape();

    writer.StartNode(fsa.GetStart());
    auto connections = fsa.GetAllConnections();
    for (const auto& connection : connections) {
        if (connection.label == FSA::GetLambdaLabel())
            writer.WriteLambdaConnection(connection);
        else
            writer.WriteConnection(connection);
    }
    writer.End();
}
