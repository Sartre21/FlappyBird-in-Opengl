#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;
uniform float scale;

uniform mat4 model;
uniform mat4 proj;

void main()
{
    gl_Position = proj * model * vec4(aPos.x + aPos.x * scale,aPos.y + aPos.y * scale,0.0, 1.0);
    texCoord = aTex;
}