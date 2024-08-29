#version 460 core

layout (location = 0) in vec4 posTex;	// x, y = Position; z, w = Texture Coords

layout (std430, binding = 0) buffer Transforms {
	mat4 modelMatrices[];
};

out vec2 o_TextureCoords;

uniform mat4 u_ViewOrtho;

void main() {
	o_TextureCoords = posTex.zw;

	// modelMatrices[spriteId]; -> Model Matrix for this Sprite
	gl_Position = u_ViewOrtho * modelMatrices[gl_BaseInstance + gl_InstanceID] * vec4(posTex.xy, 0.0, 1.0);
}