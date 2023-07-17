#pragma once
#include <eigen3/Eigen/Dense>

class Body {
public:
    Body(Eigen::Vector3d pos, Eigen::Vector3d vel, double m);
    double mass;
    Eigen::Vector3d velocity;
    Eigen::Vector3d position;
    Eigen::Vector3d totForce = {0,0,0};
    void step(double dt);
};
