
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

class node {
public:
    string name;
    double priority;

    node(double a, string b) : priority(a), name(b) {}
};
// can have constructor so that obj holds *isolate
class CompareNode {
public:
    CompareNode(){}
    bool operator()(const node &n1, const node &n2) {
        return n1.priority < n2.priority;
    }
};


class ReverseCompareNode: public CompareNode {
public:
     bool operator()(const node &n1, const node &n2) {
        return n1.priority > n2.priority;
    }
};


class Widget {
public:
    int getId() {
        return 50;
    }
};

int main() {


    auto w = new Widget();


    typedef priority_queue<node, vector<node>, CompareNode> my_pq;

    typedef priority_queue<node, vector<node>, std::function<bool (node, node)>> XPG;
    //priority_queue<node, vector<node>, ReverseCompareNode> pq;
    //function<bool>

   // my_pq pq = my_pq(ReverseCompareNode());

    // what is this auto?
    auto compare = [&](const node &lhs, const node &rhs) -> bool { return lhs.priority + w->getId() < rhs.priority + w->getId(); };


    auto compare_rev = [&](const node &lhs, const node &rhs) -> bool { return lhs.priority + w->getId() > rhs.priority + w->getId(); };

    XPG pq(compare);


    node d(100, "pay bills");
    node b(53, "buy a book");
    node a(22, "eat lunch");
    node c(27, "do homework");

    pq.push(b);
    pq.push(a);
    pq.push(c);
    pq.push(d);
    pq.emplace(102, "Study");
    pq.emplace(7, "Wash the dog");

    /*int size = pq.size();
    for (int i = 0; i < size; ++i) {
        cout << pq.top().priority << " " << pq.top().name << "\n";
        pq.pop();
    }*/

    while (!pq.empty()) {
        std::cout << pq.top().priority << ":" << pq.top().name << std::endl;
        pq.pop();
    }
}