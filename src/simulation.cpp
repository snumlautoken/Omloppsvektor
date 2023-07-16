#include <iterator>
#include "simulation.hpp"

Simulation::Simulation(double step) : dt(step) {}

void Simulation::addBody(Body b) {
    bodies.push_back(b);
}

void Simulation::step() {
    for (auto itFst = bodies.begin(); itFst != bodies.end(); ++itFst) {
        for (auto itSnd = std::next(itFst); itSnd != bodies.end(); ++itSnd) {
            Eigen::Vector3d fst2snd = (itSnd->position - itFst->position);
            double distance = fst2snd.norm();
            Eigen::Vector3d force = (G * itFst->mass * itSnd->mass / (distance)) * fst2snd.normalized();
            itFst->totForce += force;
            itSnd->totForce -= force;
        }
        itFst->step(dt);
    }
}

