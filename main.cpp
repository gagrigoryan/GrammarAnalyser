#include <iostream>
#include <iterator>
#include <sstream>
#include "Grammar.h"

using namespace std;

int main() {
    Grammar g = Grammar();
    g.loadGrammar(ifstream("D:/univer/ProgUniver/Grammar/grammar.txt"));
    g.printFIRST(cout);
    cout << endl << endl;
    g.printFOLLOW(cout);
    cout << endl << endl;
    g.printSATable();

    cout << g.parse("id * id + id");

    return 0;
}
