#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


using namespace std;

class WordPlayer {
private:
    unordered_set<string> dictionary;

    bool isValidWord(const string &word, const string &letterBank) const {
        string bank = letterBank;
        for (char c: word) {
            auto pos = bank.find(c);
            if (pos == string::npos) {
                return false;
            }
            bank.erase(pos, 1);
        }
        return true;
    }

public:
    void loadDictionary(istream &dictionaryStream) {
        string word;
        while (dictionaryStream >> word) {
            dictionary.insert(word);
        }
    }

    vector<string> playWords(const string &letterBank) {
        vector<string> validWords;
        for (auto const &entry: dictionary) {
            string const &word = entry;
            bool f = word.length() > 3;
            if (f and isValidWord(word, letterBank)) {
                validWords.push_back(word);
            }
        }

        sort(validWords.begin(), validWords.end());
        return validWords;
    }

    void justDoIt() {
        // Read the letter bank from input
        std::cout << "Please enter the letter bank of known letters" << std::endl;
        string letterBank;
        cin >> letterBank;

        // Play words based on the letter bank
        vector<string> validWords = this->playWords(letterBank);

        // Print the valid words
        std::cout << "VALID WORDS" << std::endl;
        for (const string &word: validWords) {
            cout << word << endl;
        }
    }
};

int main() {
    // Create a WordPlayer object
    WordPlayer player;

    std::ifstream is("words.txt");
    // Read the dictionary from input redirection
    player.loadDictionary(is);
    std::cout << "Dictionary is now filled" << std::endl;

    for (;;) {
        std::string temp;
        std::cout << "Enter $ to quit or anything else to play" << std::endl;
        std::getline(std::cin, temp);
        if (temp == "$") {
            break;
        }
        player.justDoIt();
    }

    return 0;
}
