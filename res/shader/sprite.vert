#version 460 core

layout (location = 0) in vec4 posTex;	// x, y = Position; z, w = Texture Coords

layout (std430, binding = 0) buffer Transforms {
	mat4 modelMatrices[];
};

// -> Has to be the same Value as MAX_SPRITES_PER_BATCH in the Code
const int MAX_SPRITES = 1000;
layout (shared) uniform EntityIds {
	int ids[MAX_SPRITES];
};

out flat int o_EntityId;
out vec2 o_TextureCoords;

uniform mat4 u_ViewOrtho;

void main() {
	o_TextureCoords = vec2(posTex.z, posTex.w);
	o_EntityId = ids[gl_BaseInstance + gl_InstanceID];

	gl_Position = u_ViewOrtho * modelMatrices[o_EntityId] * vec4(posTex.xy, 0.0, 1.0);
}