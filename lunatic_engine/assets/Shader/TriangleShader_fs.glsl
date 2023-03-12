#version 330 core
in vec2 Texcoord;
out vec4 FragColor;
//in vec4 vertexColor;
uniform sampler2D _Albedo;
void main() {
    FragColor = texture(_Albedo,Texcoord);
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
