#version 330 core
out vec4 FragColor;

uniform float vertexColor;

void main()
{
    FragColor = vec4(vertexColor,vertexColor * 2, vertexColor * 4, 1.0);
}
