#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D u_BoxTexture;
uniform sampler2D u_FaceTexture;

void main() {
    FragColor = mix(texture(u_BoxTexture, TexCoord), texture(u_FaceTexture, TexCoord), 0.2);
}
