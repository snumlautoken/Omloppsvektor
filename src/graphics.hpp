#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "body.hpp"

const float gr = (1+sqrt(5))/4;

class Graphics {
public:
    Graphics(int width, int height);
    void renderPoints(std::vector<Body>& bodies);
    GLFWwindow* window;
private:
    GLuint createShaderProgram(const std::string& vertexrShaderPath, const std::string& fragmentShaderPath);
    glm::fvec3 getColor(uint32_t color);

    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    float vertices[36] = {
        -0.5, 0.0, gr, 0.5, 0.0, gr, -0.5, 0.0, -gr, 0.5, 0.0, -gr,
        0.0, gr, 0.5, 0.0, gr, -0.5, 0.0, -gr, 0.5, 0.0, -gr, -0.5,
        gr, 0.5, 0.0, -gr, 0.5, 0.0, gr, -0.5, 0.0, -gr, -0.5, 0.0 
    };
    GLuint indices[60] = { 
        0,4,1, 0,9,4, 9,5,4, 4,5,8, 4,8,1,    
        8,10,1, 8,3,10, 5,3,8, 5,2,3, 2,7,3,    
        7,10,3, 7,6,10, 7,11,6, 11,0,6, 0,1,6, 
        6,1,10, 9,0,11, 9,11,2, 9,2,5, 7,2,11 
    };
};
