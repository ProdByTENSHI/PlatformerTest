#include "render/MasterRenderer.h"

#include "core/Globals.h"
#include "resources/Vertex.h"

namespace tenshi
{
	MasterRenderer::MasterRenderer()
	{
		m_SpriteShader = g_ResourceManager->GetShader("sprite");

		m_TransformsSsbo = new Ssbo(sizeof(glm::mat4) * g_MAX_SPRITES, "Transforms");
		m_TransformsSsbo->BindToShader(*m_SpriteShader.get(), SPRITE_TRANSFORMS_SSBO_BIMDING_POINT);
	}

	void MasterRenderer::AddSpriteEntity(SpriteEntity& entity)
	{
		for (auto& batch : m_SpriteBatches)
		{
			if (!batch->IsTextureInBatch(entity))
				continue;

			batch->m_Entities.push_back(&entity);
		}

		SpriteBatch* batch = new SpriteBatch();
		batch->m_Entities.push_back(&entity);

		// -- GL Buffers
		glCreateVertexArrays(1, &batch->m_Vao);
		glCreateBuffers(1, &batch->m_Vbo);

		glNamedBufferStorage(batch->m_Vbo, MAX_SPRITES_PER_BATCH * sizeof(Vertex), nullptr,
			GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(batch->m_Vao, 0);
		glEnableVertexArrayAttrib(batch->m_Vao, 1);

		glVertexArrayAttribFormat(batch->m_Vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Position));
		glVertexArrayAttribFormat(batch->m_Vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_TexCoords));

		glVertexArrayAttribBinding(batch->m_Vao, 0, 0);
		glVertexArrayAttribBinding(batch->m_Vao, 1, 0);

		glVertexArrayVertexBuffer(batch->m_Vao, 0, batch->m_Vbo, 0, sizeof(Vertex));
	}

	void MasterRenderer::Render()
	{
		// -- Render Batched Sprites
		m_SpriteShader->Bind();

		for (auto& batch : m_SpriteBatches)
		{
			for (auto& entity : batch->m_Entities)
			{
				m_TransformsSsbo->SubBufferData(entity->m_EntityId * sizeof(glm::mat4),
					sizeof(glm::mat4), glm::value_ptr(entity->m_Transform.m_ModelMatrix));
			}

			glBindVertexArray(batch->m_Vao);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, batch->m_Entities.size());
			glBindVertexArray(0);
		}

		// -- Render Spritesheets
	}
}