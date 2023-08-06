#pragma once
#include <eigen3/Eigen/Dense>

const double AU = 1.495978707e11;

class Body {
public:
    Body(Eigen::Vector3d pos, Eigen::Vector3d vel, double m, uint32_t c = 0xFF00FF);
    double mass;
    Eigen::Vector3d velocity;
    Eigen::Vector3d position;
    Eigen::Vector3d totForce = {0,0,0};
    uint32_t color;
    void step(double dt);
};
