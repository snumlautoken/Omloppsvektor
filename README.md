# Omloppsvektor

Omloppsvektor is a project written in C++/OpenGL that provides a 3D interface for a newtonian gravitational simulator.

![Demo](https://github.com/snumlautoken/Omloppsvektor/blob/main/examples/demo.gif?raw=true)

## Building

The required dependencies are: 
- `glfw3`
- `GLEW`
- `Eigen3`
- `yaml-cpp`
- `glm`
- `stb`

After installing the required dependencies simply run `cmake .` followed by `make` to build the executable!

## Running

The simulation environment is determined by yaml-configurations and an example of one can be found in [resources/rocky.yaml](https://github.com/snumlautoken/Omloppsvektor/blob/main/examples/rocky.yaml), which should be self-explanatory with units in AU, earth-masses and km/s.

To run the example simply run:
```
./Omloppsvektor examples/rocky.yaml
```
