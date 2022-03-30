#include "presenter.h"

#include <string>

using namespace std;

void FAPresenter::Present(const FA& fa, std::ostream& os)
{
    DotWriter::Begin(os);
    for (const string& terminal : fa.GetTerminals()) {
        DotWriter::WriteTerm(os, terminal);
    }
    DotWriter::SetNodesShape(os);

    DotWriter::StartNode(os, fa.GetStart());
    auto connections = fa.GetAllConnections();
    for (const auto& connection : connections) {
        DotWriter::WriteConnection(os, connection);
    }
    DotWriter::End(os);
}
