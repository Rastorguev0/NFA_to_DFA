#pragma once
#include <string>
#include <vector>
#include <ostream>

#include "common.h"

class FSAReader
{
public:
    void ReadLine(std::string line);
    bool IsCorrect() const;

    static bool IsLambdaConnection(const Connection<std::string>& connection);

    const std::string& GetStartNode() const;
    const std::vector<std::string>& GetTermNodes() const;
    const std::vector<Connection<std::string>>& GetConnections() const;
private:
    static const std::string lambda;
    bool m_correct = true;
    std::vector<std::string> m_terms;
    std::vector<Connection<std::string>> m_con;
};


class DotWriter
{
public:
    DotWriter(std::ostream& o);
    void Begin();
    void WriteTerm(const std::string& name);
    void SetNodesShape();
    void StartNode(const std::string& name);
    void WriteConnection(const Connection<std::string>& con);
    void WriteLambdaConnection(const Connection<std::string>& con);
    void End();
private:
    std::ostream& os;
};