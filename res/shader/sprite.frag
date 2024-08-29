#version 460 core

uniform sampler2D u_Texture;

in vec2 o_TextureCoords;

out vec4 o_FragColor;

void main() {
	o_FragColor = texture(u_Texture, o_TextureCoords);
}