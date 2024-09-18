#include "ecs/systems/SpriteRenderer.h"

#include "core/Globals.h"
#include "ecs/components/SpriteComponent.h"
#include "ecs/components/TransformComponent.h"
#include "tenshiUtil/container/STLUtil.h"

namespace tenshi {
SpriteRenderer::SpriteRenderer(Signature signature, u8 id)
    : System(signature, id) {
  EventHandler<Entity, Signature> _onSignatureChange(
      [this](Entity entity, Signature signature) -> i32 {
        if (m_Signature == signature) {
          TransformComponent &_transform =
              *g_Ecs->GetComponent<TransformComponent>(entity);
          SpriteComponent &_sprite =
              *g_Ecs->GetComponent<SpriteComponent>(entity);

          // Check if a Batch with the same Texture exists that can store this
          // Entity and add it to this Batch if possible
          for (i32 i = 0; i < m_Batches.size(); i++) {
            auto &_batch = m_Batches[i];
            if (!_batch->IsTextureInBatch(_sprite.m_Texture) ||
                _batch->m_Entities.size() >= MAX_SPRITES_PER_BATCH)
              continue;

            _batch->m_Entities.push_back(entity);

            // -- Subscribe to Transform onChangeEvent to only change the
            // Transforms Buffer when Data changed
            EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2>
                _onTransformChange([this, entity](const glm::mat4 &modelMatrix,
                                                  const glm::vec2 &pos,
                                                  const glm::vec2 &rot,
                                                  const glm::vec2 &scale) {
                  g_MasterRenderer->m_TransformsSsbo.SubBufferData(
                      entity * sizeof(glm::mat4), sizeof(glm::mat4),
                      glm::value_ptr(modelMatrix));
                });

            _transform.onTransformChange.Subscribe(_onTransformChange);

            g_MasterRenderer->m_TransformsSsbo.SubBufferData(
                entity * sizeof(glm::mat4), sizeof(glm::mat4),
                glm::value_ptr(_transform.m_ModelMatrix));
            std::cout << "MOKÖ" << std::endl;
            return i;
          }

          // If no Sprite Batch was found that met the requirements create a new
          // one for this Entity
          SpriteBatch *batch = new SpriteBatch(_sprite.m_Texture);
          batch->m_Entities.push_back(entity);

          // -- Setup GL Buffers
          glCreateVertexArrays(1, &batch->m_Vao);
          glCreateBuffers(1, &batch->m_Vbo);

          // ! Sometimes throws an Write Access Violation?
          glNamedBufferStorage(batch->m_Vbo, 6 * sizeof(Vertex),
                               &GetVertices()[0], GL_DYNAMIC_STORAGE_BIT);

          glEnableVertexArrayAttrib(batch->m_Vao, 0);
          glVertexArrayAttribFormat(batch->m_Vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
          glVertexArrayAttribBinding(batch->m_Vao, 0, 0);
          glVertexArrayVertexBuffer(batch->m_Vao, 0, batch->m_Vbo, 0,
                                    sizeof(Vertex));

          // -- Subscribe to Transform onChangeEvent to only change the
          // Transforms Buffer when Data changed
          EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2>
              _onTransformChange([this, entity](const glm::mat4 &modelMatrix,
                                                const glm::vec2 &pos,
                                                const glm::vec2 &rot,
                                                const glm::vec2 &scale) {
                g_MasterRenderer->m_TransformsSsbo.SubBufferData(
                    entity * sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(modelMatrix));
              });

          _transform.onTransformChange.Subscribe(_onTransformChange);

          g_MasterRenderer->m_TransformsSsbo.SubBufferData(
              entity * sizeof(glm::mat4), sizeof(glm::mat4),
              glm::value_ptr(_transform.m_ModelMatrix));

          m_Batches.push_back(batch);
          m_Entities.push_back(entity);

          std::cout << "[SPRITE RENDERER] Created new Batch for Texture "
                    << g_Ecs->GetComponent<SpriteComponent>(entity)
                           ->m_Texture->GetName()
                    << std::endl;
          return m_Batches.size() - 1;
        } else {
          std::vector<Entity>::iterator _it =
              STLUtil::GetItOfElementInVec<Entity>(m_Entities, entity);
          if (_it == m_Entities.end())
            return 0;

          for (auto &batch : m_Batches) {
            if (!batch->IsEntityInBatch(entity))
              continue;

            batch->RemoveEntity(entity);
            break;
          }

          m_Entities.erase(_it);
        }

        return -1;
      });
  g_Ecs->m_OnEntitySignatureChange.Subscribe(_onSignatureChange);
}

SpriteRenderer::~SpriteRenderer() {
  for (auto &batch : m_Batches) {
    delete batch;
  }
}

void SpriteRenderer::Render(Shader &shader) {
  i32 _baseOffset = 0;
  for (auto &batch : m_Batches) {
    glBindVertexArray(batch->m_Vao);
    batch->m_Texture->Bind();

    // Send Entity IDs for this Draw Calls to the GPU
    for (i32 i = 0; i < batch->m_Entities.size(); i++) {
      g_MasterRenderer->m_EntityIdsUbo.SubBufferData(
          (_baseOffset + i) * sizeof(i32), sizeof(i32), &batch->m_Entities[i]);
    }

    glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6,
                                      batch->m_Entities.size(), _baseOffset);

    batch->m_Texture->Unbind();
    glBindVertexArray(0);

    _baseOffset += batch->m_Entities.size();
  }
}
} // namespace tenshi
