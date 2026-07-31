#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <raylib.h>
#include <rlImGui.h>
#include <types.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace SIK {
enum class Views { TOOLS, PROPERTIES, HIERARCHY, TEXTURES, CONSOLE };

class Bone;
class Sprite;
class AppState {
 public:
  AppState();
  ~AppState();

  std::vector<std::unique_ptr<Bone>>& GetBones();
  std::vector<std::unique_ptr<Sprite>>& GetSprites();

  bool& GetView(Views view);
  bool IsViewActive(Views view);

  Mode& GetMode();
  SelectionType& GetSelectionType();
  int& GetIndex();
  bool& GetIkSmoothing();
  bool IsIkSmoothing();

  Camera2D& GetCamera();

  bool& GetDrag();
  bool IsDragging();

  Vector2& GetGrabOffset();
  float& GetGrabAngle();
  float& GetRotation();
  float& GetSpriteResizeBaseDistance();
  float& GetSpriteResizeBaseScale();

  float& GetProjectedBoneResize();
  float& GetBaseBoneLength();

  bool& CreateDrag();
  Vector2& StartWorld();
  int& CreateParentId();

  int NextBoneId();
  int NextSpriteId();

  inline static const std::string APPNAME = "SIK";

 private:
  std::vector<std::unique_ptr<Bone>> bones;
  std::vector<std::unique_ptr<Sprite>> sprites;

  std::unordered_map<Views, bool> views;
  Mode currentMode = Mode::SELECT;
  ;
  SelectionType selType = NONE;
  int selIndex = -1;

  bool ikSmoothing = true;

  Camera2D camera = {0};

  bool dragging = false;
  Vector2 dragGrabOffset = {0, 0};
  float dragGrabAngleOffset = 0;
  float rotateStartValue = 0;
  float spriteResizeBaseDist = 1.0f;
  float spriteResizeBaseScale = 1.0f;
  float boneResizeBaseProjected = 1.0f;
  float boneResizeBaseLength = 60.0f;

  bool createDragging = false;
  Vector2 createStartWorld = {0, 0};
  int createParent = -1;

  int nextBoneId = 1;
  int nextSpriteId = 1;
};
}  // namespace SIK
