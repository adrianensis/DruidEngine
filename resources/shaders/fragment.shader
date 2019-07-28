#version 450

precision mediump float;

out vec4 FragColor;

// Passed in from the vertex shader.
varying vec2 vTexcoord;
varying lowp vec4 vColor;

uniform float animationX;
uniform float animationY;
uniform float animationWidth;
uniform float animationHeight;

uniform sampler2D uSampler;

void main()
{
    vec2 t = vTexcoord;
    t.y = 1.0 - t.y; // flip the texture along the y-Axis!

    t.x = t.x * animationWidth + animationX;
    t.y = t.y * animationHeight + animationY;

    FragColor = texture2D(uSampler, t);
}
