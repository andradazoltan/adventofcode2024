#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <cmath>
#include <vector>

// Permutation Function
void permute(std::vector<std::string> &permuted, std::string &s, int idx) {
    if (idx == s.size() - 1) {
        permuted.push_back(s);
        return;
    }

    for (int i = idx; i < s.size(); i++) {
        // Easy check to reduce number of permutations. We will still have
        // duplicates but way fewer.
        if (s[idx] == s[i] && (idx != i))
            continue;
        std::swap(s[idx], s[i]);
        permute(permuted, s, idx + 1);
        std::swap(s[idx], s[i]);
    }
}

int main(void) {
    std::string s;
    
    std::vector<char> operators{'s', 'm'};
    long calib_result = 0;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        std::vector<int> operands;
        long result = std::stol(s.substr(0, s.find(':')));
        std::cout << "TARGET: " << result << std::endl;

        // Remove result from the string
        size_t pos = s.find(' ');
        s.erase(0, pos + 1);

        do {
            pos = s.find(' ');
            operands.push_back(std::stoi(s.substr(0, pos)));
            s.erase(0, pos + 1);
        } while (pos != std::string::npos);

        // Permute operators
        std::vector<std::string> permuted;
        int num_spots = operands.size() - 1;

        for (int num_s = 0; num_s < operands.size(); num_s++) {
            std::string base = "";
            for (int i = 0; i < num_s; i++)
                base += 's';
            for (int i = num_s; i < num_spots; i++)
                base += 'm';

            // Permute string if needed
            if ((num_s == 0) || (num_s == num_spots)) {
                permuted.push_back(base);
            }
            else
                permute(permuted, base, 0);
        }

        std::cout << "PERMUTE DONE\n";

        // Try each operator
        for (auto &perm : permuted) {
            long curr_result = 0;
            bool prev_sum = false;

            for (int i = 0; i < perm.size(); i++) {
                if (perm[i] == 'm') {
                    if (prev_sum || (i == 0))
                        curr_result += operands[i];
                    else
                        curr_result *= operands[i];

                    prev_sum = false;
                }

                if (perm[i] == 's') {
                    if (prev_sum || (i == 0))
                        curr_result += operands[i];
                    else
                        curr_result *= operands[i];
                    prev_sum = true;
                }
            }

            // Handle the last operand
            if (prev_sum)
                curr_result += operands[operands.size() - 1];
            else
                curr_result *= operands[operands.size() - 1];

            // Check if we have a match
            if (curr_result == result) {
                calib_result += result;
                break;
            }
        }

        std::cout << std::endl;

        std::getline(f, s);
    }

    std::cout << "TOTAL " << calib_result << std::endl;

    return 0;
}
