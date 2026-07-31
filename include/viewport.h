#pragma once
#include <appstate.h>

namespace SIK {
class Viewport {
 public:
  Viewport() {}
  ~Viewport() {}
  void Draw(AppState&);

 private:
  void DrawGrid(AppState&);
  void DrawSprites(AppState&);
  void DrawBones(AppState&);
  void DrawBoneShape(const Bone&, Color, Color);
};
}  // namespace SIK