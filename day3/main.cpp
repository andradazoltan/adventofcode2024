#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

//#define PART_1

int main(void) {
    std::string s;
#ifdef PART_1
    std::regex reg("mul\\([0-9]+,[0-9]+\\)");
#else
    std::regex reg("(do\\(\\))|(don't\\(\\))|mul\\([0-9]+,[0-9]+\\)");
#endif

    bool enabled = true;
    int sum = 0;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        for (auto i = std::sregex_iterator(s.begin(), s.end(), reg); i != std::sregex_iterator(); ++i) {
            std::string match = i->str();
            if (match == "do()") {
                enabled = true;
                continue;
            }
            if (match == "don't()") {
                enabled = false;
                continue;
            }

            if (enabled) {
                match.erase(0, match.find('(') + 1);

                int first = std::stoi(match.substr(0, match.find(',')));
                match.erase(0, match.find(',') + 1);
                int second = std::stoi(match.substr(0, match.find(')')));

                sum += first * second;
            }
        }

        std::getline(f, s);
    }

    std::cout << "SUM: " << sum << std::endl;

    return 0;
}

