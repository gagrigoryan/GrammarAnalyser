#include <iostream>
#include <iterator>
#include <sstream>
#include "Grammar.h"

using namespace std;

int main() {
    Grammar g = Grammar();
    g.loadGrammar(ifstream("D:/univer/ProgUniver/Grammar/grammar.txt"));

    return 0;
}
