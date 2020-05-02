#include <iostream>
#include <iterator>
#include <sstream>
#include "Grammar.h"

using namespace std;

int main() {
    Grammar g = Grammar();
    g.loadGrammar(ifstream("C:/Users/gorgr/Desktop/ProgUniver/Grammar/grammar.txt"));
//    g.printFIRST(cout);
//    cout << endl << endl;
//    g.printFOLLOW(cout);

    g.printSATable();

    return 0;
}
