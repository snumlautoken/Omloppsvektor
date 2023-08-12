#include <iostream>
#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>
#include "simulation.hpp"
#include "graphics.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No file provided" << std::endl;
        return 1;
    }
    
    YAML::Node config = YAML::LoadFile(argv[1]);

    Graphics graphics(config["win_size"][0].as<int>(),config["win_size"][1].as<int>());
    Simulation sim(config["sim_time_per_sec"].as<double>());

    for(YAML::const_iterator it=config.begin();it!=config.end();++it) {
        if (it->first.as<std::string>() == "body") {
            std::vector<double> pos = it->second["pos_au"].as<std::vector<double>>();
            std::vector<double> vel = it->second["vel_kms"].as<std::vector<double>>();
            uint32_t color = it->second["color_hex"].as<uint>();
            Body b(Eigen::Vector3d(pos[0],pos[1],pos[2]),Eigen::Vector3d(vel[0],vel[1],vel[2]), it->second["mass_earths"].as<double>(), it->second["gl_rad"].as<float>(), color);
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
