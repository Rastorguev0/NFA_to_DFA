#include "fa.h"
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

bool FA::AddTransition(const Connection<string>& c)
{
    unsigned from = MaybeAddNode(c.from);
    unsigned to = MaybeAddNode(c.to);
    m_connection_data[from].push_back(to);
    m_label_data[from].push_back(c.label);
    m_abc.insert(c.label);
    return true;
}

bool FA::AssignTerminal(const std::string& term)
{
    if (m_mapping.contains(term)) {
        m_t.insert(term);
        return true;
    }
    return false;
}

bool FA::AssignStart(const std::string& start)
{
    if (m_mapping.contains(start)) {
        m_s = m_mapping.at(start);
        return true;
    }
    return false;
}

const std::unordered_set<std::string>& FA::GetAlphabet() const
{
    return m_abc;
}

const std::unordered_set<std::string>& FA::GetTerminals() const
{
    return m_t;
}

string FA::GetStart() const
{
    return m_rmapping.at(m_s);
}

std::vector<Connection<std::string>> FA::GetConnectionsFrom(const std::string& from) const
{
    if (!m_mapping.contains(from)) return {};
    
    vector<Connection<string>> result;
    unsigned from_u = m_mapping.at(from);
    for (int i = 0; i < m_connection_data[from_u].size(); ++i) {
        const string& to = m_rmapping.at(m_connection_data[from_u][i]);
        result.push_back({ from, to, m_label_data[from_u][i] });
    }
    return result;
}

std::vector<Connection<std::string>> FA::GetAllConnections() const
{
    vector<Connection<string>> result;
    for (int i = 0; i < m_connection_data.size(); ++i) {
        unordered_map<unsigned, set<string>> to_labels;
        for (int j = 0; j < m_connection_data[i].size(); ++j) {
            to_labels[m_connection_data[i][j]].insert(m_label_data[i][j]);
        }
        for (const auto& [to, label_set] : to_labels) {
            result.push_back({ m_rmapping.at(i), m_rmapping.at(to), GroupStrings(label_set) });
        }
    }
    return result;
}

unsigned FA::MaybeAddNode(const std::string& node_name)
{
    if (!m_mapping.contains(node_name)) {
        m_mapping[node_name] = m_connection_data.size();
        m_rmapping[m_connection_data.size()] = node_name;
        m_connection_data.push_back({});
        m_label_data.push_back({});
        return m_mapping.at(node_name);
    }
    else return m_mapping.at(node_name);
}

FA Converter::ConvertToDFA(const FA& NFA, ConversionRecorder* recorder)
{
    queue<unordered_set<string>> Q;
    unordered_set<string> Q_history;
    const unordered_set<string> terminals = NFA.GetTerminals();
    FA DFA;
    Q.push({ NFA.m_rmapping.at(NFA.m_s) });
    while (!Q.empty())
    {
        unordered_set<string> q = Q.front();
        Q.pop();
        string q_node_name = GroupStrings(q);

        for (const string& str : q) {
            if (NFA.GetTerminals().contains(str)) {
                DFA.AssignTerminal(q_node_name);
                break;
            }
        }

        Q_history.insert(q_node_name);

        for (const auto& a : NFA.m_abc)
        {
            unordered_set<string> next_q;
            for (const string& node : q)
            {
                for (int i = 0; i < NFA.m_connection_data[NFA.m_mapping.at(node)].size(); ++i)
                {
                    if (NFA.m_label_data[NFA.m_mapping.at(node)][i] == a)
                        next_q.insert(NFA.m_rmapping.at(NFA.m_connection_data[NFA.m_mapping.at(node)][i]));
                }
            }

            string next_q_node_name = GroupStrings(next_q);
            Connection<string> c = { q_node_name, next_q_node_name, a };
            DFA.AddTransition(c);
            if (recorder) {
                recorder->AddStep(c);
            }

            if (!Q_history.contains(next_q_node_name))
                Q.push(move(next_q));
        }
    }
    DFA.AssignStart(NFA.GetStart());
    return DFA;
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
