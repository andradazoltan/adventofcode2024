#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <tuple>
#include <cmath>

int main(void) {
    std::string s;
    std::vector<std::tuple<int, int>> diskmap;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);
    
    int id = 0;
    bool free_space = false;
    std::getline(f, s);
    while (f.good()) {
        for (auto &c : s) {
            int size = (int)(c - '0');
            if (!free_space) {
                diskmap.push_back(std::make_tuple(id, size));
                id++;
            }
            else
                diskmap.push_back(std::make_tuple(-1, size));

            free_space ^= true;
        }

        std::getline(f, s);
    }
     
    // Fill up the free space
    auto end_ptr = diskmap.end() - 1;
    for (auto disk = diskmap.begin(); disk != diskmap.end(); ++disk) {
        if (std::get<0>(*disk) == -1) {
            if (std::get<0>(*end_ptr) == -1)
                end_ptr--;

            // Break once we filled all the free space
            if (end_ptr < disk)
                break;

            int diff = std::get<1>(*disk) - std::get<1>(*end_ptr);
            if (diff == 0) {
                std::get<0>(*disk) = std::get<0>(*end_ptr); // Change the ID
                std::get<0>(*end_ptr) = -1;                 // Change the ID
                end_ptr--;
            }
            else if (diff < 0) {
                std::get<0>(*disk) = std::get<0>(*end_ptr); // Change the ID
                std::get<1>(*end_ptr) = abs(diff);          // Change the size remaining

                // Insert free space at the end
                diskmap.insert(end_ptr + 1, std::make_tuple(-1, std::get<1>(*disk)));
            }
            else {
                std::get<0>(*disk) = std::get<0>(*end_ptr); // Change the ID
                std::get<1>(*disk) = std::get<1>(*end_ptr); // Change the size
                std::get<0>(*end_ptr) = -1;                 // Change the ID

                // Insert free space right after
                diskmap.insert(disk + 1, std::make_tuple(-1, diff));
                end_ptr--;
            }
        }
    }

    // Calculate checksum
    long checksum = 0;
    int slot = 0;
    for (auto &disk : diskmap) {
        for (int i = 0; i < std::get<1>(disk); i++) {
            if (std::get<0>(disk) != -1)
                checksum += slot * std::get<0>(disk);
            slot++;
        }
    }

    std::cout << "CHECKSUM " << checksum << std::endl;
}
