#version 450

precision mediump float;

out vec4 FragColor;

// Passed in from the vertex shader.
varying vec2 vTexcoord;

uniform uint invertXAxis;

uniform float regionX;
uniform float regionY;
uniform float regionWidth;
uniform float regionHeight;

uniform float animationX;
uniform float animationY;
uniform float animationWidth;
uniform float animationHeight;

uniform sampler2D uSampler;

uniform vec4 color;

void main()
{
    vec2 t = vTexcoord;
    t.y = 1.0 - t.y; // flip the texture along the y-Axis!

    t.x = t.x * regionWidth + regionX;
    t.y = t.y * regionHeight + regionY;

    t.x = t.x * animationWidth + animationX;
    t.y = t.y * animationHeight + animationY;

    if(invertXAxis != 0){
      t.x = (1.0 - t.x);
    }

    FragColor = texture2D(uSampler, t);

    if((FragColor.r + FragColor.g + FragColor.b) == 0){
      FragColor.a = 0;
    } else {
      FragColor.r += color.r;
      FragColor.g += color.g;
      FragColor.b += color.b;
      FragColor.a = color.a;
    }
}
