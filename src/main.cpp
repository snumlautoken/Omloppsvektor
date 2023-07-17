#include <iostream>
#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>
#include "simulation.hpp"
#include "graphics.hpp"

int main() {
    YAML::Node config = YAML::LoadFile("examples/solarsystem.yaml");


    Graphics graphics(config["win_size"][0].as<int>(),config["win_size"][1].as<int>());
    Simulation sim(1);

    for(YAML::const_iterator it=config.begin();it!=config.end();++it) {
        if (it->first.as<std::string>() == "body") {
            std::vector<double> pos = it->second["pos"].as<std::vector<double>>();
            std::vector<double> vel = it->second["vel"].as<std::vector<double>>();
            Body b(Eigen::Vector3d(pos[0],pos[1],pos[2]),Eigen::Vector3d(vel[0],vel[1],vel[2]), it->second["mass"].as<double>());
            sim.addBody(b);
        }
    }    

    while(!glfwWindowShouldClose(graphics.window)) {
        sim.step();
        std::vector<double> vertices;
        for (Body b : sim.bodies) {
            vertices.push_back(b.position[0]);
            vertices.push_back(b.position[1]);
            vertices.push_back(b.position[2]);
        }
        graphics.renderPoints(vertices);
        glfwPollEvents();
    }
    return 0;
}
