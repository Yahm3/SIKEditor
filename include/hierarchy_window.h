#pragma once
#include <appstate.h>

namespace SIK {
class HierarchyWindow {
 public:
  HierarchyWindow() {}
  ~HierarchyWindow() {}
  void Draw(AppState&);

 private:
  void DrawBoneNode(AppState&, int);
};
}  // namespace SIK