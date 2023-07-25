#version 330 core
out vec4 outColor;

void main() {
    float scale = (1-pow(2*gl_PointCoord.x-1,2)-pow(2*gl_PointCoord.y-1,2));
    outColor = scale*vec4(1.0, 0.0, 1.0, 1.0);
}
