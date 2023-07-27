#include <algorithm>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

class WordPlayer {
private:
    vector<string> dictionary;
    int minLength = 4;
    bool missing = false;
    bool fake = false;
    bool toSort = false;
    std::vector<uint32_t> letterCount;

    vector<string> playWords(const string &letterBank) {
        vector<string> validWords;
        for (string const &word: dictionary) {
            bool f = word.length() >= minLength;
            if (f and isValidWord(word, letterBank)) {
                validWords.push_back(word);
            }
        }

        sort(validWords.begin(), validWords.end());
        return validWords;
    }
    static bool isValidWord(const string &word, const string &letterBank) {
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

    static inline int c2i(char c) {
        return static_cast<int>(c - 'a');
    }

    int numWordsValid(std::string const &letterBank) {
        std::vector<std::string> validWords = this->playWords(letterBank);
        return static_cast<int>(validWords.size());
    }

public:
    void init(int argc, char **argv) {
        int optionIndex = 0;
        int option = 0;
        struct option longOpts[] = {{"length", required_argument, nullptr, 'l'},
                                    {"missing", no_argument, nullptr, 'm'},
                                    {"fake", no_argument, nullptr, 'f'},
                                    {"sort", no_argument, nullptr, 's'},
                                    {nullptr, 0, nullptr, '\0'}};

        while ((option = getopt_long(argc, argv, "l:msf", longOpts, &optionIndex)) != -1) {
            switch (option) {
                case 'l': {
                    minLength = stoi(optarg);
                    break;
                }
                case 'm': {
                    missing = true;
                    break;
                }
                case 'f': {
                    fake = true;
                    break;
                }
                case 's': {
                    toSort = true;
                }
            }
        }
        if (missing or fake) {
            letterCount.resize(26, 0);
        }
    }

    void loadDictionary(istream &dictionaryStream) {
        string word;
        while (dictionaryStream >> word) {
            dictionary.push_back(word);
            if (fake or missing) {
                for (char const c: word) {
                    ++letterCount[c2i(c)];
                }
            }
        }
    }


    void justDoIt(std::string const &letterBank) {
        // Read the letter bank from input
        // Play words based on the letter bank
        vector<string> validWords = this->playWords(letterBank);

        // Print the valid words
        std::cout << "VALID WORDS" << std::endl;
        int count = 0;
        if (toSort) {
            std::sort(validWords.begin(), validWords.end(), [](std::string const &lhs, std::string const &rhs) -> bool {
                size_t const l = lhs.size();
                size_t const r = rhs.size();
                return tie(l, lhs) < tie(r, rhs);
            });
        }
        for (const string &word: validWords) {
            cout << std::setw(4) << word << '\t';
            ++count;
            if (count == 5) {
                count = 0;
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;

        if (fake) {
            std::cout << "Probability of real letter" << std::endl;
            double sum = 0;
            for (char const c: letterBank) {
                sum += static_cast<double>(letterCount[c2i(c)]);
            }
            for (char const cc: letterBank) {
                double percent = (letterCount[c2i(cc)]) / (sum);
                percent *= 100;
                std::cout << cc << ": " << percent << '%' << endl;
            }
            std::cout << std::endl;
        }
        if (missing) {
            using pt = std::pair<char, int>;
            std::vector<pt> coolBank(26);
            char end = 'z' + 1;
            for (char i = 'a'; i < end; ++i) {
                int count_ = this->numWordsValid(letterBank + i);
                coolBank[i - 'a'] = std::make_pair(i, count_);
            }
            std::sort(coolBank.begin(), coolBank.end(), [this](pt const &lhs, pt const &rhs) -> bool {
                if (lhs.second == rhs.second){
                    return letterCount[lhs.first - 'a'] > letterCount[rhs.first - 'a'];
                }
                return lhs.second > rhs.second;
            });
            int counter = 0;
            std::cout << "Number of words added with each missing letter" << std::endl;
            for (auto const &k: coolBank) {
                std::cout << k.first << ": " << k.second << '\t';
                ++counter;
                if (counter == 5) {
                    std::cout << '\n';
                    counter = 0;
                }
            }
            std::cout << std::endl;
        }
    }
};

int main(int argc, char **argv) {
    // Create a WordPlayer object
    try {
        std::cout << std::fixed << std::setprecision(1);
        WordPlayer player;

        player.init(argc, argv);

        std::ifstream is("words.txt");
        // Read the dictionary from input redirection
        player.loadDictionary(is);
        // std::cout << "Dictionary is now filled" << std::endl;
        player.justDoIt(argv[argc - 1]);
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
