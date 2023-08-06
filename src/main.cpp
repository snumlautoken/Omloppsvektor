#include <iostream>
#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>
#include "simulation.hpp"
#include "graphics.hpp"

int main() {
    YAML::Node config = YAML::LoadFile("examples/solarsystem.yaml");


    Graphics graphics(config["win_size"][0].as<int>(),config["win_size"][1].as<int>());
    Simulation sim(config["sim_time_per_sec"].as<double>());

    for(YAML::const_iterator it=config.begin();it!=config.end();++it) {
        if (it->first.as<std::string>() == "body") {
            std::vector<double> pos = it->second["pos"].as<std::vector<double>>();
            std::vector<double> vel = it->second["vel"].as<std::vector<double>>();
            uint32_t color = it->second["color"].as<uint>();
            Body b(Eigen::Vector3d(pos[0],pos[1],pos[2]),Eigen::Vector3d(vel[0],vel[1],vel[2]), it->second["mass"].as<double>(), color);
            sim.addBody(b);
        }

        for (int i = 0; i < config["random"].as<uint>(); i++) {
            Body b(Eigen::Vector3d(double(std::rand())*2/RAND_MAX-1,double(std::rand())*2/RAND_MAX-1,double(std::rand())*2/RAND_MAX-1),Eigen::Vector3d((double(std::rand())*2/RAND_MAX-1)/20,0,0), 1000, (double(std::rand())*2/RAND_MAX-1)*0xFFFFFF);
            sim.addBody(b);
        }

    }    

    while(!glfwWindowShouldClose(graphics.window)) {
        sim.step();
        graphics.renderPoints(sim.bodies);
    }
    return 0;
}
