#pragma once
#include <string>
#include <unordered_set>
#include <set>

template<typename T>
struct Connection {
    T from;
    T to;
    std::string label;
};

template<typename RangeBasedIterable>
std::string GroupStrings(const RangeBasedIterable& group)
{
    if (group.empty()) return "";

    std::string result = "";
    for (const auto str : group) {
        result += str + ", ";
    }
    result.pop_back();
    result.pop_back();
    return result;
}

template<typename RangeBasedIterable>
std::string GroupStringPtrs(const RangeBasedIterable& group)
{
    std::set<std::string> group_set;
    for (const auto& item : group) {
        group_set.insert(*item);
    }
    return GroupStrings(group_set);
}