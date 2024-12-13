#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

//#define PART_1

bool test_report_part1(std::string s);
bool test_report_part2(std::string s, bool removed_one);

int main(void) {
    std::string s;

    int safe = 0;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
#ifdef PART_1
        if (test_report_part1(s))
            safe++;
#else
        if (test_report_part2(s, false))
            safe++;
        else {
            // The above function doesn't work for removing the first and
            // second levels. Try to do that if the first check fails
            auto first_space = s.find(' ');
            std::string s_first = s.substr(0, first_space);
            std::string s_no_first = s.substr(first_space + 1, std::string::npos);

            auto second_space = s_no_first.find(' ');
            std::string s_no_second = s_first + " " + s_no_first.substr(second_space + 1, std::string::npos);

            if (test_report_part2(s_no_first, true) || test_report_part2(s_no_second, true))
                safe++;
        }
#endif

        std::getline(f, s);
    }

    std::cout << "Num Safe: " << safe << std::endl;

    return 0;
}

/**
 * Test the levels report from Part 1.
 *
 * s: Report to check
 *
 * Returns true if safe, false if unsafe
 */
bool test_report_part1(std::string s) {
    bool unsafe = false;
    bool increasing = false;
    bool decreasing = false;
    
    int prev = -1;

    size_t delim_pos;
    do {
        delim_pos = s.find(' ');

        int curr = std::stoi(s.substr(0, delim_pos));
        
        if (prev != -1) {
            if (curr > prev)
                increasing = true;
            if (curr < prev)
                decreasing = true;

            // Unsafe if difference is not within correct bounds
            if (abs(curr - prev) < 1 || abs(curr - prev) > 3) {
                unsafe = true;
                break;
            }

            // Unsafe if both increasing and decreasing
            if (increasing && decreasing) {
                unsafe = true;
                break;
            }
        }

        prev = curr;
        s.erase(0, delim_pos + 1);
    } while (delim_pos != std::string::npos); 

    return !unsafe;
}
 
/**
 * Test the levels report from Part 2.
 *
 * s: Report to check
 * removed_one: True if we already removed a level
 *
 * Returns true if safe, false if unsafe
 */ 
bool test_report_part2(std::string s, bool removed_one) {
    bool unsafe = false;
    bool increasing = false;
    bool decreasing = false;

    int prev = -1;

    size_t delim_pos;
    do {
        delim_pos = s.find(' ');

        int curr = std::stoi(s.substr(0, delim_pos));
        
        if (prev != -1) {
            bool prev_increasing = increasing;
            bool prev_decreasing = decreasing;

            if (curr > prev)
                increasing = true;
            if (curr < prev)
                decreasing = true;

            // Unsafe if difference is not within correct bounds
            if (abs(curr - prev) < 1 || abs(curr - prev) > 3) {
                if (!removed_one) {
                    removed_one = true;

                    // Restore increasing and decreasing
                    increasing = prev_increasing;
                    decreasing = prev_decreasing;

                    s.erase(0, delim_pos + 1);
                    continue;
                }
                else {
                    unsafe = true;
                    break;
                }
            }
            // Unsafe if both increasing and decreasing
            if (increasing && decreasing) {
                if (!removed_one) {
                    removed_one = true;

                    // Restore increasing and decreasing
                    increasing = prev_increasing;
                    decreasing = prev_decreasing;

                    s.erase(0, delim_pos + 1);
                    continue;
                }
                else {
                    unsafe = true;
                    break;
                }
            } 
        }       

        prev = curr;
        s.erase(0, delim_pos + 1);
    } while (delim_pos != std::string::npos); 

    return !unsafe; 
}
