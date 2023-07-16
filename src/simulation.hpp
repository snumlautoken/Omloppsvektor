#include <vector>
#include "body.hpp"

const double G = 6.674e-11;

class Simulation {
public:
    Simulation(double step);
    void addBody(Body b);
    void step();
    std::vector<Body> bodies;
private:
    double dt;
};
