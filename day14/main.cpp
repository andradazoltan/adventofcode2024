#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <tuple>

//#define PART_1

//#define TEST

#ifdef TEST
    #define WIDTH  11
    #define HEIGHT 7
#else
    #define WIDTH  101
    #define HEIGHT 103
#endif

typedef struct {
    int x;
    int y;
} pv_t;

int main(void) {
    std::string s;
    std::vector<pv_t> position;
    std::vector<pv_t> velocity;

    // Parse the input file
#ifdef TEST
    std::ifstream f("test.txt", std::ifstream::in);
#else
    std::ifstream f("input.txt", std::ifstream::in);
#endif

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

    // Find each robot's final position after 100s
    std::vector<pv_t> final_pos;
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

        int x = (position[i].x + vel_x * 100) % WIDTH;
        int y = (position[i].y + vel_y * 100) % HEIGHT;

        final_pos.push_back({.x = x, .y = y});
    }

    // Calculate the safety factor
    int quad_tl = 0;
    int quad_tr = 0;
    int quad_bl = 0;
    int quad_br = 0;
    for (auto &p : final_pos) {
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

    std::cout << "SAFETY " << safety_factor << std::endl;
    

    return 0;
}
