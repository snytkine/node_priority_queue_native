#include <iostream>

#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <functional>

using namespace std;

void Push(vector<int>& heap, int val) {
    heap.push_back(val);
    make_heap(heap.begin(), heap.end(), [](int &lhs, int &rhs){return lhs > rhs;});
    //push_heap(heap.begin(), heap.end()); //, greater<int>()
}
int Pop(vector<int>& heap) {
    int val = heap.front();

    //This operation will move the smallest element to the end of the vector
    pop_heap(heap.begin(), heap.end(), greater<int>());

    //Remove the last element from vector, which is the smallest element
    heap.pop_back();
    return val;
}
int main() {
    vector<int> heap;
    Push(heap, 1);
    Push(heap, 7);
    Push(heap, 5);
    Push(heap, 25);
    Push(heap, 51);
    Push(heap, 3);

    while (!heap.empty()) {
        cout << Pop(heap) << endl;
    }
    return 0;
}