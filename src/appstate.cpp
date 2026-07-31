#include <appstate.h>

#include "bone.h"
#include "sprite.h"

namespace SIK {

AppState::AppState() {
  views[Views::TOOLS] = true;
  views[Views::PROPERTIES] = true;
  views[Views::HIERARCHY] = true;
  views[Views::TEXTURES] = true;
  views[Views::CONSOLE] = true;
}

AppState::~AppState() = default;

std::vector<std::unique_ptr<Bone>>& AppState::GetBones() { return bones; }
std::vector<std::unique_ptr<Sprite>>& AppState::GetSprites() { return sprites; }

bool& AppState::GetView(Views view) { return views[view]; }
bool AppState::IsViewActive(Views view) { return views[view] == true; }

Mode& AppState::GetMode() { return currentMode; }
SelectionType& AppState::GetSelectionType() { return selType; }
int& AppState::GetIndex() { return selIndex; }
bool& AppState::GetIkSmoothing() { return ikSmoothing; }
bool AppState::IsIkSmoothing() { return ikSmoothing == true; }

Camera2D& AppState::GetCamera() { return camera; }

bool& AppState::GetDrag() { return dragging; }
bool AppState::IsDragging() { return dragging == true; }

Vector2& AppState::GetGrabOffset() { return dragGrabOffset; }
float& AppState::GetGrabAngle() { return dragGrabAngleOffset; }
float& AppState::GetRotation() { return rotateStartValue; }
float& AppState::GetSpriteResizeBaseDistance() { return spriteResizeBaseDist; }
float& AppState::GetSpriteResizeBaseScale() { return spriteResizeBaseScale; }

float& AppState::GetProjectedBoneResize() { return boneResizeBaseProjected; }
float& AppState::GetBaseBoneLength() { return boneResizeBaseLength; }

bool& AppState::CreateDrag() { return createDragging; }
Vector2& AppState::StartWorld() { return createStartWorld; }
int& AppState::CreateParentId() { return createParent; }

int AppState::NextBoneId() { return nextBoneId++; }
int AppState::NextSpriteId() { return nextSpriteId++; }

}  // namespace SIK