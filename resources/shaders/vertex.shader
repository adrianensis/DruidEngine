#version 450

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec4 color;

varying lowp vec4 vColor;
varying vec2 vTexcoord;

void main()
{
  gl_Position = projectionMatrix * viewMatrix * ( (translationMatrix * scaleMatrix) * rotationMatrix ) * vec4(position.x, position.y, position.z, 1.0);

  // Pass the texcoord to the fragment shader.
  vTexcoord = texcoord;
  vColor = color;
}
