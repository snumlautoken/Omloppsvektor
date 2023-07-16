#include "body.hpp"

Body::Body(double x, double y, double z, double m) {
    position = {x,y,z};
    mass = m;
    velocity = {0,0,0};
}


void Body::step(double dt) {
    Eigen::Vector3d acceleration = totForce / mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    totForce = {0,0,0};
}
