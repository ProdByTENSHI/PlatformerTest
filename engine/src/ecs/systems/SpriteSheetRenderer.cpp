#include "ecs/systems/SpriteSheetRenderer.h"

#include "core/Globals.h"
#include "ecs/components/SpriteSheetComponent.h"
#include "ecs/components/TransformComponent.h"
#include "tenshiUtil/container/STLUtil.h"

namespace tenshi {
SpriteSheetRenderer::SpriteSheetRenderer(Signature signature, u8 id)
    : System(signature, id) {
  EventHandler<Entity, Signature> _onSignatureChange([this](Entity entity,
                                                            Signature signature)
                                                         -> i32 {
    if (m_Signature == signature) {
      TransformComponent &_transform =
          *g_Ecs->GetComponent<TransformComponent>(entity);
      SpriteSheetComponent &_spriteSheet =
          *g_Ecs->GetComponent<SpriteSheetComponent>(entity);

      for (i32 i = 0; i < m_Batches.size(); i++) {
        auto &_batch = m_Batches[i];
        if (!_batch->IsTextureInBatch(_spriteSheet.m_SpriteSheet->m_Texture))
          continue;

        // -- Subscribe to Transform onChangeEvent to only change the Transforms
        // Buffer when Data changed
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

        _batch->m_FrameToEntityId[_spriteSheet.m_CurrentFrame].push_back(
            entity);

        // -- Subscribe to OnFrameChange Event
        EventHandler<u32, u32> _onFrameChange(
            [&_spriteSheet, entity, _batch](u32 previous, u32 index) {
              _batch->MoveEntity(entity, previous, index);
            });
        _spriteSheet.m_OnFrameChange.Subscribe(_onFrameChange);

        std::cout << "[SPRITESHEET RENDERER] Added Spritesheet to Batch " << i
                  << std::endl;

        return i;
      }

      SpriteSheetBatch *batch =
          new SpriteSheetBatch(_spriteSheet.m_SpriteSheet->m_Texture);

      // -- Setup GL Buffers
      glCreateVertexArrays(1, &batch->m_Vao);
      glCreateBuffers(1, &batch->m_Vbo);

      // -- Setup UV Coords Translation
      /*	Indexing into the Vbo:
       *	Index * (sizeof(Vertex) * 6) to get the Vertices for the wanted
       *Frame
       *	|--0--|--1--|--2--|
       *	|--3--|--4--|--5--|
       *	|--6--|--7--|--8--|
       */

      // Create hFrames * vFrames Squares in the Vbo and assign the relevant
      // Vertex Data to each of them ! Sometimes throws an Write Access
      // Violation?
      const u32 _FRAME_COUNT = _spriteSheet.m_SpriteSheet->H_FRAMES *
                               _spriteSheet.m_SpriteSheet->V_FRAMES;
      glNamedBufferStorage(batch->m_Vbo,
                           _spriteSheet.m_SpriteSheet->FRAME_COUNT * 6 *
                               sizeof(Vertex),
                           nullptr, GL_DYNAMIC_STORAGE_BIT);

      // Iterate through all Frames, Create Vertices for current Frame and put
      // it into the Vertex Buffer When Drawing instead of recalculating we just
      // index into the part of the VBO that contains the cached Vertices
      std::vector<Vertex> _vertices(GetVertices());
      for (i32 i = 0; i < _FRAME_COUNT; i++) {
        _spriteSheet.m_SpriteSheet->SetTextureCoordsOfFrame(i, _vertices);
        glNamedBufferSubData(batch->m_Vbo, i * (sizeof(Vertex) * 6),
                             sizeof(Vertex) * 6, &_vertices[0]);
        batch->m_FrameToEntityId.insert(std::make_pair(i, std::vector<u32>()));
      }

      glEnableVertexArrayAttrib(batch->m_Vao, 0);
      glVertexArrayAttribFormat(batch->m_Vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
      glVertexArrayAttribBinding(batch->m_Vao, 0, 0);
      glVertexArrayVertexBuffer(batch->m_Vao, 0, batch->m_Vbo, 0,
                                sizeof(Vertex));

      // -- Subscribe to Transform onChangeEvent to only change the Transforms
      // Buffer when Data changed
      EventHandler<glm::mat4, glm::vec2, glm::vec2, glm::vec2>
          _onTransformChange(
              [this, entity](const glm::mat4 &modelMatrix, const glm::vec2 &pos,
                             const glm::vec2 &rot, const glm::vec2 &scale) {
                g_MasterRenderer->m_TransformsSsbo.SubBufferData(
                    entity * sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(modelMatrix));
              });

      _transform.onTransformChange.Subscribe(_onTransformChange);

      g_MasterRenderer->m_TransformsSsbo.SubBufferData(
          entity * sizeof(glm::mat4), sizeof(glm::mat4),
          glm::value_ptr(_transform.m_ModelMatrix));

      // -- Subscribe to OnFrameChange Event
      EventHandler<u32, u32> _onFrameChange(
          [&_spriteSheet, entity, batch](u32 previous, u32 index) {
            batch->MoveEntity(entity, previous, index);
          });
      _spriteSheet.m_OnFrameChange.Subscribe(_onFrameChange);

      m_Batches.push_back(batch);

      // -- Add to Frame to EntityId
      batch->m_FrameToEntityId.insert(
          std::make_pair(_spriteSheet.m_CurrentFrame, std::vector<Entity>()));
      batch->m_FrameToEntityId[_spriteSheet.m_CurrentFrame].push_back(entity);

      m_Entities.push_back(entity);
      std::cout
          << "[SPRITESHEET RENDERER] Create new Spritesheet Batch for Entity "
          << entity << std::endl;
      return m_Batches.size() - 1;
    } else {
      std::vector<Entity>::iterator _it =
          STLUtil::GetItOfElementInVec<Entity>(m_Entities, entity);
      if (_it == m_Entities.end())
        return -1;

      for (auto &batch : m_Batches) {
        if (!batch->IsEntityInBatch(entity))
          continue;

        batch->RemoveEntity(entity);
        break;
      }

      m_Entities.erase(_it);

      return -1;
    }
  });
  g_Ecs->m_OnEntitySignatureChange.Subscribe(_onSignatureChange);
}

void SpriteSheetRenderer::Render(Shader &shader) {
  i32 _baseOffset = 0;
  for (auto &batch : m_Batches) {
    glBindVertexArray(batch->m_Vao);
    batch->m_Texture->Bind();

    // Instance all Frames of this Spritesheet that have at least 1 Entity at
    // this Stage Send Entity IDs for this Draw Calls to the GPU
    for (i32 i = 0; i < batch->m_FrameToEntityId.size(); i++) {
      if (batch->m_FrameToEntityId[i].size() <= 0)
        continue;

      for (i32 j = 0; j < batch->m_FrameToEntityId[i].size(); j++) {
        g_MasterRenderer->m_EntityIdsUbo.SubBufferData(
            (_baseOffset + j) * sizeof(i32), sizeof(i32),
            &batch->m_FrameToEntityId[i][j]);
      }

      // Offset is in Vertices not in Bytes!
      glDrawArraysInstancedBaseInstance(GL_TRIANGLES, i * 6, 6,
                                        batch->m_FrameToEntityId[i].size(),
                                        _baseOffset);
      _baseOffset += batch->m_FrameToEntityId[i].size();
    }

    batch->m_Texture->Unbind();
    glBindVertexArray(0);

    _baseOffset = 0;
  }
}
} // namespace tenshi
