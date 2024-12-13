#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <tuple>
#include <map>
#include <cmath>

int main(void) {
    std::string s;
    std::map<char, std::vector<std::tuple<int, int>>> antenna;
    std::vector<std::tuple<bool, std::tuple<int, int>>> antinodes;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    int x = 0;
    int y = 0;

    std::getline(f, s);
    while (f.good()) {
        x = 0;

        for (auto &c : s) {
            if (c != '.')
                antenna[c].push_back(std::make_tuple(x, y));    
            x++;
        }

        std::getline(f, s);
        y++;
    }


    // Check each antenna pair
    for (auto &at : antenna) {
        for (int i = 0; i < at.second.size(); i++) {
            for (int j = i + 1; j < at.second.size(); j++) {
                int x1 = std::get<0>(at.second[i]);
                int y1 = std::get<1>(at.second[i]);
                int x2 = std::get<0>(at.second[j]);
                int y2 = std::get<1>(at.second[j]);

                // Include both antenna nodes
                antinodes.push_back(std::make_tuple(false, std::make_tuple(x1, y1)));
                antinodes.push_back(std::make_tuple(false, std::make_tuple(x2, y2)));

                int x_dist = abs(x1 - x2);
                int y_dist = abs(y1 - y2);

                // Initialize first antinodes
                int antinode_x1, antinode_x2;
                int antinode_y1, antinode_y2;
                if ((x1 <= x2) && (y1 <= y2)) {
                    antinode_x1 = x1 - x_dist;
                    antinode_x2 = x2 + x_dist;
                    antinode_y1 = y1 - y_dist;
                    antinode_y2 = y2 + y_dist;
                }
                else if (x1 <= x2) {
                    antinode_x1 = x1 - x_dist;
                    antinode_x2 = x2 + x_dist;
                    antinode_y1 = y1 + y_dist;
                    antinode_y2 = y2 - y_dist;
                }
                else if (y1 <= y2) {
                    antinode_x1 = x1 + x_dist;
                    antinode_x2 = x2 - x_dist;
                    antinode_y1 = y1 - y_dist;
                    antinode_y2 = y2 + y_dist;
                }
                else {
                    antinode_x1 = x1 + x_dist;
                    antinode_x2 = x2 - x_dist;
                    antinode_y1 = y1 + y_dist;
                    antinode_y2 = y2 - y_dist; 
                }
 
                while ((antinode_x1 >= 0) && (antinode_x1 < x) && (antinode_y1 >= 0) && (antinode_y1 < y)) {
                    antinodes.push_back(std::make_tuple(false, std::make_tuple(antinode_x1, antinode_y1)));
                
                    if ((x1 <= x2) && (y1 <= y2)) {
                        antinode_x1 -= x_dist;
                        antinode_y1 -= y_dist;
                    }
                    else if (x1 <= x2) {
                        antinode_x1 -= x_dist;
                        antinode_y1 += y_dist;
                    }
                    else if (y1 <= y2) {
                        antinode_x1 += x_dist;
                        antinode_y1 -= y_dist;
                    }
                    else {
                        antinode_x1 += x_dist;
                        antinode_y1 += y_dist;
                    }
                }
                
                while ((antinode_x2 >= 0) && (antinode_x2 < x) && (antinode_y2 >= 0) && (antinode_y2 < y)) {
                    antinodes.push_back(std::make_tuple(false, std::make_tuple(antinode_x2, antinode_y2)));

                    if ((x1 <= x2) && (y1 <= y2)) {
                        antinode_x2 += x_dist;
                        antinode_y2 += y_dist;
                    }
                    else if (x1 <= x2) {
                        antinode_x2 += x_dist;
                        antinode_y2 -= y_dist;
                    }
                    else if (y1 <= y2) {
                        antinode_x2 -= x_dist;
                        antinode_y2 += y_dist;
                    }
                    else {
                        antinode_x2 -= x_dist;
                        antinode_y2 -= y_dist; 
                    }
                }
            }
        }
    }

    int num_antinodes = antinodes.size();
    for (int i = 0; i < antinodes.size(); i++) {
        for (int j = i + 1; j < antinodes.size(); j++) {
            if ((std::get<0>(std::get<1>(antinodes[i])) == std::get<0>(std::get<1>(antinodes[j]))) &&
                (std::get<1>(std::get<1>(antinodes[i])) == std::get<1>(std::get<1>(antinodes[j]))) &&
                (std::get<0>(antinodes[j]) == false)) {

                // Mark as touched
                std::get<0>(antinodes[j]) = true;
                num_antinodes--;
            }
        }
    }

    std::cout << "ANTINODES " << num_antinodes << std::endl;
}
