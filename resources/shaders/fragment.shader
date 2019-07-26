#version 450

out vec4 FragColor;

// Passed in from the vertex shader.
varying vec2 vTexcoord;
varying lowp vec4 vColor;

uniform sampler2D uSampler;

void main()
{
    vec2 t = vTexcoord;
    t.y = 1.0 - t.y; // flip the texture along the y-Axis!

    FragColor = texture2D(uSampler, t);
}
