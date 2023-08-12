#include "body.hpp"
#include <iostream>

Body::Body(Eigen::Vector3d pos, Eigen::Vector3d vel, double m, float r, uint32_t c) {
    position = pos;
    velocity = vel*1000;
    mass = m*earthMass;
    color = c;
    glRad = r;
}


void Body::step(double dt) {
    Eigen::Vector3d acceleration = totForce / mass;
    velocity += acceleration * dt;
    position += velocity * dt / AU;
    totForce = {0,0,0};
}
