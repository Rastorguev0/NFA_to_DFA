#include <sstream>
#include <string>

#include "stream_io.h"

using namespace std;


void FAReader::ReadLine(std::string line)
{
    if (line.empty() || !m_correct) return;
    
    istringstream is(move(line));
    string type;
    is >> type;
    if (type == "term") {
        string term;
        while (is >> term) {
            m_terms.push_back(move(term));
        }
    }
    else if (type == "path") {
        Connection<string> con;
        is >> con.from;
        is >> con.label;
        m_correct = is.good();
        is >> con.to;
        m_con.push_back(move(con));
    }
    else m_correct = false;
}

bool FAReader::IsCorrect() const
{
    return m_correct;
}

const std::string& FAReader::GetStartNode() const
{
    return m_con.front().from;
}

const std::vector<std::string>& FAReader::GetTermNodes() const
{
    return m_terms;
}

const std::vector<Connection<string>>& FAReader::GetConnections() const
{
    return m_con;
}

void DotWriter::Begin(std::ostream& os)
{
    os << "digraph fa {\n";
    os << "fontname = \"Helvetica,Arial,sans-serif\";\n";
    os << "node[fontname = \"Helvetica,Arial,sans-serif\"];\n";
    os << "edge[fontname = \"Helvetica,Arial,sans-serif\"];\n";
    os << "rankdir = LR;\n";
    os << "node [shape = none]; \" \";\n";
}

void DotWriter::WriteTerm(std::ostream& os, const std::string& name)
{
    os << "node [shape = doublecircle]; \"" << name << "\";\n";
}

void DotWriter::SetNodesShape(std::ostream& os)
{
    os << "node [shape = circle];\n";
}

void DotWriter::StartNode(std::ostream& os, const std::string& name)
{
    os << "\" \" -> \"" << name << "\";\n";
}

void DotWriter::WriteConnection(std::ostream& os, const Connection<std::string>& con)
{
    os << "\"" << con.from << "\" -> " << "\"" << con.to << "\" [label = \"" << con.label << "\"];\n";
}

void DotWriter::End(std::ostream& os)
{
    os << "}";
}
