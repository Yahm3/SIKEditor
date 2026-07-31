#pragma once

#include <appstate.h>
#include <raylib.h>
#include <types.h>

#include <string>

namespace SIK {

class Bone {
 public:
  using ID = int;
  static inline const ID ROOT = -1;

  Bone() = default;
  ~Bone() = default;

  const std::string& Name() const;
  std::string& Name();

  ID ParentBoneID() const;
  void SetParentBoneID(ID id);

  const Vector2& LocalPosition() const;
  Vector2& LocalPosition();

  float LocalRotation() const;
  float& LocalRotation();

  float Length() const;
  float& Length();

  int IKChainSize() const;
  int& IKChainSize();

  const Vector2& WorldStart() const;
  Vector2& WorldStart();

  float WorldRotation() const;
  float& WorldRotation();

  Vector2 WorldEnd() const;

  static int CreateBone(AppState&, Vector2, Vector2, int);
  static void DeleteBone(AppState& state, int idx);
  static int FindNearest(AppState&, Vector2, float, float*);
  static int FindNearestTip(AppState&, Vector2, float);
  static int AncestorChainLength(AppState&, int);

 private:
  std::string m_name;
  ID m_parent{ROOT};
  Vector2 m_localPos{0, 0};
  float m_localRot = 0;
  float m_length = 60.0f;
  int m_ikChainOverride = -1;
  Vector2 m_worldStart = {0, 0};
  float m_worldRot = 0;
};
}  // namespace SIK