#include "render/MasterRenderer.h"

#include "core/Globals.h"
#include "resources/Vertex.h"

namespace tenshi
{
	MasterRenderer::MasterRenderer()
	{
		m_SpriteShader = g_ResourceManager->GetShader("sprite");
		m_SpriteShader->Bind();

		m_TransformsSsbo.Create(sizeof(glm::mat4) * g_MAX_SPRITES, "Transforms");
		m_TransformsSsbo.BindToShader(*m_SpriteShader.get(), SPRITE_TRANSFORMS_SSBO_BIMDING_POINT);

		m_EntityIdsUbo.Create(sizeof(i32) * MAX_SPRITES_PER_BATCH, "EntityIds");
		m_EntityIdsUbo.BindToShader(*m_SpriteShader.get(), ENTITY_IDS_UBO_BIINDING_POINT);
	}

	void MasterRenderer::AddEntity(u32 entityId, std::shared_ptr<Texture> texture)
	{
		for (auto& batch : m_SpriteBatches)
		{
			if (!batch->IsTextureInBatch(texture) || batch->m_EntityIds.size() >= MAX_SPRITES_PER_BATCH)
				continue;

			batch->m_EntityIds.push_back(entityId);

			// -- Subscribe to Transform onChangeEvent to only change the Transforms Buffer when Data changed
			EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2> _onTransformChange
			([this, entityId](const glm::mat4& modelMatrix, const glm::vec2& pos, const glm::vec2& rot, const glm::vec2& scale)
				{
					m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
					glm::value_ptr(modelMatrix));

			std::cout << "Entity " << entityId << " Transform was changed!" << std::endl;
				});

			Entity& _entity = *g_EntityManager->GetEntity<Entity>(entityId);
			_entity.m_Transform.onTransformChange.Subscribe(_onTransformChange);

			m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
				glm::value_ptr(_entity.m_Transform.m_ModelMatrix));
			return;
		}

		SpriteBatch* batch = new SpriteBatch(texture);
		batch->m_EntityIds.push_back(entityId);

		// -- GL Buffers
		glCreateVertexArrays(1, &batch->m_Vao);
		glCreateBuffers(1, &batch->m_Vbo);

		glNamedBufferStorage(batch->m_Vbo, sizeof(Vertex) * 6, &GetVertices()[0],
			GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(batch->m_Vao, 0);
		glVertexArrayAttribFormat(batch->m_Vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(batch->m_Vao, 0, 0);
		glVertexArrayVertexBuffer(batch->m_Vao, 0, batch->m_Vbo, 0, sizeof(Vertex));

		// -- Subscribe to Transform onChangeEvent to only change the Transforms Buffer when Data changed
		EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2> _onTransformChange
		([this, entityId](const glm::mat4& modelMatrix, const glm::vec2& pos, const glm::vec2& rot, const glm::vec2& scale)
			{
				m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
				glm::value_ptr(modelMatrix));

		std::cout << "Entity " << entityId << " Transform was changed!" << std::endl;
			});

		Entity& _entity = *g_EntityManager->GetEntity<Entity>(entityId);
		_entity.m_Transform.onTransformChange.Subscribe(_onTransformChange);

		m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
			glm::value_ptr(_entity.m_Transform.m_ModelMatrix));

		m_SpriteBatches.push_back(batch);
	}

	void MasterRenderer::Render()
	{
		i32 _baseOffset = 0;
		for (auto& batch : m_SpriteBatches)
		{
			glBindVertexArray(batch->m_Vao);
			batch->m_Texture->Bind();

			for (i32 i = 0; i < batch->m_EntityIds.size(); i++)
			{
				m_EntityIdsUbo.SubBufferData((_baseOffset + i) * sizeof(i32), sizeof(i32),
					&batch->m_EntityIds[i]);
			}

			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, batch->m_EntityIds.size(), _baseOffset);

			batch->m_Texture->Unbind();
			glBindVertexArray(0);

			_baseOffset += batch->m_EntityIds.size();
		}

		g_Camera->Update(*m_SpriteShader);
	}
}