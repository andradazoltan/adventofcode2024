#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main(void) {
    std::string s;
    std::vector<int> left_list;
    std::vector<int> right_list;

    int distance = 0;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        // Parse the line
        auto delim_pos = s.find(' ');
        
        int left  = std::stoi(s.substr(0, delim_pos));
        int right = std::stoi(s.substr(delim_pos, std::string::npos));
        
        left_list.push_back(left);
        right_list.push_back(right);

        std::getline(f, s);
    }

    // PART 1 ***************************************************
    // Sort both lists
    std::sort(left_list.begin(), left_list.end());
    std::sort(right_list.begin(), right_list.end());

    // Calculate the distance
    for (int i = 0; i < left_list.size(); i++) {
        distance += abs(left_list[i] - right_list[i]);
    }

    std::cout << "Part 1: " << distance << std::endl;

    // PART 2 ***************************************************
    // Create a map of integer counts in the right list
    std::map<int, int> right_list_cnts;

    for (auto &val : right_list) {
        if (right_list_cnts[val])
            right_list_cnts[val]++;
        else
            right_list_cnts[val] = 1;
    }

    // Calculate similarity score
    int similarity = 0;

    for (auto &val : left_list) {
        similarity += val * right_list_cnts[val];
    }

    std::cout << "Part 2: " << similarity << std::endl;

    return 0;
}
