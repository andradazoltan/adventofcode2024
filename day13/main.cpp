#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <climits>
#include <cmath>

//#define PART_1

typedef struct {
    int x_inc;
    int y_inc;
} button_t;

typedef struct {
    button_t a;
    button_t b;
    long prize_x;
    long prize_y;
} machine_t;

int main(void) {
    std::string s;
    std::vector<machine_t> machines;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        if (s.find("Button A") != std::string::npos) {
            int x_inc = std::stoi(s.substr(s.find("X+") + 2, s.find(",")));
            int y_inc = std::stoi(s.substr(s.find("Y+") + 2, s.length() - 1));

            machines.push_back({.a = {.x_inc = x_inc, .y_inc = y_inc},
                                .b = {.x_inc = 0, .y_inc = 0},
                                .prize_x = 0,
                                .prize_y = 0});
        }

        else if (s.find("Button B") != std::string::npos) {
            int x_inc = std::stoi(s.substr(s.find("X+") + 2, s.find(",")));
            int y_inc = std::stoi(s.substr(s.find("Y+") + 2, s.length() - 1));
 
            machines[machines.size() - 1].b.x_inc = x_inc;
            machines[machines.size() - 1].b.y_inc = y_inc;
        }

        else if (s.find("Prize") != std::string::npos) {
            int prize_x = std::stoi(s.substr(s.find("X=") + 2, s.find(",")));  
            int prize_y = std::stoi(s.substr(s.find("Y=") + 2, s.length() - 1));  

            machines[machines.size() - 1].prize_x = prize_x;
            machines[machines.size() - 1].prize_y = prize_y;
#ifndef PART_1
            machines[machines.size() - 1].prize_x += 10000000000000;
            machines[machines.size() - 1].prize_y += 10000000000000;
#endif
        }

        std::getline(f, s);
    }

    // We need to find an integer solution for the system of equations:
    //      prize_x = a * a.x_inc + b * b.x_inc
    //      prize_y = a * a.y_inc + b * b.y_inc

    long total_tokens = 0;
    for (auto &mach : machines) {
        long det = mach.a.x_inc * mach.b.y_inc - mach.a.y_inc * mach.b.x_inc;
        long det_x = mach.prize_x * mach.b.y_inc - mach.prize_y * mach.b.x_inc;
        long det_y = mach.prize_y * mach.a.x_inc - mach.prize_x * mach.a.y_inc;

        // Check if this is an integer solution
        if ((det_x % det == 0) && (det_y % det == 0)) {
            long a = det_x / det;
            long b = det_y / det;
            total_tokens += a * 3 + b;
        }
    }

    std::cout << "TOKENS " << total_tokens << std::endl;

    return 0;
}
