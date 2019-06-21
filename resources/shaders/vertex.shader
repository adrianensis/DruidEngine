#version 450

layout (location = 0) in vec3 position;

uniform mat4 translationMatrix;

void main()
{
    gl_Position = translationMatrix * vec4(position.x, position.y, position.z, 1.0);
}
