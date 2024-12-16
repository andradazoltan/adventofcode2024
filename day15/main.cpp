#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>

typedef enum {
    EMPTY = 0,
    WALL  = 1,
    BOX   = 2,
    ROBOT = 3
} map_space_t;

typedef enum {
    UP    = 0,
    DOWN  = 1,
    LEFT  = 2,
    RIGHT = 3
} dir_t;

/**
 * Simulate one move of the robot.
 *
 * NOTE: don't need to check for area boundary conditions because the edges are walls,
 * so the robot will never move there.
 *
 * Returns true if the robot was able to move, false otherwise
 */
bool simulate_move(std::vector<std::vector<map_space_t>> &grid, int robot_x, int robot_y, dir_t dir) {
    // Calculate next spot
    int next_x = robot_x;
    int next_y = robot_y;

    if (dir == UP)
        next_y--;
    else if (dir == DOWN)
        next_y++;
    else if (dir == LEFT)
        next_x--;
    else
        next_x++;
    
    // Handle case where spot is empty
    if (grid[next_y][next_x] == EMPTY) {
        grid[next_y][next_x] = ROBOT;
        grid[robot_y][robot_x] = EMPTY;
        return true;
    }
 
    // Handle case where next spot is a box
    if (grid[next_y][next_x] == BOX) {
        if (dir == UP) {
            for (int i = next_y - 1; i > 0; i--) {
                // We hit a wall, there is nowhere to move the boxes
                if (grid[i][next_x] == WALL)
                    break;

                // We found an empty space, move boxes over one space
                if (grid[i][next_x] == EMPTY) {
                    for (int j = i; j < next_y; j++) {
                        grid[j][next_x] = BOX;
                    }
                    grid[next_y][next_x] = ROBOT;
                    grid[robot_y][robot_x] = EMPTY;
                    return true;
                }
            }
        }

        else if (dir == DOWN) {
            for (int i = next_y + 1; i < grid.size(); i++) {
                if (grid[i][next_x] == WALL)
                    break;

                if (grid[i][next_x] == EMPTY) {
                    for (int j = i; j > next_y; j--) {
                        grid[j][next_x] = BOX;
                    }
                    grid[next_y][next_x] = ROBOT;
                    grid[robot_y][robot_x] = EMPTY;
                    return true;
                }
            }
        }
 
        else if (dir == LEFT) {
            for (int i = next_x - 1; i > 0; i--) {
                if (grid[next_y][i] == WALL)
                    break;

                if (grid[next_y][i] == EMPTY) {
                    for (int j = i; j < next_x; j++) {
                        grid[next_y][j] = BOX;
                    }
                    grid[next_y][next_x] = ROBOT;
                    grid[robot_y][robot_x] = EMPTY;
                    return true;
                }
            } 
        }

        else {
            for (int i = next_x + 1; i < grid[0].size(); i++) {
                if (grid[next_y][i] == WALL)
                    break;

                if (grid[next_y][i] == EMPTY) {
                    for (int j = i; j > next_x; j--) {
                        grid[next_y][j] = BOX;
                    }
                    grid[next_y][next_x] = ROBOT;
                    grid[robot_y][robot_x] = EMPTY;
                    return true;
                }
            }
        }
    }

    // Otherwise the robot can't move
    return false;
}

int main(void) {
    std::string s;

    std::vector<std::vector<map_space_t>> grid;
    std::vector<dir_t> directions;
    int robot_x;
    int robot_y;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        std::vector<map_space_t> row;

        // If the first charcter is a '#' we have a grid row
        if (s[0] == '#') {
            for (auto &c : s) {
                if (c == '#')
                    row.push_back(WALL);
                else if (c == 'O')
                    row.push_back(BOX);
                else if (c == '@') {
                    robot_x = row.size();
                    robot_y = grid.size();
                    row.push_back(ROBOT);
                }
                else
                    row.push_back(EMPTY);
            }
        
            grid.push_back(row);
        }
        else {
            for (auto &c : s) {
                if (c == '<')
                    directions.push_back(LEFT);
                else if (c == '>')
                    directions.push_back(RIGHT);
                else if (c == 'v')
                    directions.push_back(DOWN);
                else
                    directions.push_back(UP);
            }
        }

        std::getline(f, s);
    }

    // Simulate all the movements
    for (auto &dir : directions) {
        if (simulate_move(grid, robot_x, robot_y, dir)) {
            if (dir == UP)
                robot_y--;
            else if (dir == DOWN)
                robot_y++;
            else if (dir == LEFT)
                robot_x--;
            else
                robot_x++;
        }
    }

    // Calculate the sum of GPS
    long gps = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == BOX)
                gps += 100 * i + j;
        }
    }

    std::cout << "GPS " << gps << std::endl;
    
    return 0;
}
