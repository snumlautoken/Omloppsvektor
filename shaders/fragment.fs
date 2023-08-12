#version 440 core
in vec3 normal;
in vec3 pos;

out vec4 outColor;

uniform vec3 inColor;
uniform vec3 lightPos;

void main() {
    float diff = max(dot(normalize(normal), normalize(lightPos-pos)), 0.0);
    outColor = vec4(inColor, 1.0);
}
