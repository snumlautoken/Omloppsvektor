#include <algorithm>
#include <iostream>
#include "input.hpp"

Input::Input(GLFWwindow* w) : window(w) {
    time = glfwGetTime();
    glfwGetCursorPos(window, &prevX, &prevY);
}

void Input::pollEvents() {
    double newTime = glfwGetTime();
    double dT = newTime - time;
    time = newTime;

    glfwPollEvents();
    processKb(dT);
    processM(dT);
}

void Input::processKb(double dT) {
    float speed = 1.0 * dT;
    glm::vec3 movePos(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)           { movePos += dir; };
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)           { movePos -= glm::normalize(glm::cross(dir, up)); };
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)           { movePos -= dir; };
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)           { movePos += glm::normalize(glm::cross(dir, up)); };
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)       { movePos += up; };
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)  { movePos -= up; };

    movePos = glm::length(movePos) > 1.0 ? glm::normalize(movePos) : movePos;
    pos += movePos * speed;
}

void Input::processM(double dT) {
    double newX, newY;
    glfwGetCursorPos(window, &newX, &newY);
    float xoffset = newX - prevX;
    float yoffset = prevY - newY;
    prevX = newX;
    prevY = newY;

    yaw = fmod(yaw + xoffset*0.1, 360.0);
    pitch += yoffset*0.1;

    pitch = std::clamp<float>(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir = glm::normalize(direction);
}

glm::mat4 Input::getView() {
    return glm::lookAt(pos, pos+dir, up);
}
