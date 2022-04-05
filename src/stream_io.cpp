#include <sstream>
#include <string>

#include "stream_io.h"

using namespace std;

const string FSAReader::lambda = "\\lambda";

void FSAReader::ReadLine(std::string line)
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

bool FSAReader::IsCorrect() const
{
    return m_correct;
}

const std::string& FSAReader::GetStartNode() const
{
    return m_con.front().from;
}

const std::vector<std::string>& FSAReader::GetTermNodes() const
{
    return m_terms;
}

bool FSAReader::IsLambdaConnection(const Connection<std::string>& connection)
{
    return connection.label == lambda;
}

const std::vector<Connection<string>>& FSAReader::GetConnections() const
{
    return m_con;
}

DotWriter::DotWriter(std::ostream& o) : os(o)
{
}

void DotWriter::Begin()
{
    os << "digraph fa {\n";
    os << "fontname = \"Helvetica,Arial,sans-serif\";\n";
    os << "node[fontname = \"Helvetica,Arial,sans-serif\"];\n";
    os << "edge[fontname = \"Helvetica,Arial,sans-serif\"];\n";
    os << "rankdir = LR;\n";
    os << "node [shape = none]; \" \";\n";
}

void DotWriter::WriteTerm(const std::string& name)
{
    os << "node [shape = doublecircle]; \"" << name << "\";\n";
}

void DotWriter::SetNodesShape()
{
    os << "node [shape = circle];\n";
}

void DotWriter::StartNode(const std::string& name)
{
    os << "\" \" -> \"" << name << "\";\n";
}

void DotWriter::WriteConnection(const Connection<std::string>& con)
{
    os << "\"" << con.from << "\" -> " << "\"" << con.to << "\" [label = \"" << con.label << "\"];\n";
}

void DotWriter::End()
{
    os << "}";
}

void DotWriter::WriteLambdaConnection(const Connection<std::string>& con)
{
    os << "\"" << con.from << "\" -> " << "\"" << con.to << "\" [label = <&lambda;>];\n";
}
