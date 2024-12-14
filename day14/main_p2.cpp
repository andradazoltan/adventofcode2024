#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <climits>

#define WIDTH  101
#define HEIGHT 103

typedef struct {
    int x;
    int y;
} pv_t;

// Print the grid of bots in their current positions
void print_bots(std::vector<pv_t> &p) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char c = '.';
            for (auto &bot : p) {
                if ((bot.x == j) && (bot.y == i))
                    c = '#';
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int main(void) {
    std::string s;
    std::vector<pv_t> position;
    std::vector<pv_t> velocity;

    // Parse the input file
    std::ifstream f("input.txt", std::ifstream::in);

    std::getline(f, s);
    while (f.good()) {
        pv_t pos;
        pos.x = std::stoi(s.substr(2, s.find(',')));
        pos.y = std::stoi(s.substr(s.find(',') + 1, s.find(' ')));

        position.push_back(pos);

        s.erase(0, s.find(' ') + 1);

        pv_t vel;
        vel.x = std::stoi(s.substr(2, s.find(',')));
        vel.y = std::stoi(s.substr(s.find(',') + 1, s.length() - 1));
        
        velocity.push_back(vel);

        std::getline(f, s);
    }

    // Iterate many times, print out the grid when we see the safety factor
    // at an abnormally low or abnormally high value. This either means the
    // robots are mostly in 1 quadrant to form the tree, or the robots are
    // equally in all quadrants if th tree is somewhat centered.
    //
    // These thresholds were chosen heuristically for the input data set by 
    // first finding the min / max safety factors.
    int max_sf_thresh = 240000000;
    int min_sf_thresh = 68000000;
    for (int iter = 0; iter < 10000; iter++) {
        for (int i = 0; i < position.size(); i++) {
            // We can change negative velocities into positive velocities
            // based on the fact that positions wrap once the robot
            // reaches the end of the area
            int vel_x = velocity[i].x;
            if (vel_x < 0)
                vel_x = WIDTH + vel_x;
        
            int vel_y = velocity[i].y;
            if (vel_y < 0)
                vel_y = HEIGHT + vel_y;

            position[i].x = (position[i].x + vel_x) % WIDTH;
            position[i].y = (position[i].y + vel_y) % HEIGHT;
        }

        // Calculate the safety factor
        int quad_tl = 0;
        int quad_tr = 0;
        int quad_bl = 0;
        int quad_br = 0;
        for (auto &p : position) {
            if ((p.x < (WIDTH / 2)) && (p.y < (HEIGHT / 2)))
                quad_tl++;
        
            if ((p.x < (WIDTH / 2)) && (p.y > (HEIGHT / 2)))
                quad_bl++;
        
            if ((p.x > (WIDTH / 2)) && (p.y < (HEIGHT / 2)))
                quad_tr++;

            if ((p.x > (WIDTH / 2)) && (p.y > (HEIGHT / 2)))
                quad_br++;
        }

        int safety_factor = quad_tl * quad_tr * quad_bl * quad_br;

        if ((safety_factor < min_sf_thresh) || (safety_factor > max_sf_thresh)) {
            std::cout << "ITERATION " << iter + 1 << std::endl;
            print_bots(position);
            std::cout << std::endl;
        }
    }

    return 0;
}
