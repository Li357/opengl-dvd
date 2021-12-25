#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D aTexture;
uniform vec3 dvdColor;

void main() {
    FragColor = texture(aTexture, texCoord) * vec4(dvdColor, 1.0);
}