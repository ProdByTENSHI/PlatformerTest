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

	void MasterRenderer::Render()
	{
		g_Ecs->m_SpriteRenderer->Render(*m_SpriteShader);
		g_Ecs->m_SpriteSheetRenderer->Render(*m_SpriteShader);

		g_Camera->Update(*m_SpriteShader);
	}
}