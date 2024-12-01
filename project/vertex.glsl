#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 fragColor;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(void) 
{
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(position, 1.0);
    
    fragColor = color;
}