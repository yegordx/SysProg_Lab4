#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <stdexcept>
#include <string>
using namespace std;

struct FiniteAutomaton {
    set<char> alphabet;                  
    set<int> states;                    
    int initialState;                    
    set<int> finalStates;                 
    map<pair<int, char>, int> transitions;
};

FiniteAutomaton loadAutomaton(const string& filename) {
    FiniteAutomaton automaton;
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Unable to open file.");
    }

    int wordLength, numStates, numFinalStates;
    file >> wordLength; 
    file >> numStates;  

    string alphabetStr;
    file >> alphabetStr;
    for (char symbol : alphabetStr) {
        automaton.alphabet.insert(symbol);
    }

    for (int i = 0; i < numStates; ++i) {
        int state;
        file >> state;
        automaton.states.insert(state);
    }

    file >> automaton.initialState; 

    file >> numFinalStates;
    for (int i = 0; i < numFinalStates; ++i) {
        int state;
        file >> state;
        automaton.finalStates.insert(state);
    }

    int fromState, toState;
    char symbol;
    while (file >> fromState >> symbol >> toState) {
        automaton.transitions[{fromState, symbol}] = toState;
    }

    file.close();
    return automaton;
}

// ���� �������� � �������
void printAutomaton(const FiniteAutomaton& automaton) {
    cout << "Finite Automaton:" << endl;

    // �������� ������
    cout << "States: ";
    for (int state : automaton.states) {
        cout << state << " ";
    }
    cout << endl;

    // ���������� ����
    cout << "Initial State: " << automaton.initialState << endl;

    // Գ����� �����
    cout << "Final States: ";
    for (int state : automaton.finalStates) {
        cout << state << " ";
    }
    cout << endl;

    // ������
    cout << "Alphabet: ";
    for (char symbol : automaton.alphabet) {
        cout << symbol << " ";
    }
    cout << endl;

    // ��������
    cout << "Transitions:" << endl;
    for (const auto& entry : automaton.transitions) {
        const auto& key = entry.first;  // ���� � ���� (fromState, symbol)
        int value = entry.second;      // �������� � toState
        cout << "  (" << key.first << ", '" << key.second << "') -> " << value << endl;
    }
    cout << endl;
}

bool processWord(const FiniteAutomaton& automaton, const string& word) {
    int currentState = automaton.initialState;

    for (char symbol : word) {
        if (automaton.transitions.find({ currentState, symbol }) != automaton.transitions.end()) {
            currentState = automaton.transitions.at({ currentState, symbol });
        }
        else {
            return false; // ������� �� ��������
        }
    }

    return automaton.finalStates.find(currentState) != automaton.finalStates.end();
}

set<char> findUnacceptedSymbols(const FiniteAutomaton& automaton) {
    set<char> acceptedSymbols;

    // ������ �� �������, ��� ���� � ��������
    for (const auto& entry : automaton.transitions) {
        char symbol = entry.first.second; // entry.first - �� ���� (fromState, symbol), ������ ������
        acceptedSymbols.insert(symbol);   // ������ ������ � acceptedSymbols
    }

    set<char> unacceptedSymbols;
    // ��������� �� ��� �������� ������� �� ����������
    for (char symbol : automaton.alphabet) {
        if (acceptedSymbols.find(symbol) == acceptedSymbols.end()) {
            unacceptedSymbols.insert(symbol); // ���� ������ ������� � ���������, ������ �� ������������
        }
    }

    return unacceptedSymbols;
}

int main() {
    try {
        // ���������� ��������
        string filename = "Auto1.txt";
        /*cout << "Enter automaton file name: ";
        cin >> filename;*/

        FiniteAutomaton automaton = loadAutomaton(filename);

        // ���� �������� � �������
        printAutomaton(automaton);

        set<char> unacceptedSymbols = findUnacceptedSymbols(automaton);

        // ������� ���������� �������
        if (unacceptedSymbols.empty()) {
            cout << "All symbols in the alphabet are accepted by the automaton." << endl;
        }
        else {
            cout << "Unaccepted symbols: ";
            for (char symbol : unacceptedSymbols) {
                cout << symbol << " ";
            }
            cout << endl;
        }

        string word;
        cout << "Enter the word: ";
        cin >> word;

        if (processWord(automaton, word)) {
            cout << "Automaton accepted the word." << endl;
        }
        else {
            cout << "Automaton did NOT accept the word." << endl;
        }
    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}
