#version 420

uniform float time;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 positionOffset;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec4 color;

out vec2 vTexcoord;
out vec4 vColor;

void main()
{
  mat4 PV_Matrix = projectionMatrix * viewMatrix;
  
  gl_Position = PV_Matrix * vec4(position.x, position.y, position.z, 1.0);

  // Pass data to the fragment shader.
  vTexcoord = texcoord;
  vColor = color;
}
