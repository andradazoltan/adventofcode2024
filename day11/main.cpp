#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <map>

#define NUM_BLINKS 75

long num_stones(long stone, int blink) {
    static std::map<long, std::map<int, long>> memo;

    if (memo[stone][blink])
        return memo[stone][blink];
    else if (blink == 1) {
        if (stone == 0)
            memo[stone][blink] = 1;
        else {
            std::string st = std::to_string(stone);
            if ((st.length() % 2) == 0)
                memo[stone][blink] = 2;
            else
                memo[stone][blink] = 1;
        }
    }
    else {
        if (stone == 0)
            memo[stone][blink] = num_stones(1, blink - 1);
        else {
            std::string st = std::to_string(stone);

            if ((st.length() % 2) == 0) {
                long stone1 = std::stol(st.substr(0, (int)(st.length() / 2))); 
                long stone2 = std::stol(st.substr((int)(st.length() / 2), st.length()));
                memo[stone][blink] = num_stones(stone1, blink - 1) + num_stones(stone2, blink - 1);
            }
            else
                memo[stone][blink] = num_stones(stone * 2024, blink - 1);
        }
    }

    return memo[stone][blink];
}

int main(void) {
    std::string s;
    std::vector<long> line;    

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        size_t pos;
        do {
            pos = s.find(' ');
            line.push_back(std::stol(s.substr(0, pos)));
            s.erase(0, pos + 1);
        } while (pos != std::string::npos);

        std::getline(f, s);
    }

    // BRUTE FORCE
    /*
    for (int i = 0; i < NUM_BLINKS; i++) {
        int size = line.size();
        for (int j = 0; j < size; j++) {
            if (line[j] == 0)
                line[j] = 1;
            else {
                std::string st = std::to_string(line[j]);

                if ((st.length() % 2) == 0) {
                    line[j] = std::stol(st.substr(0, (int)(st.length() / 2)));
                    line.push_back(std::stol(st.substr((int)(st.length() / 2), st.length())));
                }
                else
                    line[j] = line[j] * 2024;
            }
        }
    }
    */

    // MEMOIZATION
    long total_stones = 0;
    for (auto &s : line)
        total_stones += num_stones(s, NUM_BLINKS);

    std::cout << "STONES " << total_stones << std::endl;

    return 0;
}
