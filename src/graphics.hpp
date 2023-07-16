#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <string>
#include <vector>

class Graphics {
public:
    Graphics(int width, int height);
    void renderPoints(std::vector<double> pts);
private:
    GLuint createShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};
