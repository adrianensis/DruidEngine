#version 420

uniform mat4 viewTranslationMatrix;
uniform mat4 viewRotationMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 vColor;

void main()
{
  gl_Position = projectionMatrix * (viewRotationMatrix * viewTranslationMatrix) * vec4(position.x, position.y, position.z, 1.0);
  vColor = color;
}
