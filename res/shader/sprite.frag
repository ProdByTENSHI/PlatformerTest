#version 460 core

uniform sampler2D u_Texture;

in vec2 o_TextureCoords;
in flat int o_EntityId;

out vec4 o_FragColor;

void main() {
	o_FragColor = texture(u_Texture, o_TextureCoords);
//	float _color = o_EntityId / 12.0;
//	o_FragColor = vec4(_color, _color, _color, 1.0);
}