//
// Created by gorgr on 26.04.2020.
//

#include <sstream>
#include "Grammar.h"


bool Grammar::initFIRSTWithTerminalsAndEpsilon() {
    for (auto & rule : grammar) {
        set<string> firstSet;
        FIRSTForG.insert(pair<string, set<string>>(rule.first, firstSet));
        for (auto & right : rule.second) {
            for (string & symbol : right) {
                if (symbol == "e")
                    FIRSTForG[rule.first].insert(symbol);
                else {
                    if (grammar.count(symbol) <= 0) {
                        set<string> TerminalRightSet;
                        TerminalRightSet.insert(symbol);
                        FIRSTForG.insert(pair<string, set<string>>(symbol, TerminalRightSet));
                    }
                }
            }
        }
    }

    return true;
}

bool Grammar::initFIRSTWithNonTerminals() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto & rule : grammar) {
            for (auto & currRule : rule.second) {
                auto first = FIRST(currRule);
                for (auto & currFirst : first) {
                    if (FIRSTForG[rule.first].find(currFirst) == FIRSTForG[rule.first].end()) {
                        changed = true;
                        FIRSTForG[rule.first].insert(currFirst);
                    }
                }
            }
        }
    }

    return true;
}

bool Grammar::initFOLLOW() {
    for (auto & rule : grammar) {
        set<string> followSet;
        if (rule.first == "E")
            followSet.insert("$");
        FOLLOWForG.insert(pair<string, set<string>>(rule.first, followSet));
    }

    return true;
}

bool Grammar::calculateFOLLOW() {
    initFOLLOW();
//    for (auto & el : FOLLOWForG) {
//        cout << el.first << ": ";
//        for (auto & s : el.second)
//            cout << s << " ";
//        cout << endl;
//    }
//    cout << endl << endl;
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &rule : grammar) {
            for (auto &symbols : rule.second) {
                for (int i = 0; i < symbols.size(); ++i) {
                    if (grammar.count(symbols[i]) > 0) {
                        int old_size = FOLLOWForG[symbols[i]].size();
                        auto firstBeta = FIRST({symbols.begin() + i + 1, symbols.end()});
                        if (!firstBeta.empty()) {
                            for (auto &s : firstBeta) {
                                if (s != "e") {
                                    FOLLOWForG[symbols[i]].insert(s);
                                }
                            }
                        }
                        if (firstBeta.empty() || firstBeta.find("e") != firstBeta.end()) {
                            auto followA = FOLLOWForG[rule.first];
                            for (auto &s : followA) {
                                FOLLOWForG[symbols[i]].insert(s);
                            }
                        }
                        if (old_size < FOLLOWForG[symbols[i]].size()) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }

    return true;
}

Grammar::Grammar() {

}

Grammar::~Grammar() {

}

bool Grammar::loadGrammar(ifstream stream) {
    if (!stream || !stream.is_open())
        return false;
    string s;
    for(getline(stream, s); !stream.eof(); getline(stream, s)) {
        istringstream iss(s);
        vector<string> results((istream_iterator<string>(iss)),
                               istream_iterator<string>());
        string left = results[0];
        results.erase(results.begin(), results.begin()+2);
        auto rule = grammar.find(left);
        if (rule != grammar.end()){
            rule->second.push_back(results);
        } else {
            vector<vector<string>> right;
            right.push_back(results);
            grammar.insert(pair<string, vector<vector<string>>>(left, right));
        }
    }

    if (!initFIRSTWithTerminalsAndEpsilon()) return false;
    if (!initFIRSTWithNonTerminals()) return false;
    if (!calculateFOLLOW()) return false;
    if (!buildSATable()) return false;

    return true;
}

void Grammar::printFIRST(ostream &stream) {
    for (auto & symbol : FIRSTForG)
    {
        stream << symbol.first << ": [";
        for (const auto & element : symbol.second)
            stream << " " << element;
        cout << " ]" << endl;
    }
}
void Grammar::printFOLLOW(ostream &stream) {
    for (auto & nonTermFollow : FOLLOWForG) {
        cout << nonTermFollow.first << ": [ ";
        for (auto & setFollow : nonTermFollow.second)
            cout << setFollow << " ";
        cout << "]" << endl;
    }
}

set<string> Grammar::FIRST(const vector<string> & str) {
    set<string> result;
    if (str.size() == 1) {
        if (str[0] == "e")
            result.insert("e");
        else
            return FIRSTForG[str[0]];
    } else {
        for (auto & symbol : str) {
            auto first = FIRST({symbol});
            if (first.find("e") == first.end())
                return first;
            for (auto & elFirst : first) {
                if (elFirst == "e") {
                    break;
                }
                else {
                    result.insert(elFirst);
                }
            }
            if (symbol == str.back() && first.find("e") != first.end())
                result.insert("e");
        }
    }


    return result;
}

set<string> Grammar::FOLLOW(const string & nonTerminal) {
    return FOLLOWForG[nonTerminal];
}

bool Grammar::buildSATable() {
    for (auto & rule : grammar) {
        for (auto & right : rule.second) {
            auto firstRight = FIRST(right);
            for (auto & terminal : firstRight) {
                if (grammar.count(terminal) <= 0 && terminal != "e") {
                    if (SATable[rule.first].count(terminal) > 0)
                        return false;
                    SATable[rule.first][terminal] = "R1, R";
                }
            }
            if (firstRight.find("e") != firstRight.end()) {
                auto followRight = FOLLOW(rule.first);
                for (auto & symbol : followRight) {
                    SATable[rule.first][symbol] = "R1,R";
                }
            }
        }
    }
    for (auto & term : FIRSTForG) {
        if (grammar.count(term.first) <= 0) {
            SATable[term.first][term.first] = "P,A";
        }
    }
    SATable["$"]["$"] = "Accept";

    return true;
}

void Grammar::printSATable() {
    for (auto & term : SATable) {
        cout << term.first << " =   ";
        for (auto & a : term.second) {
            cout << "[ " << a.first << " - " << a.second << " ]";
        }
        cout << endl;
    }
}