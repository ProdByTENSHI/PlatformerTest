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

	i32 MasterRenderer::AddStaticEntity(u32 entityId, std::shared_ptr<Texture> texture)
	{
		for (i32 i = 0; i < m_StaticSpriteBatches.size(); i++)
		{
			auto& _batch = m_StaticSpriteBatches[i];
			if (!_batch->IsTextureInBatch(texture) || _batch->m_EntityIds.size() >= MAX_SPRITES_PER_BATCH)
				continue;

			_batch->m_EntityIds.push_back(entityId);

			// -- Subscribe to Transform onChangeEvent to only change the Transforms Buffer when Data changed
			EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2> _onTransformChange
			([this, entityId](const glm::mat4& modelMatrix, const glm::vec2& pos, const glm::vec2& rot, const glm::vec2& scale)
				{
					m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
					glm::value_ptr(modelMatrix));
				});

			Entity& _entity = *g_EntityManager->GetEntity<Entity>(entityId);
			_entity.m_Transform.onTransformChange.Subscribe(_onTransformChange);

			m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
				glm::value_ptr(_entity.m_Transform.m_ModelMatrix));
			return i;
		}

		SpriteBatch* batch = new SpriteBatch(texture);
		batch->m_EntityIds.push_back(entityId);

		// -- Setup GL Buffers
		glCreateVertexArrays(1, &batch->m_Vao);
		glCreateBuffers(1, &batch->m_Vbo);

		// ! Sometimes throws an Write Access Violation?
		glNamedBufferStorage(batch->m_Vbo, 6 * sizeof(Vertex), &GetVertices()[0],
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
			});

		Entity& _entity = *g_EntityManager->GetEntity<Entity>(entityId);
		_entity.m_Transform.onTransformChange.Subscribe(_onTransformChange);

		m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
			glm::value_ptr(_entity.m_Transform.m_ModelMatrix));

		m_StaticSpriteBatches.push_back(batch);
		return m_StaticSpriteBatches.size() - 1;
	}

	i32 MasterRenderer::AddDynamicEntity(u32 entityId, SpriteSheet& spriteSheet)
	{
		SpriteSheetEntity& _entity = *g_EntityManager->GetEntity<SpriteSheetEntity>(entityId);

		for (i32 i = 0; i < m_DynamicSpriteBatches.size(); i++)
		{
			auto& _batch = m_DynamicSpriteBatches[i];
			if (!_batch->IsTextureInBatch(spriteSheet.m_Texture))
				continue;

			// -- Subscribe to Transform onChangeEvent to only change the Transforms Buffer when Data changed
			EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2> _onTransformChange
			([this, entityId](const glm::mat4& modelMatrix, const glm::vec2& pos, const glm::vec2& rot, const glm::vec2& scale)
				{
					m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
					glm::value_ptr(modelMatrix));
				});

			_entity.m_Transform.onTransformChange.Subscribe(_onTransformChange);

			m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
				glm::value_ptr(_entity.m_Transform.m_ModelMatrix));

			_batch->m_FrameToEntityId[_entity.m_CurrentFrame].push_back(entityId);

			return i;
		}

		SpriteSheetBatch* batch = new SpriteSheetBatch(spriteSheet.m_Texture);

		// -- Setup GL Buffers
		glCreateVertexArrays(1, &batch->m_Vao);
		glCreateBuffers(1, &batch->m_Vbo);

		// -- Setup UV Coords Translation
		/*	Indexing into the Vbo:
		*	Index * (sizeof(Vertex) * 6) to get the Vertices for the wanted Frame
		*	|--0--|--1--|--2--|
		*	|--3--|--4--|--5--|
		*	|--6--|--7--|--8--|
		*/

		// Create hFrames * vFrames Squares in the Vbo and assign the relevant Vertex Data to each of them
		// ! Sometimes throws an Write Access Violation?
		const u32 _FRAME_COUNT = spriteSheet.H_FRAMES * spriteSheet.V_FRAMES;
		glNamedBufferStorage(batch->m_Vbo, spriteSheet.FRAME_COUNT * 6 * sizeof(Vertex), nullptr,
			GL_DYNAMIC_STORAGE_BIT);

		// Iterate through all Frames, Create Vertices for current Frame and put it into the Vertex Buffer
		// When Drawing instead of recalculating we just index into the part of the VBO that contains the cached Vertices
		std::vector<Vertex> _vertices(GetVertices());
		for (i32 i = 0; i < _FRAME_COUNT; i++)
		{
			spriteSheet.SetTextureCoordsOfFrame(i, _vertices);
			glNamedBufferSubData(batch->m_Vbo, i * (sizeof(Vertex) * 6), sizeof(Vertex) * 6, &_vertices[0]);
			batch->m_FrameToEntityId.insert(std::make_pair(i, std::vector<u32>()));
		}

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
			});

		_entity.m_Transform.onTransformChange.Subscribe(_onTransformChange);

		m_TransformsSsbo.SubBufferData(entityId * sizeof(glm::mat4), sizeof(glm::mat4),
			glm::value_ptr(_entity.m_Transform.m_ModelMatrix));

		// -- Add to Frame to EntityId
		batch->m_FrameToEntityId.insert(std::make_pair(_entity.m_CurrentFrame, std::vector<u32>()));
		batch->m_FrameToEntityId[_entity.m_CurrentFrame].push_back(entityId);

		m_DynamicSpriteBatches.push_back(batch);

		return m_DynamicSpriteBatches.size() - 1;
	}

	void MasterRenderer::Render()
	{
		// -- Render Static Sprites
		i32 _baseOffset = 0;
		for (auto& batch : m_StaticSpriteBatches)
		{
			glBindVertexArray(batch->m_Vao);
			batch->m_Texture->Bind();

			// Send Entity IDs for this Draw Calls to the GPU
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

		// -- Render Dynamic Sprites
		_baseOffset = 0;
		for (auto& batch : m_DynamicSpriteBatches)
		{
			glBindVertexArray(batch->m_Vao);
			batch->m_Texture->Bind();

			// Send Entity IDs for this Draw Calls to the GPU
			for (i32 i = 0; i < batch->m_FrameToEntityId.size(); i++)
			{
				if (batch->m_FrameToEntityId[i].size() <= 0)
					continue;

				for (i32 j = 0; j < batch->m_FrameToEntityId[i].size(); j++)
				{
					m_EntityIdsUbo.SubBufferData((_baseOffset + j) * sizeof(i32), sizeof(i32),
						&batch->m_FrameToEntityId[i][j]);
				}

				// Offset is in Vertices not in Bytes!
				glDrawArraysInstancedBaseInstance(GL_TRIANGLES, i * 6, 6, batch->m_FrameToEntityId[i].size(), _baseOffset);
				_baseOffset += batch->m_FrameToEntityId[i].size();
			}

			batch->m_Texture->Unbind();
			glBindVertexArray(0);

			_baseOffset = 0;
		}

		g_Camera->Update(*m_SpriteShader);
	}

	SpriteBatch* MasterRenderer::GetSpriteBatch(u32 index)
	{
		return m_StaticSpriteBatches[index];
	}

	SpriteSheetBatch* MasterRenderer::GetSpriteSheetBatch(u32 index)
	{
		return m_DynamicSpriteBatches[index];
	}
}