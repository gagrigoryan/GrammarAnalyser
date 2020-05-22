#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <numeric>

using namespace std;

ostream & operator << (ostream &stream, const set<string> &terminals);

class Grammar{
private:
    map<string, vector<vector<string>>> grammar;
    map<string, set<string>> FIRSTForG;
    map<string, set<string>> FOLLOWForG;
    map<string, map<string, string>> SATable;
    vector<string> SA_ReverseWords;

    bool initFIRSTWithTerminalsAndEpsilon();
    bool initFIRSTWithNonTerminals();
    bool initFOLLOW();
    void initSA_Reverse_words();
    unsigned int indexSA_Reverse_words(vector<string> & word);
    bool calculateFOLLOW();
    bool buildSATable();

public:
    Grammar();
    ~Grammar();
    bool loadGrammar(ifstream stream);
    void printFIRST(ostream & stream);
    void printFOLLOW(ostream & stream);
    void printSATable();
    void printSAWords();
    bool parse(string expr);
    set<string> FIRST(const vector<string>&);
    set<string> FOLLOW(const string&);
};

ostream & operator << (ostream & stream, const Grammar &g);
vector<string> split(string & s);