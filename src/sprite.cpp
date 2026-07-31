#include <sprite.h>

#include "sik_math.h"

namespace SIK {

Texture2D Sprite::Texture() const { return tex; }
void Sprite::SetTexture(Texture2D t) { tex = t; }

const std::string& Sprite::Name() const { return name; }
std::string& Sprite::Name() { return name; }

const Vector2& Sprite::Position() const { return pos; }
Vector2& Sprite::Position() { return pos; }

float Sprite::Rotation() const { return rot; }
float& Sprite::Rotation() { return rot; }

float Sprite::Scale() const { return scale; }
float& Sprite::Scale() { return scale; }

Sprite::ID Sprite::AttachedBoneId() const { return attachedBone; }
void Sprite::SetAttachedBoneId(ID id) { attachedBone = id; }

const Vector2& Sprite::LocalOffset() const { return localOffset; }
Vector2& Sprite::LocalOffset() { return localOffset; }

float Sprite::LocalRotationOffset() const { return localRotOffset; }
float& Sprite::LocalRotationOffset() { return localRotOffset; }

int Sprite::FindNearest(AppState& state, Vector2 worldPoint, float maxDist) {
  int best = -1;
  float bestDist = maxDist;
  auto& sprites = state.GetSprites();
  for (size_t i = 0; i < sprites.size(); i++) {
    auto& s = sprites[i];
    float w = s->Texture().width * s->Scale();
    float h = s->Texture().height * s->Scale();
    float r = 0.5f * sqrtf(w * w + h * h);
    float d = Math::V2Len(Math::V2Sub(worldPoint, s->Position()));
    if (d < r && d < bestDist) {
      bestDist = d;
      best = (int)i;
    }
  }
  return best;
}

Vector2 Sprite::ResizeHandlePosition(const Sprite& s) {
  Vector2 localCorner = {s.Texture().width * s.Scale() * 0.5f,
                         s.Texture().height * s.Scale() * 0.5f};
  return Math::V2Add(s.Position(), Math::V2Rotate(localCorner, s.Rotation()));
}

int Sprite::FindNearestHandle(AppState& state, Vector2 worldPoint,
                              float maxDist) {
  auto& sprites = state.GetSprites();
  int best = -1;
  float bestDist = maxDist;
  for (size_t i = 0; i < sprites.size(); i++) {
    auto& sprite = sprites[i];
    float d = Math::V2Len(
        Math::V2Sub(worldPoint, ResizeHandlePosition(*sprite.get())));
    if (d < bestDist) {
      bestDist = d;
      best = (int)i;
    }
  }
  return best;
}

}  // namespace SIK
