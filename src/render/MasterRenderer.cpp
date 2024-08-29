#include "render/MasterRenderer.h"

#include "core/Globals.h"
#include "resources/Vertex.h"

namespace tenshi
{
	MasterRenderer::MasterRenderer()
	{
		m_SpriteShader = g_ResourceManager->GetShader("sprite");
		m_SpriteShader->Bind();

		m_TransformsSsbo = new Ssbo(sizeof(glm::mat4) * g_MAX_SPRITES, "Transforms");
		m_TransformsSsbo->BindToShader(*m_SpriteShader.get(), SPRITE_TRANSFORMS_SSBO_BIMDING_POINT);

		m_SpriteShader->Unbind();
	}

	void MasterRenderer::AddSpriteEntity(SpriteEntity& entity)
	{
		for (auto& batch : m_SpriteBatches)
		{
			if (!batch->IsTextureInBatch(entity))
				continue;

			batch->m_Entities.push_back(&entity);
		}

		SpriteBatch* batch = new SpriteBatch(m_SpriteBatches.size());
		batch->m_Entities.push_back(&entity);

		// -- GL Buffers
		glCreateVertexArrays(1, &batch->m_Vao);
		glCreateBuffers(1, &batch->m_Vbo);

		glNamedBufferStorage(batch->m_Vbo, MAX_SPRITES_PER_BATCH * sizeof(Vertex), &GetVertices()[0],
			GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(batch->m_Vao, 0);
		glVertexArrayAttribFormat(batch->m_Vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(batch->m_Vao, 0, 0);

		glVertexArrayVertexBuffer(batch->m_Vao, 0, batch->m_Vbo, 0, sizeof(Vertex));

		m_SpriteBatches.push_back(batch);
	}

	void MasterRenderer::Render()
	{
		// -- Render Batched Sprites
		m_SpriteShader->Bind();

		for (auto& batch : m_SpriteBatches)
		{
			u32 _baseOffset = 0;
			for (u32 i = 0; i < batch->m_Entities.size(); i++)
			{
				auto& entity = batch->m_Entities[i];

				m_TransformsSsbo->SubBufferData((_baseOffset + i) * sizeof(glm::mat4),
					sizeof(glm::mat4), glm::value_ptr(entity->m_Transform.m_ModelMatrix));
			}

			glBindVertexArray(batch->m_Vao);
			batch->m_Entities[0]->m_Sprite->m_Texture->Bind();

			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, batch->m_Entities.size(), _baseOffset);

			batch->m_Entities[0]->m_Sprite->m_Texture->Unbind();
			glBindVertexArray(0);

			_baseOffset += batch->m_Entities.size();
		}

		g_Camera->Update(*m_SpriteShader);

		// -- Render Spritesheets

	}
}