#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include "body.hpp"
#include "input.hpp"

const float gr = (1+sqrt(5))/4;

class Graphics {
public:
    Graphics(int width, int height);
    void renderPoints(std::vector<Body>& bodies);
    GLFWwindow* window;
private:
    GLuint createShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    glm::fvec3 getColor(uint32_t color);
    GLuint loadCubemap(std::string file);

    void generateCube(float sideLen);
    void generateSphere(float radius, uint stacks, uint sectors);

    GLuint shaderProgram;
    GLuint skyboxProgram;
    GLuint skyboxvao;
    GLuint skyboxvbo;
    GLuint skyboxebo;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint skyboxtex;

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<GLuint> indices;
    
    std::unique_ptr<Input> input;
};
