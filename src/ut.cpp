#include "common.h"
#include "fa.h"
#include "presenter.h"
#include "reporter.h"
#include "stream_io.h"
#include "ut.h"

#include <string>
#include <set>
#include <unordered_set>
#include <vector>

using namespace std;

#define ASSERT(expression) if (!(expression)) throw exception(to_string(__LINE__).c_str());

bool CommonTest()
{
    {
        set<string> test = { "" };
        ASSERT(GroupStrings(test) == "");
    }
    {
        set<string> test = { "a", "2" };
        ASSERT(GroupStrings(test) == "2, a");
    }
    {
        unordered_set<string> test = { "a", "bb", " " };
        ASSERT(GroupStrings(test) == "a, bb,  ");
    }
    {
        vector<string> test = { "a", "2", "44", "1" };
        ASSERT(GroupStrings(test) == "a, 2, 44, 1");
    }

    return true;
}

struct ConnectionHasher {
    size_t operator()(const Connection<string>& c) const {
        static hash<string> hasher;
        return pow(hasher(c.from), 3) + pow(hasher(c.to), 2) + hasher(c.label);
    }
};

bool operator==(const Connection<string>& one, const Connection<string>& two) {
    return make_tuple(one.from, one.to, one.label) == make_tuple(two.from, two.to, two.label);
}

bool FATest()
{
    FSA fa;
    {
        ASSERT(!fa.AssignStart("start"));
        ASSERT(!fa.AssignStart("0"));
        ASSERT(!fa.AssignTerminal("terminal"));
        ASSERT(!fa.AssignTerminal("0"));
    }
    {
        ASSERT(fa.AddTransition({ "0", "1", "a" }));
        ASSERT(fa.AssignStart("0"));
        ASSERT(fa.AssignTerminal("1"));
        ASSERT(fa.GetAlphabet() == set<string>{"a"});
        ASSERT(fa.GetStart() == "0");
        ASSERT(fa.GetTerminals() == unordered_set<string>{"1"});

        ASSERT(!fa.AssignTerminal("2"));
    }
    {
        ASSERT(fa.AddTransition({ "0", "zero", "" }));
        ASSERT(fa.AddTransition({ "zero", "one", "a a a" }));
        ASSERT(fa.AssignTerminal("one"));
        
        auto hash = [](const Connection<string>& con) {
            return true;
        };
        unordered_set<Connection<string>, ConnectionHasher> assert = { {"0", "1", "a"}, { "0", "zero", "" }, { "zero", "one", "a a a" } };
        const auto connections = fa.GetAllConnections();
        ASSERT(assert.size() == connections.size());
        for (int i = 0; i < connections.size(); ++i) {
            //ASSERT(connections[i].from == assert[i].from);
            //ASSERT(connections[i].to == assert[i].to);
            //ASSERT(connections[i].label == assert[i].label);
        }
     }

    return true;
}

bool RunUnitTesting()
{
    return CommonTest() && FATest();
}
