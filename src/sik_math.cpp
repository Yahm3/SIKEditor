#include <sik_math.h>

namespace SIK {
namespace Math {
Vector2 V2Sub(Vector2 a, Vector2 b) { return {a.x - b.x, a.y - b.y}; }

Vector2 V2Add(Vector2 a, Vector2 b) { return {a.x + b.x, a.y + b.y}; }

Vector2 V2Scale(Vector2 a, float s) { return {a.x * s, a.y * s}; }

float V2Len(Vector2 a) { return sqrtf(a.x * a.x + a.y * a.y); }

Vector2 V2Norm(Vector2 a) {
  float l = V2Len(a);
  if (l < 0.0001f) return {1, 0};
  return {a.x / l, a.y / l};
}

Vector2 V2Rotate(Vector2 v, float rad) {
  float c = cosf(rad), s = sinf(rad);
  return {v.x * c - v.y * s, v.x * s + v.y * c};
}

float NormalizeAngle(float a) {
  while (a > PI) a -= 2 * PI;
  while (a < -PI) a += 2 * PI;
  return a;
}

float PointSegmentDist(Vector2 p, Vector2 a, Vector2 b, float* outT) {
  Vector2 ab = V2Sub(b, a);
  float lenSq = ab.x * ab.x + ab.y * ab.y;
  float t = lenSq > 0.0001f ? ((p.x - a.x) * ab.x + (p.y - a.y) * ab.y) / lenSq
                            : 0.0f;
  t = std::max(0.0f, std::min(1.0f, t));
  if (outT) *outT = t;
  Vector2 proj = V2Add(a, V2Scale(ab, t));
  return V2Len(V2Sub(p, proj));
}

Vector2 WorldToLocalOfParent(AppState& state, Vector2 worldPoint,
                             int parentIdx) {
  auto& bones = state.GetBones();

  if (parentIdx < 0) return worldPoint;
  auto& p = bones[parentIdx];
  Vector2 pTip = p->WorldEnd();
  Vector2 rel = Math::V2Sub(worldPoint, pTip);
  return Math::V2Rotate(rel, -p->WorldRotation());
}

bool HitTestBody(AppState& state, Vector2 worldPoint, float boneRadius,
                 SelectionType& outType, int& outIndex) {
  float dBone;

  int bi = Bone::FindNearest(state, worldPoint, boneRadius, &dBone);
  int si = Sprite::FindNearest(state, worldPoint, 1.0f);
  if (bi >= 0) {
    outType = BONE;
    outIndex = bi;
    return true;
  }
  if (si >= 0) {
    outType = SPRITE;
    outIndex = si;
    return true;
  }
  return false;
}

void UpdateSkeleton(AppState& state) {
  auto& bones = state.GetBones();
  auto& sprites = state.GetSprites();

  for (size_t i = 0; i < bones.size(); i++) {
    auto& b = bones[i];
    if (b->ParentBoneID() < 0) {
      b->WorldStart() = b->LocalPosition();
      b->WorldRotation() = b->LocalRotation();
    } else {
      auto& p = bones[b->ParentBoneID()];
      Vector2 pTip = p->WorldEnd();
      Vector2 rotatedOffset =
          Math::V2Rotate(b->LocalPosition(), p->WorldRotation());
      b->WorldStart() = Math::V2Add(pTip, rotatedOffset);
      b->WorldRotation() = p->WorldRotation() + b->LocalRotation();
    }
  }
  for (auto& s : sprites) {
    if (s->AttachedBoneId() >= 0 && s->AttachedBoneId() < (int)bones.size()) {
      auto& b = bones[s->AttachedBoneId()];
      s->Position() =
          Math::V2Add(b->WorldStart(),
                      Math::V2Rotate(s->LocalOffset(), b->WorldRotation()));
      s->Rotation() = b->WorldRotation() + s->LocalRotationOffset();
    }
  }
}

void SolveIK(AppState& state, int effectorIdx, Vector2 target, int iterations) {
  if (effectorIdx < 0) return;

  const float IK_DAMPING = 0.55f;
  const float IK_MAX_STEP = 35.0f * DEG2RAD;
  const float IK_SMOOTH_AMT = 0.35f;
  auto& bones = state.GetBones();

  int fullChain = Bone::AncestorChainLength(state, effectorIdx);
  int limit = bones[effectorIdx]->IKChainSize() > 0
                  ? std::min(bones[effectorIdx]->IKChainSize(), fullChain)
                  : fullChain;

  std::vector<int> chain;
  int cur = effectorIdx;
  while (cur != -1 && (int)chain.size() < limit) {
    chain.push_back(cur);
    cur = bones[cur]->ParentBoneID();
  }

  std::vector<float> preLocalRot(chain.size());
  for (size_t i = 0; i < chain.size(); i++)
    preLocalRot[i] = bones[chain[i]]->LocalRotation();

  for (int iter = 0; iter < iterations; iter++) {
    for (int bidx : chain) {
      UpdateSkeleton(state);
      auto& b = bones[bidx];
      Vector2 effectorTip = bones[effectorIdx]->WorldEnd();
      Vector2 pivot = b->WorldStart();
      Vector2 toEffector = Math::V2Norm(Math::V2Sub(effectorTip, pivot));
      Vector2 toTarget = Math::V2Norm(Math::V2Sub(target, pivot));
      float angEff = atan2f(toEffector.y, toEffector.x);
      float angTgt = atan2f(toTarget.y, toTarget.x);
      float delta = Math::NormalizeAngle(angTgt - angEff);
      delta = std::max(-IK_MAX_STEP, std::min(IK_MAX_STEP, delta));
      b->LocalRotation() =
          Math::NormalizeAngle(b->LocalRotation() + delta * IK_DAMPING);
    }
  }

  if (state.IsIkSmoothing()) {
    for (size_t i = 0; i < chain.size(); i++) {
      auto& b = bones[chain[i]];
      float delta = Math::NormalizeAngle(b->LocalRotation() - preLocalRot[i]);
      b->LocalRotation() =
          Math::NormalizeAngle(preLocalRot[i] + delta * IK_SMOOTH_AMT);
    }
  }

  UpdateSkeleton(state);
}

}  // namespace Math
}  // namespace SIK