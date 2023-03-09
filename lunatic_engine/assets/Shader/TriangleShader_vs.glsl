#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mat_mvp;
//out vec4 vertexColor;

void main() {
    gl_Position = mat_mvp * vec4(aPos, 1.0);
    //vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}
