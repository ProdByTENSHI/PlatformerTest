#include "ecs/Ecs.h"

#include <fstream>

namespace tenshi {
Ecs::Ecs() {}

Ecs::~Ecs() {}

Entity Ecs::CreateEntity() {
  Entity _entity = GetNextEntityId();
  ++m_EntityCount;

  return _entity;
}

void Ecs::DestroyEntity(Entity entity) {
  for (auto &arr : m_ComponentArrays) {
    arr.second->OnEntityDestroyed(entity);
  }

  m_EntitySignatures[entity].reset();
  m_OnEntitySignatureChange.Dispatch(entity, m_EntitySignatures[entity]);

  m_EntityIdStack.push(entity);
  --m_EntityCount;

  std::cout << m_EntityCount << std::endl;

  std::cout << "Destroyed Entity " << entity << std::endl;
}

void Ecs::Serialize() {
  std::cout << "[ECS] Serializing..." << std::endl;
  json _finalData;
  std::vector<json> _entityData;
  _entityData.resize(m_EntityCount);

  for (auto &arr : m_ComponentArrays) {
    for (auto &component : arr.second->m_EntityToComponent) {
      json _temp;
      std::string _entityId = std::to_string(component.first);
      _temp[std::to_string(component.second->GetType())] =
          component.second->Serialize();
      _entityData[component.first][_entityId].push_back(_temp);
    }
  }

  for (i32 i = 0; i < m_EntityCount; i++) {
    _finalData.push_back(_entityData[i]);
  }

  std::fstream _stream(ENTITIES_DATA_PATH, std::ios::out);
  _stream << _finalData;
  _stream.close();

  DestroyEntity(0);
  DestroyEntity(1);
}

void Ecs::Deserialize() {
  std::cout << "[ECS] Deserializing..." << std::endl;
  std::fstream _stream(ENTITIES_DATA_PATH, std::ios::in);
  if (!_stream.is_open()) {
    std::cerr << "[ECS] Could not opem Entity Data File" << std::endl;
    return;
  }

  json _data;
  _stream >> _data;

  for (auto &entity : _data.items()) {
    for (auto &component : entity.value().items()) {
      std::cout << component.key() << " " << component.value() << std::endl;
    }
  }
}

void Ecs::InitSystems() {
  Signature _sig;
  _sig.set(ComponentType::TransformType);
  _sig.set(ComponentType::SpriteType);
  m_SpriteRenderer = std::make_unique<SpriteRenderer>(_sig, 0);
  _sig.reset();

  _sig.set(ComponentType::TransformType);
  _sig.set(ComponentType::SpriteSheetType);
  m_SpriteSheetRenderer = std::make_unique<SpriteSheetRenderer>(_sig, 1);
  _sig.reset();
}

u32 Ecs::GetNextEntityId() {
  u32 _id = 0;
  if (m_EntityIdStack.size() > 0) {
    _id = m_EntityIdStack.top();
    m_EntityIdStack.pop();
  } else {
    _id = m_EntityCount;
  }

  return _id;
}

void Ecs::Init() { InitSystems(); }
} // namespace tenshi
