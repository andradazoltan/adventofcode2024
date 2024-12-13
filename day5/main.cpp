#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

int main(void) {
    std::string s;
    std::map<int, std::vector<int>> order;

    int sum_middle = 0;
    int sum_middle_wrong = 0;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        size_t pos;
        if ((pos = s.find('|')) != std::string::npos) {
            int left = std::stoi(s.substr(0, pos));
            int right = std::stoi(s.substr(pos + 1, std::string::npos));

            order[left].push_back(right);
        }
        else if (s.find(',') != std::string::npos) {
            std::vector<int> update;
            bool valid_order = true;
            int prev_val = -1;

            // Part 1
            do {
                pos = s.find(',');
                int curr_val = std::stoi(s.substr(0, pos));
                update.push_back(curr_val);

                if (prev_val != -1) {
                    if (std::find(order[prev_val].begin(), order[prev_val].end(), curr_val) == order[prev_val].end()) {
                        valid_order = false;
                    }
                }
                prev_val = curr_val;
                s.erase(0, pos + 1);
            } while (pos != std::string::npos);

            // Part 2
            if (valid_order)
                sum_middle += update[(int)(update.size() / 2)]; 
            else {
                for (int i = 0; i < update.size(); i++) {
                    for (int j = i + 1; j < update.size(); j++) {
                        if (std::find(order[update[i]].begin(), order[update[i]].end(), update[j]) == order[update[i]].end()) {
                            std::iter_swap(update.begin() + i, update.begin() + j);
                        }
                    }
                }

                sum_middle_wrong += update[(int)(update.size() / 2)]; 
            }
        }

        std::getline(f, s);
    }

    std::cout << "Part 1: " << sum_middle << std::endl;
    std::cout << "Part 2: " << sum_middle_wrong << std::endl;
}
