#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
out vec2 Texcoord;
uniform mat4 mat_mvp;
//out vec4 vertexColor;

void main() {
    gl_Position = mat_mvp * vec4(aPos, 1.0);
    Texcoord = aTexcoord;
    //vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}
