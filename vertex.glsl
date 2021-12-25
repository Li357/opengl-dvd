#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform vec2 delta;

void main() {
    gl_Position = vec4(aPosition.xy + delta, aPosition.z, 1.0);
    texCoord = aTexCoord;
}