#pragma once
#include <string>
#include <vector>
#include <ostream>

#include "common.h"

class FAReader
{
public:
    void ReadLine(std::string line);
    bool IsCorrect() const;

    const std::string& GetStartNode() const;
    const std::vector<std::string>& GetTermNodes() const;
    const std::vector<Connection<std::string>>& GetConnections() const;
private:

    bool m_correct = true;
    std::vector<std::string> m_terms;
    std::vector<Connection<std::string>> m_con;
};


class DotWriter
{
public:
    static void Begin(std::ostream& os);
    static void WriteTerm(std::ostream& os, const std::string& name);
    static void SetNodesShape(std::ostream& os);
    static void StartNode(std::ostream& os, const std::string& name);
    static void WriteConnection(std::ostream& os, const Connection<std::string>& con);
    static void End(std::ostream& os);
};