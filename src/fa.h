#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>

#include "common.h"

class Converter;

class FSA
{
public:
    bool AddTransition(const Connection<std::string>& c);
    bool AssignTerminal(const std::string& term);
    bool AssignStart(const std::string& start);

    static const std::string& GetLambdaLabel();

    const std::set<std::string>& GetAlphabet() const;
    std::unordered_set<std::string> GetTerminals() const;
    std::string GetStart() const;
    std::vector<Connection<std::string>> GetConnectionsFrom(const std::string& from) const;
    std::vector<Connection<std::string>> GetAllConnections() const;
private:
    friend Converter;

    static const std::string lambda;

    // alphabet
    std::set<std::string> m_abc;
    using label = const std::string*;

    // states
    std::set<std::string>  m_states;
    using state = const std::string*;

    // start and terminal nodes
    std::string m_start;
    std::unordered_set<state> m_terminals;

    // transitions
        // where do the transitions lead to from [key]
        std::unordered_map<state, std::unordered_map<label, std::unordered_set<state>>> m_transitions_from;
        // where do the transitions lead from to [key]
        std::unordered_map<state, std::unordered_map<label, std::unordered_set<state>>> m_transitions_to;
    //

    std::vector<Connection<std::string>> GetConnectionsFrom(state from) const;
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
    static FSA ConvertToDFA(const FSA& NFSA, ConversionRecorder* recorder = nullptr);
    static FSA DeleteLambdaTransitions(const FSA& NFSA, ConversionRecorder* recorder = nullptr);
private:
    static void CollectTransitions(const FSA& fsa, FSA& res, FSA::state from, FSA::state start, FSA::label lambda, ConversionRecorder* rec);
};