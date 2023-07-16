#pragma once
#include <eigen3/Eigen/Dense>

class Body {
public:
    Body(double x, double y, double z, double m);
    double mass;
    Eigen::Vector3d velocity;
    Eigen::Vector3d position;
    Eigen::Vector3d totForce = {0,0,0};
    void step(double dt);
};
