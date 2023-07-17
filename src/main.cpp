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
            Body b(it->second["pos"][0].as<double>(),it->second["pos"][1].as<double>(),it->second["pos"][2].as<double>(),it->second["mass"].as<double>());
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
