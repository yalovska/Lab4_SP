#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>

using namespace std;

class FiniteAutomaton {
private:
    set<char> alphabet;                         // Алфавіт автомата
    set<int> states;                            // Множина станів
    int startState;                             // Початковий стан
    set<int> finalStates;                       // Фінальні стани
    map<pair<int, char>, set<int>> transitions; // Переходи

public:
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Не вдалося відкрити файл: " + filename);

        int alphabetSize, statesCount, finalStatesCount;
        file >> alphabetSize; // Потужність алфавіту
        for (int i = 0; i < alphabetSize; ++i) {
            char symbol;
            file >> symbol;
            alphabet.insert(symbol);
        }

        file >> statesCount; // Кількість станів
        for (int i = 0; i < statesCount; ++i) states.insert(i);

        file >> startState; // Початковий стан

        file >> finalStatesCount; // Кількість фінальних станів
        for (int i = 0; i < finalStatesCount; ++i) {
            int state;
            file >> state;
            finalStates.insert(state);
        }

        int from, to;
        char symbol;
        while (file >> from >> symbol >> to) {
            transitions[{from, symbol}].insert(to);
        }

        file.close();
    }

    bool analyzeWord(const string& word) {
        set<int> currentStates = {startState};

        for (char c : word) {
            if (alphabet.find(c) == alphabet.end()) return false; // Некоректний символ
            set<int> nextStates;
            for (int state : currentStates) {
                if (transitions.count({state, c})) {
                    nextStates.insert(transitions[{state, c}].begin(), transitions[{state, c}].end());
                }
            }
            currentStates = move(nextStates);
        }

        for (int state : currentStates) {
            if (finalStates.count(state)) return true;
        }
        return false;
    }

    void runInteractiveMode() {
        string word;
        cout << "Введіть слово для аналізу (або 'exit' для виходу):" << endl;
        while (true) {
            cout << "> ";
            cin >> word;
            if (word == "exit") break;
            if (analyzeWord(word)) {
                cout << "Слово приймається автоматом." << endl;
            } else {
                cout << "Слово не приймається автоматом." << endl;
            }
        }
    }
};

int main() {
    try {
        FiniteAutomaton automaton;
        automaton.loadFromFile("/Users/yanayalovska/CLionProjects/sp_lab4_18/automaton.txt");

        cout << "Автомат успішно завантажено. Запускаємо перевірку слів." << endl;
        automaton.runInteractiveMode();

    } catch (const exception& e) {
        cerr << "Помилка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
