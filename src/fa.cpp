#include "fa.h"
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

const string FSA::lambda = "\\lambda";

bool FSA::AddTransition(const Connection<std::string>& c)
{
    const auto label = &(*m_abc.insert(c.label).first);
    const auto from = &(*m_states.insert(c.from).first);
    const auto to = &(*m_states.insert(c.to).first);

    m_transitions_from[from][label].insert(to);
    m_transitions_to[to][label].insert(from);

    return true;
}

bool FSA::AssignTerminal(const std::string& term)
{
    auto state = m_states.find(term);

    if (state != m_states.end()) {
        m_terminals.insert(&(*state));
        return true;
    }
    else return false;
}

bool FSA::AssignStart(const std::string& start)
{
    if (m_states.contains(start)) {
        m_start = start;
        return true;
    }
    else return false;
}

const std::string& FSA::GetLambdaLabel()
{
    return lambda;
}

const std::set<std::string>& FSA::GetAlphabet() const
{
    return m_abc;
}

std::unordered_set<std::string> FSA::GetTerminals() const
{
    unordered_set<string> result;
    for (const auto terminal : m_terminals) {
        result.insert(*terminal);
    }
    return result;
}

std::string FSA::GetStart() const
{
    return m_start;
}

std::vector<Connection<std::string>> FSA::GetConnectionsFrom(const std::string& from) const
{
    vector<Connection<string>> result;
    auto from_it = m_states.find(from);
    if (from_it != m_states.end() && m_transitions_from.contains(&(*from_it))) {
        result = GetConnectionsFrom(&(*from_it));
    }
    return result;
}

std::vector<Connection<std::string>> FSA::GetAllConnections() const
{
    vector<Connection<string>> result;
    for (const auto& [state_from, _] : m_transitions_from) {
        vector<Connection<string>> one = GetConnectionsFrom(state_from);
        result.insert(result.end(), make_move_iterator(one.begin()), make_move_iterator(one.end()));
    }
    return result;
}

std::vector<Connection<std::string>> FSA::GetConnectionsFrom(state from) const
{
    vector<Connection<string>> result;
    unordered_map<state, unordered_set<label>> connections;
    for (const auto& [label, states] : m_transitions_from.at(from)) {
        for (const auto state : states) {
            connections[state].insert(label);
        }
    }
    for (const auto& [to, labels] : connections) {
        result.push_back({ *from, *to, GroupStringPtrs(labels) });
    }
    return result;
}


FSA Converter::ConvertToDFA(const FSA& NFSA, ConversionRecorder* recorder)
{
    FSA DFSA;
    queue<unordered_set<FSA::state>> Q;
    unordered_set<string>            Q_history;

    Q.push({ &(*NFSA.m_states.find(NFSA.m_start)) });
    while (!Q.empty())
    {
        unordered_set<FSA::state> q = Q.front(); Q.pop();
        const string              q_name = GroupStringPtrs(q);

        for (const auto state : q)
        {
            if (NFSA.m_terminals.contains(state))
            {
                DFSA.AssignTerminal(q_name);
                break;
            }
        }

        for (auto _label = NFSA.m_abc.begin(); _label != NFSA.m_abc.end(); _label = next(_label))
        {
            FSA::label label = &(*_label);
            unordered_set<FSA::state> q_next;

            for (const FSA::state state : q)
            {
                if (NFSA.m_transitions_from.contains(state))
                {
                    if (NFSA.m_transitions_from.at(state).contains(label))
                    {
                        for (const auto next : NFSA.m_transitions_from.at(state).at(label))
                            q_next.insert(next);
                    }
                }
            }

            if (q_next.empty()) continue;

            string q_next_name = GroupStringPtrs(q_next);
            Connection c = { q_name, q_next_name, *label };
            DFSA.AddTransition(c);
            if (recorder) {
                recorder->AddStep(c);
            }

            if (!Q_history.contains(q_next_name))
            {
                Q.push(move(q_next));
                Q_history.insert(q_next_name);
            }
        }
    }

    DFSA.AssignStart(NFSA.m_start);

    return DFSA;
}

void Converter::CollectTransitions(const FSA& fsa, FSA& result, FSA::state from, FSA::state start, FSA::label lambda, ConversionRecorder* rec)
{
    if (!fsa.m_transitions_from.contains(start)) return;
    for (const auto& [label, connections_to] : fsa.m_transitions_from.at(start))
    {
        if (label != lambda)
        {
            for (const auto to : connections_to)
            {
                Connection c = { *from, *to, *label };
                if (rec)
                    rec->AddStep(c);
                result.AddTransition(move(c));
            }
        }
        else
        {
            for (const auto to : connections_to)
            {
                if (start != to)
                    CollectTransitions(fsa, result, from, to, lambda, rec);

                if (fsa.m_terminals.contains(to))
                    result.AssignTerminal(*from);
            }
        }
    }
}


FSA Converter::DeleteLambdaTransitions(const FSA& fsa, ConversionRecorder* rec)
{
    auto lambda_it = fsa.m_abc.find(FSA::GetLambdaLabel());
    if (lambda_it == fsa.m_abc.end())
        return fsa;
    const FSA::label lambda = &(*lambda_it);

    FSA result;

    for (const auto& [from, transitions] : fsa.m_transitions_from)
    {
        if (fsa.m_transitions_to.contains(from)
            && fsa.m_transitions_to.at(from).size() == 1
            && fsa.m_transitions_to.at(from).contains(lambda)
            && *from != fsa.m_start)
            continue;

        CollectTransitions(fsa, result, from, from, lambda, rec);
    }

    result.AssignStart(fsa.m_start);
    for (const auto terminal : fsa.m_terminals)
        result.AssignTerminal(*terminal);

    return result;
}

void ConversionRecorder::AddStep(const Connection<std::string>& node_name)
{
    m_steps.push_back(node_name);
}

const Connection<std::string>& ConversionRecorder::GetStep(unsigned i) const
{
    return m_steps[i];
}

unsigned ConversionRecorder::StepsCount() const
{
    return m_steps.size();
}
