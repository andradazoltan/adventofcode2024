#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>

typedef enum {
    EMPTY = 0,
    WALL  = 1,
    BOX_A = 2,
    BOX_B = 3,
    ROBOT = 4
} map_space_t;

typedef enum {
    UP    = 0,
    DOWN  = 1,
    LEFT  = 2,
    RIGHT = 3
} dir_t;

/**
 * Get the next x-coordinate based on the direction.
 */
int get_next_x_coord(int x, dir_t dir) {
    if (dir == LEFT)
        return x - 1;
    else if (dir == RIGHT)
        return x + 1;
    else
        return x;
}
 
/**
 * Get the next y-coordinate based on the direction.
 */ 
int get_next_y_coord(int y, dir_t dir) {
    if (dir == UP)
        return y - 1;
    else if (dir == DOWN)
        return y + 1;
    else 
        return y;
}

/**
 * Recursively check if we can move boxes in the desired direction.
 * Doesn't actually move the boxes.
 *
 * Return true if boxes can move, false otherwise
 */
bool check_move_boxes(std::vector<std::vector<map_space_t>> &grid, int box_a_x, int box_a_y, dir_t dir) {
    int box_b_x = box_a_x + 1;
    
    int next_xa = get_next_x_coord(box_a_x, dir);
    int next_xb = get_next_x_coord(box_b_x, dir);
    int next_y  = get_next_y_coord(box_a_y, dir);

    if ((next_xa == box_b_x) && (grid[next_y][next_xb] == EMPTY)) {
        return true;
    }
    else if ((next_xb == box_a_x) && (grid[next_y][next_xa] == EMPTY)) {
        return true;
    }
    else if ((grid[next_y][next_xa] == EMPTY) && (grid[next_y][next_xb] == EMPTY)) {
        return true;
    }
    else if ((grid[next_y][next_xa] == WALL) || (grid[next_y][next_xb] == WALL)) {
        return false;
    }
    else {
        bool res = true;

        if (grid[next_y][next_xa] == BOX_A)
            res &= check_move_boxes(grid, next_xa, next_y, dir);
        if ((grid[next_y][next_xa] == BOX_B) && (dir != RIGHT))
            res &= check_move_boxes(grid, next_xa - 1, next_y, dir);
        if ((grid[next_y][next_xb] == BOX_A) && (dir != LEFT))
            res &= check_move_boxes(grid, next_xb, next_y, dir);

        return res;
    }
}
 
/**
 * Recursively move boxes in the desired direction.
 */
void move_boxes(std::vector<std::vector<map_space_t>> &grid, int box_a_x, int box_a_y, dir_t dir) {
    int box_b_x = box_a_x + 1;
    
    int next_xa = get_next_x_coord(box_a_x, dir);
    int next_xb = get_next_x_coord(box_b_x, dir);
    int next_y  = get_next_y_coord(box_a_y, dir);
    
    if (((next_xa == box_b_x) && (grid[next_y][next_xb] == EMPTY)) ||
        ((next_xb == box_a_x) && (grid[next_y][next_xa] == EMPTY)) ||
        ((grid[next_y][next_xa] == EMPTY) && (grid[next_y][next_xb] == EMPTY))) {
        grid[box_a_y][box_a_x] = EMPTY;
        grid[box_a_y][box_a_x + 1] = EMPTY;
 
        grid[next_y][next_xa] = BOX_A;
        grid[next_y][next_xb] = BOX_B;
       return;
    }
    else {
        if (grid[next_y][next_xa] == BOX_A)
            move_boxes(grid, next_xa, next_y, dir);
        if ((grid[next_y][next_xa] == BOX_B) && (dir != RIGHT))
            move_boxes(grid, next_xa - 1, next_y, dir);
        if ((grid[next_y][next_xb] == BOX_A) && (dir != LEFT))
            move_boxes(grid, next_xb, next_y, dir);

        grid[box_a_y][box_a_x] = EMPTY;
        grid[box_a_y][box_a_x + 1] = EMPTY;
 
        grid[next_y][next_xa] = BOX_A;
        grid[next_y][next_xb] = BOX_B; 
    }
}
  
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
    int next_x = get_next_x_coord(robot_x, dir);
    int next_y = get_next_y_coord(robot_y, dir);


    // Handle case where spot is empty
    if (grid[next_y][next_x] == EMPTY) {
        grid[next_y][next_x] = ROBOT;
        grid[robot_y][robot_x] = EMPTY;
        return true;
    }
 
    // Handle case where next spot is a box
    if (grid[next_y][next_x] == BOX_A || grid[next_y][next_x] == BOX_B) {
        int box_a_x = (grid[next_y][next_x] == BOX_A) ? next_x : next_x - 1;

        if (check_move_boxes(grid, box_a_x, next_y, dir)) {
            // Actually move the boxes
            move_boxes(grid, box_a_x, next_y, dir);

            // Move the robot
            grid[next_y][next_x] = ROBOT;
            grid[robot_y][robot_x] = EMPTY;
           
            return true;  
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
                if (c == '#') {
                    row.push_back(WALL);
                    row.push_back(WALL);
                }
                else if (c == 'O') {
                    row.push_back(BOX_A);
                    row.push_back(BOX_B);
                }
                else if (c == '@') {
                    robot_x = row.size();
                    robot_y = grid.size();
                    row.push_back(ROBOT);
                    row.push_back(EMPTY);
                }
                else {
                    row.push_back(EMPTY);
                    row.push_back(EMPTY);
                }
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
            if (grid[i][j] == BOX_A) {
                gps += 100 * i + j;
            }
        }
    }

    std::cout << "GPS " << gps << std::endl;
    
    return 0;
}
