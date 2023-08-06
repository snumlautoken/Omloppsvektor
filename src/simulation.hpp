#include <vector>
#include <chrono>
#include "body.hpp"

const double G = 6.674e-11;

class Simulation {
public:
    Simulation(double step);
    void addBody(Body b);
    void step();
    std::vector<Body> bodies;
private:
    double stepSize;
    std::chrono::time_point<std::chrono::steady_clock> prevTime;
};
