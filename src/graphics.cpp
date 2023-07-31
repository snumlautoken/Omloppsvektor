#include "graphics.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

Graphics::Graphics(int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "Omloppsvektor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);}
    );
    glewInit();
    shaderProgram = createShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glfwSwapBuffers(window);
    glUseProgram(shaderProgram);
    glEnable(GL_DEPTH_TEST);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);
    glEnableVertexAttribArray(0);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)1000 / (float)1000, 0.1f, 100.0f);
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
}

void Graphics::renderPoints(std::vector<glm::vec3>& pts) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto pt : pts) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40*pt[0],40*pt[1],-40+pt[2]*40));
        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, 36);
    }
    glfwSwapBuffers(window);
}

GLuint Graphics::createShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::stringstream ss{ };
    std::string vertexShaderSource{ };
    std::string fragmentShaderSource{ };
    std::ifstream file{ vertexShaderPath };
    if (file.is_open()) {
        ss << file.rdbuf();
        vertexShaderSource = ss.str();
        file.close();
    }
    ss.str(std::string{ });

    file.open(fragmentShaderPath);
    if (file.is_open()) {
        ss << file.rdbuf();
        fragmentShaderSource = ss.str();
        file.close();
    }
    GLuint program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* cVertexSource = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &cVertexSource, nullptr);
    glCompileShader(vertexShader);
    const char* cFragmentSource = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &cFragmentSource, nullptr);
    glCompileShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}
