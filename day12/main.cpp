#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>

//#define PART_1

// Recursively search for all plots that belong to this region
void mark_region(std::vector<std::vector<char>> &field,
                 std::vector<std::vector<int>> &regions, 
                 int x,
                 int y,
                 int region_id)
{
    // Block already searched
    if (regions[y][x] != 0)
        return;

    regions[y][x] = region_id;

    if ((x > 0) && (field[y][x] == field[y][x - 1]))
        mark_region(field, regions, x - 1, y, region_id);

    if ((y > 0) && (field[y][x] == field[y - 1][x]))
        mark_region(field, regions, x, y - 1, region_id);
    
    if ((x < (field[0].size() - 1)) && (field[y][x] == field[y][x + 1]))
        mark_region(field, regions, x + 1, y, region_id);

    if ((y < (field.size() - 1)) && (field[y][x] == field[y + 1][x]))
        mark_region(field, regions, x, y + 1, region_id);
}

int main(void) {
    std::string s;
    std::vector<std::vector<char>> field;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        std::vector<char> row;

        for (auto &c : s) {
            row.push_back(c);
        }

        field.push_back(row);
        std::getline(f, s);
    }
    
    // Group all the like crops and assign a region ID
    std::vector<std::vector<int>> regions(field.size(), std::vector(field[0].size(), 0));

    int region_id = 1;
    for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field[0].size(); j++) {
            if (regions[i][j] == 0) {
                mark_region(field, regions, j, i, region_id);
                region_id++;
            }
        }
    }

    // Calculate the area of all the regions
    std::vector<int> area(region_id - 1, 0);
    for (auto &row : regions) {
        for (auto &crop : row) {
            area[crop - 1]++;
        }
    }

    // Calculate the perimeter of all the regions
    std::vector <int> perimeter(region_id - 1, 0);
    for (int i = 0; i < regions.size(); i++) {
        for (int j = 0; j < regions[0].size(); j++) {
            int crop = regions[i][j] - 1;

            // We need a fence if we are along the border of the land
            if ((i == 0) || (i == (regions.size() - 1)))
                perimeter[crop]++;
            if ((j == 0) || (j == (regions[0].size() - 1)))
                perimeter[crop]++;

            // We need a fence between different regions
            if ((i > 0) && (field[i][j] != field[i - 1][j]))
                perimeter[crop]++;

            if ((j > 0) && (field[i][j] != field[i][j - 1]))
                perimeter[crop]++;
                                  
            if ((i < (regions.size() - 1)) && (field[i][j] != field[i + 1][j]))
                perimeter[crop]++; 

            if ((j < (regions[0].size() - 1)) && (field[i][j] != field[i][j + 1]))
                perimeter[crop]++;
        }
    }

    // Adjust perimeter to count sides, not plots
#ifndef PART_1
    for (int i = 0; i < regions.size(); i++) {
        for (int j = 0; j < regions[0].size(); j++) {
            int crop = regions[i][j] - 1;
     
            // Horizontal Edges
            if ((j > 0) && (field[i][j] == field[i][j - 1])) {
                // Border of the farm
                if ((i == 0) || (i == (regions.size() - 1)))
                    perimeter[crop]--;

                // Bottom Edge
                if ((i < (regions.size() - 1)) &&
                    (field[i][j] != field[i + 1][j]) && (field[i][j - 1] != field[i + 1][j - 1])) 
                    perimeter[crop]--;
                 
                // Top Edge
                if ((i > 0) &&
                    (field[i][j] != field[i - 1][j]) && (field[i][j - 1] != field[i - 1][j - 1])) 
                    perimeter[crop]--;
            }
                 
            // Vertical Edges
            if ((i > 0) && (field[i][j] == field[i - 1][j])) {
                // Border of the farm
                if ((j == 0) || (j == (regions[0].size() - 1)))
                    perimeter[crop]--;
 
                // Right Edge
                if ((j < (regions[0].size() - 1)) &&
                    (field[i][j] != field[i][j + 1]) && (field[i - 1][j] != field[i - 1][j + 1])) 
                    perimeter[crop]--;
                 
                // Left Edge
                if ((j > 0) &&
                    (field[i][j] != field[i][j - 1]) && (field[i - 1][j] != field[i - 1][j - 1])) 
                    perimeter[crop]--;
            } 
        }
    }
#endif

    // Calculate the total price
    int price = 0;
    for (int i = 0; i < area.size(); i++)
        price += area[i] * perimeter[i];

    std::cout << "PRICE " << price << std::endl;
}
