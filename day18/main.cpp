#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <climits>
#include <queue>
#include <tuple>

using std::vector;

// Uncomment to run small test case 
//#define TEST

#ifdef TEST
    #define GRID_SIZE 7
    #define NUM_ROWS_READ 12
#else
    #define GRID_SIZE 71
    #define NUM_ROWS_READ 1024
#endif

/**
 * Get the length of the shortest path to the bottom right corner of the grid.
 */
int get_shortest_path_length(vector<vector<bool>> &grid, int x, int y) {
    vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
    vector<vector<int>> distances(GRID_SIZE, vector<int>(GRID_SIZE, INT_MAX));

    // Dijkstra's Algorithm
    // We don't really need a priority queue, since the weight of
    // every edge is the same
    std::queue<std::tuple<int,int,int>> q;
    q.push(std::make_tuple(0, x, y));

    while (q.size() > 0) {
        auto curr_node = q.front();
        q.pop();

        int dist = std::get<0>(curr_node);
        int x = std::get<1>(curr_node);
        int y = std::get<2>(curr_node);

        // We already visited this node
        if (visited[y][x])
            continue;

        // Check if we reached the last node
        if ((x == (GRID_SIZE - 1)) && (y == (GRID_SIZE - 1))) {
            return dist;
        }

        // Left
        if ((x > 0) && !grid[y][x - 1]) {
            if ((dist + 1) < distances[y][x - 1])
                q.push(std::make_tuple(dist + 1, x - 1, y));
        }
        
        // Right
        if ((x < (GRID_SIZE - 1)) && !grid[y][x + 1]) {
            if ((dist + 1) < distances[y][x + 1])
                q.push(std::make_tuple(dist + 1, x + 1, y));
        }
        
        // Up
        if ((y > 0) && !grid[y - 1][x]) {
            if ((dist + 1) < distances[y - 1][x])
                q.push(std::make_tuple(dist + 1, x, y - 1));
        }
 
        // Down
        if ((y < (GRID_SIZE - 1)) && !grid[y + 1][x]) {
            if ((dist + 1) < distances[y + 1][x])
                q.push(std::make_tuple(dist + 1, x, y + 1));
        }

        // Mark current node as visited
        visited[y][x] = true;
    }

    // We shouldn't get here if there is a valid path
    return -1;
}
    

int main(void) {
    std::string s;

    vector<vector<bool>> grid(GRID_SIZE, vector<bool>(GRID_SIZE, false));

    // Parse the input file
#ifdef TEST
    std::ifstream f("test.txt", std::ifstream::in);
#else
    std::ifstream f("input.txt", std::ifstream::in);
#endif

    int row = 0;
    std::getline(f, s);
    while (f.good()) {
        int x = std::stoi(s.substr(0, s.find(',')));
        int y = std::stoi(s.substr(s.find(',') + 1, s.length() - 1));

        grid[y][x] = true;

        row++;
        std::getline(f, s);

        if (row >= NUM_ROWS_READ) {
            int length = get_shortest_path_length(grid, 0, 0);

            // PART 1
            if (row == NUM_ROWS_READ)
                std::cout << "SHORTEST PATH " << length << std::endl;

            // PART 2
            if (length == -1) {
                std::cout << "BLOCKING COORD: " << x << " " << y << std::endl;
                break;
            }
        }
    }

    return 0;
}
