#include "graphics.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void printError() {
    std::cout << glGetError() << std::endl;
}

Graphics::Graphics(int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, "Omloppsvektor", nullptr, nullptr);
    glewExperimental = GL_TRUE;

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);}
    );

    glewInit();

    input = std::make_unique<Input>(window);

    shaderProgram = createShaderProgram("shaders/vertex.vs", "shaders/fragment.fs");
    skyboxProgram = createShaderProgram("shaders/skybox.vs", "shaders/skybox.fs");
    glEnable(GL_MULTISAMPLE);  

    // Objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    generateSphere(1.0, 16, 32);
    size_t vertexSize = vertices.size()*sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, vertexSize*2, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normals.size()*sizeof(float), normals.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(float), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)(vertexSize));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Skybox
    glGenVertexArrays(1, &skyboxvao);
    glGenBuffers(1, &skyboxvbo);
    glGenBuffers(1, &skyboxebo);
    glBindVertexArray(skyboxvao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxvbo);
    generateCube(1.0);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(float), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);
    glEnableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glUseProgram(shaderProgram);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)1000 / (float)1000, 0.1f, 1000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    
    glUseProgram(skyboxProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    skyboxtex = loadCubemap("resources/starbox");
    glUniform1i(glGetUniformLocation(skyboxProgram, "skybox"), 0);
}

void Graphics::renderPoints(std::vector<Body>& bodies) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    input->pollEvents();
    glm::mat4 view;
    view = input->getView();
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    for (Body body : bodies) {
        glm::mat4 model = glm::mat4(1.0f);
        auto pt = body.position;
        model = glm::translate(model, glm::vec3(pt[0],pt[1],pt[2]));
        model = glm::rotate(model, (float)glfwGetTime()*glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(body.glRad,body.glRad,body.glRad));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glm::mat3 invModel = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "invModel"), 1, GL_FALSE, &invModel[0][0]);
        glBindVertexArray(vao);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        auto color = getColor(body.color);
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 0.0, 0.0, -40.0);
        glUniform3fv(glGetUniformLocation(shaderProgram, "inColor"), 1, &color[0]);
        glDrawElements(GL_TRIANGLES,16*32*2*3,GL_UNSIGNED_INT,0);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform3f(glGetUniformLocation(shaderProgram, "inColor"), 0.0, 0.0, 0.0);
        glDrawElements(GL_TRIANGLES,16*32*2*3,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }

    view = glm::mat4(glm::mat3(view));
    glUseProgram(skyboxProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(skyboxvao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxtex);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    glfwSwapBuffers(window);
}

glm::fvec3 Graphics::getColor(uint32_t color) {
    float r,g,b;
    r = ((color>>16)&0xFF)/255.0;
    g = ((color>>8)&0xFF)/255.0;
    b = ((color)&0xFF)/255.0;
    return glm::fvec3(r,g,b);
}

GLuint Graphics::loadCubemap(std::string dir) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    
    u_char* data = stbi_load((dir + "/right.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    data = stbi_load((dir + "/left.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    data = stbi_load((dir + "/top.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    data = stbi_load((dir + "/bottom.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    data = stbi_load((dir + "/front.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    data = stbi_load((dir + "/back.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Graphics::generateCube(float sideLen) {
    float halfSide = sideLen/2;
    vertices.clear();
    for (float i = -halfSide; i <= halfSide; i += sideLen)
        for (float j = -halfSide; j <= halfSide; j += sideLen)
            for (float k = -halfSide; k <= halfSide; k += sideLen)
                vertices.insert(vertices.end(), {i,j,k});
    indices.clear();
    indices.insert(indices.end(), {0,1,2,0,2,4,1,2,3,1,3,5,3,5,7,2,4,6,4,5,6,5,6,7,2,3,6,3,6,7,0,1,4,1,4,5});
}

void Graphics::generateSphere(float r, uint stacks, uint sectors) {
    vertices.clear();
    normals.clear();
    for (int i = 0; i < sectors; i++) {
        float theta = 2*M_PI*i/sectors;
        for (int j = 0; j <= stacks; j++) {
            float phi = M_PI_2-M_PI*j/stacks;
            float x = r*cos(phi)*cos(theta);
            float y = r*sin(phi);
            float z = r*cos(phi)*sin(theta);
            vertices.insert(vertices.end(), {x,y,z});
            normals.insert(normals.end(), {x/r,y/r,z/r});
        }
    }

    indices.clear();
    for (uint i = 0; i < sectors; i++) {
        for (uint j = 0; j < stacks; j += 1) {
            uint currSect = i*(stacks+1);
            uint nextSect = (i+1)*(stacks+1) % (sectors*(stacks+1));
            indices.insert(indices.end(), {currSect+j, currSect+j+1, nextSect+j+1});
            indices.insert(indices.end(), {currSect+j, nextSect+j+1, nextSect+j});
        }
    }
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
