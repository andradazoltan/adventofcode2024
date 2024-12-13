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
    for (auto file = (diskmap.end() - 1); file >= diskmap.begin(); --file) {
        if (std::get<0>(*file) == -1)
            continue;

        for (auto disk = diskmap.begin(); disk != file; ++disk) {
            if (std::get<0>(*disk) == -1) {
                int diff = std::get<1>(*disk) - std::get<1>(*file);
                if (diff == 0) {
                    std::get<0>(*disk) = std::get<0>(*file); // Change the ID
                    std::get<0>(*file) = -1;                 // Change the ID
                    break;
                }
                else if (diff > 0) {
                    std::get<0>(*disk) = std::get<0>(*file); // Change the ID
                    std::get<1>(*disk) = std::get<1>(*file); // Change the size
                    std::get<0>(*file) = -1;                 // Change the ID

                    // Insert free space right after
                    diskmap.insert(disk + 1, std::make_tuple(-1, diff));
                    break;
                }
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
