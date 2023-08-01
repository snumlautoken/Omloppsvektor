#include "body.hpp"

Body::Body(Eigen::Vector3d pos, Eigen::Vector3d vel, double m, uint32_t c) {
    position = pos;
    velocity = vel;
    mass = m;
    color = c;
}


void Body::step(double dt) {
    Eigen::Vector3d acceleration = totForce / mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    totForce = {0,0,0};
}
