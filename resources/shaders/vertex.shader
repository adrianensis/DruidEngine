#version 420

uniform float time;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 positionOffset;
uniform bool isInstanced;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec4 color;
layout (location = 3) in mat4 modelMatrix;

out vec2 vTexcoord;
out vec4 vColor;

void main()
{
  mat4 PV_Matrix = projectionMatrix * viewMatrix;

  vec4 finalPositon = vec4(position, 1.0);

  if(isInstanced)
  {
    finalPositon = modelMatrix * finalPositon;
  }
  
  gl_Position = PV_Matrix * finalPositon;

  // Pass data to the fragment shader.
  vTexcoord = texcoord;
  vColor = color;
}
