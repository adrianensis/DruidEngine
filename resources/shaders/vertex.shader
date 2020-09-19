#version 420

uniform float time;
uniform bool isAffectedByProjection;

uniform mat4 viewTranslationMatrix;
uniform mat4 viewRotationMatrix;
uniform mat4 projectionMatrix;
uniform vec4 positionOffset;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

varying vec2 vTexcoord;

void main()
{
  mat4 PV_Matrix = mat4(1.0);

  if(isAffectedByProjection) {
   PV_Matrix = projectionMatrix * (viewRotationMatrix * viewTranslationMatrix);
  }
  
  gl_Position = PV_Matrix * vec4(position.x, position.y, position.z, 1.0);

  // Pass the texcoord to the fragment shader.
  vTexcoord = texcoord;
}
