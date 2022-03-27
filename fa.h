#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "common.h"

class Converter;

class FA
{
public:
    bool AddTransition(const Connection<std::string>& c);
    bool AssignTerminal(const std::string& term);
    bool AssignStart(const std::string& start);

    const std::unordered_set<std::string>& GetAlphabet() const;
    const std::unordered_set<std::string>& GetTerminals() const;
    std::string GetStart() const;
    std::vector<Connection<std::string>> GetConnectionsFrom(const std::string& from) const;
    std::vector<Connection<std::string>> GetAllConnections() const;

private:
    friend Converter;

    unsigned MaybeAddNode(const std::string& node_name);

    std::vector<std::vector<unsigned>> m_connection_data;
    std::vector<std::vector<std::string>> m_label_data;

    // mapping for node names to vector indexes
    std::unordered_map<std::string, unsigned> m_mapping;
    std::unordered_map<unsigned, std::string> m_rmapping;

    // alphabet
    std::unordered_set<std::string> m_abc;

    // start and terminal nodes
    unsigned m_s;
    std::unordered_set<std::string> m_t;
};

class ConversionRecorder {
public:
    void AddStep(const Connection<std::string>& connection);

    const Connection<std::string>& GetStep(unsigned i) const;
    unsigned StepsCount() const;
private:
    // 1 step == 1 connection
    std::vector<Connection<std::string>> m_steps;
};

class Converter
{
public:
    static FA ConvertToDFA(const FA& NFA, ConversionRecorder* recorder = nullptr);
};