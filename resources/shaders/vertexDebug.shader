#version 420

uniform mat4 viewTranslationMatrix;
uniform mat4 viewRotationMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 position;


void main()
{
  gl_Position = projectionMatrix * (viewRotationMatrix * viewTranslationMatrix) * vec4(position.x, position.y, position.z, 1.0);

}
