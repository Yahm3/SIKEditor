#include <bone.h>
#include <sik_math.h>
#include <sprite.h>

#include <cmath>

namespace SIK {

Vector2 Bone::WorldEnd() const {
  return {m_worldStart.x + cosf(m_worldRot) * m_length,
          m_worldStart.y + sinf(m_worldRot) * m_length};
}

const std::string& Bone::Name() const { return m_name; }
std::string& Bone::Name() { return m_name; }

Bone::ID Bone::ParentBoneID() const { return m_parent; }
void Bone::SetParentBoneID(ID id) { m_parent = id; }

const Vector2& Bone::LocalPosition() const { return m_localPos; }
Vector2& Bone::LocalPosition() { return m_localPos; }

float Bone::LocalRotation() const { return m_localRot; }
float& Bone::LocalRotation() { return m_localRot; }

float Bone::Length() const { return m_length; }
float& Bone::Length() { return m_length; }

int Bone::IKChainSize() const { return m_ikChainOverride; }
int& Bone::IKChainSize() { return m_ikChainOverride; }

const Vector2& Bone::WorldStart() const { return m_worldStart; }
Vector2& Bone::WorldStart() { return m_worldStart; }

float Bone::WorldRotation() const { return m_worldRot; }
float& Bone::WorldRotation() { return m_worldRot; }

int Bone::CreateBone(AppState& state, Vector2 startWorld, Vector2 endWorld,
                     int parentIdx) {
  auto& bones = state.GetBones();

  auto b = std::make_unique<Bone>();
  b->Name() = "Bone_" + std::to_string(state.NextBoneId());
  b->SetParentBoneID(parentIdx);
  b->Length() = std::max(8.0f, Math::V2Len(Math::V2Sub(endWorld, startWorld)));
  float worldRot = atan2f(endWorld.y - startWorld.y, endWorld.x - startWorld.x);

  if (parentIdx < 0) {
    b->LocalPosition() = startWorld;
    b->LocalRotation() = worldRot;
  } else {
    auto& p = bones[parentIdx];
    b->LocalPosition() =
        Math::WorldToLocalOfParent(state, startWorld, parentIdx);
    b->LocalRotation() = Math::NormalizeAngle(worldRot - p->WorldRotation());
  }
  bones.push_back(std::move(b));
  return (int)bones.size() - 1;
}

void Bone::DeleteBone(AppState& state, int idx) {
  std::vector<std::unique_ptr<Bone>>& bones = state.GetBones();
  std::vector<std::unique_ptr<Sprite>>& sprites = state.GetSprites();

  if (idx < 0 || idx >= (int)bones.size()) return;
  int newParent = bones[idx]->ParentBoneID();

  for (size_t i = 0; i < bones.size(); i++) {
    if ((int)i == idx) continue;
    if (bones[i]->ParentBoneID() == idx) {
      Vector2 worldPos = bones[i]->WorldStart();
      float worldRot = bones[i]->WorldRotation();
      bones[i]->SetParentBoneID(newParent);
      bones[i]->LocalPosition() =
          Math::WorldToLocalOfParent(state, worldPos, newParent);
      bones[i]->LocalRotation() = Math::NormalizeAngle(
          newParent < 0 ? worldRot
                        : worldRot - bones[newParent]->WorldRotation());
    }
  }
  for (auto& s : sprites)
    if (s->AttachedBoneId() == idx) s->SetAttachedBoneId(-1);

  bones.erase(bones.begin() + idx);
  for (auto& b : bones)
    if (b->ParentBoneID() > idx) b->SetParentBoneID(b->ParentBoneID() - 1);
  for (auto& s : sprites)
    if (s->AttachedBoneId() > idx)
      s->SetAttachedBoneId(s->AttachedBoneId() - 1);

  auto& selection_type = state.GetSelectionType();
  auto& selection_index = state.GetIndex();
  if (selection_type == BONE) {
    if (selection_index == idx) {
      selection_type = NONE;
      selection_index = -1;
    } else if (selection_index > idx)
      selection_index--;
  }
}

int Bone::FindNearest(AppState& state, Vector2 worldPoint, float maxDist,
                      float* outDist) {
  int best = -1;
  float bestDist = maxDist;
  std::vector<std::unique_ptr<Bone>>& bones = state.GetBones();
  for (size_t i = 0; i < bones.size(); i++) {
    float d = Math::PointSegmentDist(worldPoint, bones[i]->WorldStart(),
                                     bones[i]->WorldEnd());
    if (d < bestDist) {
      bestDist = d;
      best = (int)i;
    }
  }
  if (outDist) *outDist = bestDist;
  return best;
}

int Bone::FindNearestTip(AppState& state, Vector2 worldPoint, float maxDist) {
  int best = -1;
  float bestDist = maxDist;
  std::vector<std::unique_ptr<Bone>>& bones = state.GetBones();
  for (size_t i = 0; i < bones.size(); i++) {
    float d = Math::V2Len(Math::V2Sub(worldPoint, bones[i]->WorldEnd()));
    if (d < bestDist) {
      bestDist = d;
      best = (int)i;
    }
  }
  return best;
}

int Bone::AncestorChainLength(AppState& state, int boneIdx) {
  std::vector<std::unique_ptr<Bone>>& bones = state.GetBones();
  int n = 1;
  for (int c = bones[boneIdx]->ParentBoneID(); c != -1;
       c = bones[c]->ParentBoneID())
    n++;
  return n;
}

}  // namespace SIK