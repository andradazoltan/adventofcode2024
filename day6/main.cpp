#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <vector>
#include <tuple>
#include <map>
#include <cstring>

// Directions guard can travel
#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

// Type of space in the map
typedef enum {
    EMPTY            = 0,
    OBSTACLE_NOT_HIT = 1,
    OBSTACLE_UP      = 2,
    OBSTACLE_DOWN    = 3,
    OBSTACLE_LEFT    = 4,
    OBSTACLE_RIGHT   = 5,
    GUARD_PATH       = 6
} map_space_t;

/**
 * Walk the guard path from the starting position and determine
 * if the guard gets stuck in a loop or not.
 *
 * Returns true if stuck, false if exit successfully
 */
int walk_path(std::vector<std::vector<map_space_t>> &the_map, std::tuple<int, int> &guard_start) {
    bool stuck = false;
    int dir = UP;
    std::tuple<int, int> guard = guard_start;

    // Walk the path
    while (1) {
        int x = std::get<0>(guard);
        int y = std::get<1>(guard);

        if (dir == UP) {
            for (int i = y - 1; i >= 0; i--) {
                if ((the_map[i][x] >= OBSTACLE_NOT_HIT) && (the_map[i][x] <= OBSTACLE_RIGHT)) {
                    // We already hit this obstacle while going up, we must be in a loop
                    if (the_map[i][x] == OBSTACLE_UP)
                        stuck = true;

                    the_map[i][x] = OBSTACLE_UP;
                    dir = RIGHT;
                    std::get<1>(guard) = i + 1;
                    break;
                }
                else    
                    the_map[i][x] = GUARD_PATH;
            }

            // We are done
            if ((dir == UP) || stuck)
                break;
        }

        else if (dir == RIGHT) {
            for (int i = x + 1; i < the_map[0].size(); i++) {
                if ((the_map[y][i] >= OBSTACLE_NOT_HIT) && (the_map[y][i] <= OBSTACLE_RIGHT)) {
                    // We already hit this obstacle while going right, we must be in a loop
                    if (the_map[y][i] == OBSTACLE_RIGHT)
                        stuck = true;

                    the_map[y][i] = OBSTACLE_RIGHT;
                    dir = DOWN;
                    std::get<0>(guard) = i - 1;
                    break;
                }
                else    
                    the_map[y][i] = GUARD_PATH;
            }
            
            // We are done
            if (dir == RIGHT)
                break;
        }

        else if (dir == DOWN) {
            for (int i = y + 1; i < the_map.size(); i++) {
                if ((the_map[i][x] >= OBSTACLE_NOT_HIT) && (the_map[i][x] <= OBSTACLE_RIGHT)) {
                    // We already hit this obstacle while going down, we must be in a loop
                    if (the_map[i][x] == OBSTACLE_DOWN)
                        stuck = true;

                    the_map[i][x] = OBSTACLE_DOWN;
                    dir = LEFT;
                    std::get<1>(guard) = i - 1;
                    break;
                }
                else    
                    the_map[i][x] = GUARD_PATH;
            }
            
            // We are done
            if (dir == DOWN)
                break;
        }
        
        else {
            for (int i = x - 1; i >= 0; i--) {
                if ((the_map[y][i] >= OBSTACLE_NOT_HIT) && (the_map[y][i] <= OBSTACLE_RIGHT)) {
                    // We already hit this obstacle while going left, we must be in a loop
                    if (the_map[y][i] == OBSTACLE_LEFT)
                        stuck = true;

                    the_map[y][i] = OBSTACLE_LEFT;
                    dir = UP;
                    std::get<0>(guard) = i + 1;
                    break;
                }
                else    
                    the_map[y][i] = GUARD_PATH;
            }
            
            // We are done
            if (dir == LEFT)
                break;
        }
    }

    return stuck;
}

int main(void) {
    std::string s;
    std::vector<std::vector<map_space_t>> empty_map;
    std::tuple<int, int> guard_start;

    int new_obstacles = 0;
    int spaces_walked = 0;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    int x = 0;
    int y = 0;
    std::getline(f, s);
    while (f.good()) {
        x = 0;
        
        empty_map.push_back(std::vector(strlen(s.c_str()), EMPTY));

        for (auto &c : s) {
            if (c == '#')
                empty_map[y][x] = OBSTACLE_NOT_HIT;

            if (c == '^') {
                empty_map[y][x] = GUARD_PATH;
                guard_start = std::make_tuple(x, y);
            }
            
            x++;
        }

        std::getline(f, s);
        y++;
    }

    // PART 1: Walk the intial path
    std::vector<std::vector<map_space_t>> map_p1 = empty_map;
    walk_path(map_p1, guard_start);
    
    for (auto &row : map_p1) {
        for (auto &pos : row) {
            if (pos == GUARD_PATH)
                spaces_walked++;
        }
    }
     
    // PART 2: The guard path does not change if we place an obstacle outside
    // its initial path. Try to place an obstacle in every place (except the starting
    // place) that the guard travelled
    for (int i = 0; i < map_p1.size(); i++) {
        for (int j = 0; j < map_p1[0].size(); j++) {
            if ((map_p1[j][i] == GUARD_PATH) &&
                ((j != std::get<0>(guard_start)) || (i != std::get<1>(guard_start)))) {

                std::vector<std::vector<map_space_t>> temp_map = empty_map;
                temp_map[j][i] = OBSTACLE_NOT_HIT;

                if (walk_path(temp_map, guard_start))
                    new_obstacles++;
            }
        }
    }

    std::cout << "Part 1: " << spaces_walked << std::endl;
    std::cout << "Part 2: " << new_obstacles << std::endl;
}
