#version 420

precision mediump float;

varying vec4 vColor;

out vec4 FragColor;

void main()
{
    FragColor = vColor;
}
