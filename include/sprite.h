#pragma once

#include <appstate.h>
#include <raylib.h>

#include <string>

namespace SIK {
class Sprite {
 public:
  using ID = int;
  static inline const ID FREEFORM = -1;

  Sprite() = default;
  ~Sprite() = default;

  Texture2D Texture() const;
  void SetTexture(Texture2D t);

  const std::string& Name() const;
  std::string& Name();

  const Vector2& Position() const;
  Vector2& Position();

  float Rotation() const;
  float& Rotation();

  float Scale() const;
  float& Scale();

  ID AttachedBoneId() const;
  void SetAttachedBoneId(ID id);

  const Vector2& LocalOffset() const;
  Vector2& LocalOffset();

  float LocalRotationOffset() const;
  float& LocalRotationOffset();

  static int FindNearest(AppState&, Vector2, float);
  static Vector2 ResizeHandlePosition(const Sprite&);
  static int FindNearestHandle(AppState& state, Vector2 worldPoint,
                               float maxDist);

 private:
  Texture2D tex{};
  std::string name;
  Vector2 pos = {0, 0};
  float rot = 0;
  float scale = 1.0f;
  ID attachedBone = FREEFORM;
  Vector2 localOffset = {0, 0};
  float localRotOffset = 0;
};
}  // namespace SIK