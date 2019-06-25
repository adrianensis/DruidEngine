#version 450

out vec4 FragColor;

// Passed in from the vertex shader.
varying vec2 vTexcoord;
varying lowp vec4 vColor;

void main()
{
    FragColor = vColor;
}
