#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#define PART_1

int main(void) {
    std::string s;
    std::vector<std::vector<char>> crossword;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        std::vector<char> tmp;
        for (auto &c : s) {
            tmp.push_back(c);
        }

        crossword.push_back(tmp);

        std::getline(f, s);
    }

    // Search the crossword
    int num_rows = crossword.size();
    int num_cols = crossword[0].size();

    int num_xmas = 0;

#ifdef PART_1
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            char c = crossword[i][j];

            if (c == 'X') {
                // Search horizontal left
                if (j >= 3) {
                    if ((crossword[i][j - 1] == 'M') &&
                        (crossword[i][j - 2] == 'A') &&
                        (crossword[i][j - 3] == 'S'))
                        num_xmas++;
                }

                // Search horizontal right
                if (j <= (num_cols - 4)) {
                    if ((crossword[i][j + 1] == 'M') &&
                        (crossword[i][j + 2] == 'A') &&
                        (crossword[i][j + 3] == 'S'))
                        num_xmas++;
                }

                // Search up
                if (i >= 3) {
                    if ((crossword[i - 1][j] == 'M') &&
                        (crossword[i - 2][j] == 'A') &&
                        (crossword[i - 3][j] == 'S'))
                        num_xmas++; 
                }

                // Search down
                if (i <= (num_cols - 4)) {
                    if ((crossword[i + 1][j] == 'M') &&
                        (crossword[i + 2][j] == 'A') &&
                        (crossword[i + 3][j] == 'S'))
                        num_xmas++; 
                }
 
                // Search diagonal up - left
                if ((i >= 3) && (j >= 3)) {
                    if ((crossword[i - 1][j - 1] == 'M') &&
                        (crossword[i - 2][j - 2] == 'A') &&
                        (crossword[i - 3][j - 3] == 'S'))
                        num_xmas++; 
                }
 

                // Search diagonal up - right
                if ((i >= 3) && (j <= (num_cols - 4))) {
                    if ((crossword[i - 1][j + 1] == 'M') &&
                        (crossword[i - 2][j + 2] == 'A') &&
                        (crossword[i - 3][j + 3] == 'S'))
                        num_xmas++; 
                }
 
                // Search diagonal down - left
                if ((i <= (num_rows - 4)) && (j >= 3)) {
                    if ((crossword[i + 1][j - 1] == 'M') &&
                        (crossword[i + 2][j - 2] == 'A') &&
                        (crossword[i + 3][j - 3] == 'S'))
                        num_xmas++; 
                }
 
                // Search diagonal down - right
                if ((i <= (num_rows - 4)) && (j <= (num_cols - 4))) {
                    if ((crossword[i + 1][j + 1] == 'M') &&
                        (crossword[i + 2][j + 2] == 'A') &&
                        (crossword[i + 3][j + 3] == 'S'))
                        num_xmas++; 
                }
 
            }
        }
    }
#else
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            char c = crossword[i][j];

            if (c == 'M') {
                // Search diagonal up - left
                if ((i >= 2) && (j >= 2)) {
                    if ((crossword[i - 1][j - 1] == 'A') &&
                        (crossword[i - 2][j - 2] == 'S') &&
                        (((crossword[i][j - 2] == 'M') && (crossword[i - 2][j] == 'S')) ||
                        ((crossword[i][j - 2] == 'S') && (crossword[i - 2][j] == 'M'))))
                        num_xmas++; 
                }
 

                // Search diagonal up - right
                if ((i >= 2) && (j <= (num_cols - 3))) {
                    if ((crossword[i - 1][j + 1] == 'A') &&
                        (crossword[i - 2][j + 2] == 'S') &&
                        (((crossword[i][j + 2] == 'M') && (crossword[i - 2][j] == 'S')) ||
                        ((crossword[i][j + 2] == 'S') && (crossword[i - 2][j] == 'M'))))
                        num_xmas++; 
                }
 
                // Search diagonal down - left
                if ((i <= (num_rows - 3)) && (j >= 2)) {
                    if ((crossword[i + 1][j - 1] == 'A') &&
                        (crossword[i + 2][j - 2] == 'S') &&
                        (((crossword[i][j - 2] == 'M') && (crossword[i + 2][j] == 'S')) ||
                        ((crossword[i][j - 2] == 'S') && (crossword[i + 2][j] == 'M'))))
 
                        num_xmas++; 
                }
 
                // Search diagonal down - right
                if ((i <= (num_rows - 3)) && (j <= (num_cols - 3))) {
                    if ((crossword[i + 1][j + 1] == 'A') &&
                        (crossword[i + 2][j + 2] == 'S') &&
                        (((crossword[i][j + 2] == 'M') && (crossword[i + 2][j] == 'S')) ||
                        ((crossword[i][j + 2] == 'S') && (crossword[i + 2][j] == 'M'))))
 
                        num_xmas++; 
                }
 
            }
        }
    }
#endif

    std::cout << "NUM: " << (num_xmas / 2) << std::endl;

    return 0;
}

