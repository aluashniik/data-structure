#include <iostream>
#include <vector>
#include <string>
using namespace std;

void PowerSet(int setSize, int indx, string sub, char set[]) {
    if (indx == setSize) {
        cout << "{" << sub << "}" << endl;
        return;
    }
    
    PowerSet(setSize, indx + 1, sub, set);
    
    string next = sub;
    if (!sub.empty()) {
        next += " ";
    }
    next += set[indx];
    PowerSet(setSize, indx + 1, next, set);
}

int main() {
    int setSize;
    cout << "Enter the size of the set: ";
    cin >> setSize;

    char set[26]; 
    for (int i = 0; i < setSize; ++i) {
        set[i] = 'a' + i; 
    }

    PowerSet(setSize, 0, "", set);

    return 0;
}
