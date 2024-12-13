#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <tuple>

//#define PART_1

int get_trail_score(std::vector<std::vector<std::tuple<int, int>>> &trails, int x, int y, int next, int trail_id) {
    if ((x < 0) || (y < 0) || (x == trails[0].size()) || (y == trails.size()))
        return 0;
    if (std::get<0>(trails[y][x]) != next)
        return 0;
    if ((std::get<0>(trails[y][x]) == 9) && (next == 9)) {
        #ifdef PART_1
            if (std::get<1>(trails[y][x]) <= trail_id) {
                std::get<1>(trails[y][x]) = trail_id + 1;
                return 1;
            }
            else
                return 0;
        #else
            return 1;
        #endif
    }

    int score = 0;
    score += get_trail_score(trails, x + 1, y, std::get<0>(trails[y][x]) + 1, trail_id);
    score += get_trail_score(trails, x - 1, y, std::get<0>(trails[y][x]) + 1, trail_id);
    score += get_trail_score(trails, x, y + 1, std::get<0>(trails[y][x]) + 1, trail_id);
    score += get_trail_score(trails, x, y - 1, std::get<0>(trails[y][x]) + 1, trail_id);

    return score;
}

int main(void) {
    std::string s;
    std::vector<std::vector<std::tuple<int, int>>> trails;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        std::vector<std::tuple<int, int>> row;

        for (auto &c : s) {
            row.push_back(std::make_tuple((int)(c - '0'), 0));
        }

        trails.push_back(row);

        std::getline(f, s);
    }

    int sum = 0;
    int trail_id = 0;
    for (int y = 0; y < trails.size(); y++) {
        for (int x = 0; x < trails[0].size(); x++) {
            if (std::get<0>(trails[y][x]) == 0) {
                int scor = get_trail_score(trails, x, y, 0, trail_id);
                sum += scor;
                trail_id++;
            }

        }
    } 

    std::cout << "SUM " << sum << std::endl;
}
