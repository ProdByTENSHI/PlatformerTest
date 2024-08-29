#version 460 core

layout (location = 0) in vec4 posTex;	// x, y = Position; z, w = Texture Coords
layout (location = 1) in int spriteId;

layout (std430, location = 0) buffer Transforms {
	mat4 modelMatrices[];
};

out vec2 o_TextureCoords;

void main() {
	o_TextureCoords = posTex.zw;

	// modelMatrices[spriteId]; -> Model Matrix for this Sprite
	gl_Position = vec4(posTex.xy, 0.0, 1.0);
}