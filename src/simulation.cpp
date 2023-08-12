#include <iterator>
#include "simulation.hpp"

Simulation::Simulation(double s) : stepSize(s) {
    prevTime = std::chrono::steady_clock::now();
}

void Simulation::addBody(Body b) {
    bodies.push_back(b);
}

void Simulation::step() {
    std::chrono::time_point<std::chrono::steady_clock> currTime = std::chrono::steady_clock::now();
    double dt = (currTime-prevTime).count()/1e9*stepSize;
    prevTime = currTime;
    for (auto itFst = bodies.begin(); itFst != bodies.end(); ++itFst) {
        for (auto itSnd = std::next(itFst); itSnd != bodies.end(); ++itSnd) {
            Eigen::Vector3d fst2snd = (itSnd->position - itFst->position);
            double distance = fst2snd.norm() * AU;
            if (distance == 0) {continue;}
            Eigen::Vector3d force = (G * itFst->mass * itSnd->mass / (distance*distance)) * fst2snd.normalized();
            itFst->totForce += force;
            itSnd->totForce -= force;
        }
        itFst->step(dt);
    }
}

