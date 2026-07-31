#pragma once

#include <appstate.h>
#include <bone.h>
#include <sik_math.h>
#include <sprite.h>

namespace SIK {
namespace ModeEvent {
inline void Select(AppState& state) {
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    float dBone;
    int bi = Bone::FindNearest(state, mouseWorld, 10.0f / camera.zoom, &dBone);
    int si = Sprite::FindNearest(state, mouseWorld, 1.0f);
    if (bi >= 0) {
      selection_type = BONE;
      selection_index = bi;
    } else if (si >= 0) {
      selection_type = SPRITE;
      selection_index = si;
    } else {
      selection_type = NONE;
      selection_index = -1;
    }
  }
}

inline void Move(AppState& state) {
  auto& sprites = state.GetSprites();
  auto& bones = state.GetBones();
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
  auto& dragging = state.GetDrag();
  auto& dragGrabOffset = state.GetGrabOffset();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    SelectionType hitType;
    int hitIndex;
    if (Math::HitTestBody(state, mouseWorld, 10.0f / camera.zoom, hitType,
                          hitIndex)) {
      selection_type = hitType;
      selection_index = hitIndex;
    }
    if (selection_type != NONE) {
      state.GetDrag() = true;
      if (selection_type == BONE) {
        dragGrabOffset =
            Math::V2Sub(bones[selection_index]->WorldStart(), mouseWorld);
      } else {
        dragGrabOffset =
            Math::V2Sub(sprites[selection_index]->Position(), mouseWorld);
      }
    }
  }
  if (dragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    Vector2 targetWorld = Math::V2Add(mouseWorld, dragGrabOffset);
    if (selection_type == BONE) {
      auto& b = bones[selection_index];
      b->LocalPosition() =
          Math::WorldToLocalOfParent(state, targetWorld, b->ParentBoneID());
    } else if (selection_type == SPRITE) {
      auto& s = sprites[selection_index];
      if (s->AttachedBoneId() >= 0) {
        auto& b = bones[s->AttachedBoneId()];
        s->LocalOffset() = Math::V2Rotate(
            Math::V2Sub(targetWorld, b->WorldStart()), -b->WorldRotation());
      } else {
        s->Position() = targetWorld;
      }
    }
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragging = false;
}

inline void Rotate(AppState& state) {
  auto& sprites = state.GetSprites();
  auto& bones = state.GetBones();
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
  auto& dragging = state.GetDrag();
  auto& dragGrabAngleOffset = state.GetGrabAngle();
  auto& rotateStartValue = state.GetRotation();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    SelectionType hitType;
    int hitIndex;
    if (Math::HitTestBody(state, mouseWorld, 10.0f / camera.zoom, hitType,
                          hitIndex)) {
      selection_type = hitType;
      selection_index = hitIndex;
    }
    if (selection_type != NONE) {
      dragging = true;
      Vector2 origin = (selection_type == BONE)
                           ? bones[selection_index]->WorldStart()
                           : sprites[selection_index]->Position();
      dragGrabAngleOffset =
          atan2f(mouseWorld.y - origin.y, mouseWorld.x - origin.x);
      if (selection_type == BONE) {
        rotateStartValue = bones[selection_index]->LocalRotation();
      } else {
        auto& s = sprites[selection_index];
        rotateStartValue = (s->AttachedBoneId() >= 0) ? s->LocalRotationOffset()
                                                      : s->Rotation();
      }
    }
  }
  if (dragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    Vector2 origin = (selection_type == BONE)
                         ? bones[selection_index]->WorldStart()
                         : sprites[selection_index]->Position();
    float mouseAngleNow =
        atan2f(mouseWorld.y - origin.y, mouseWorld.x - origin.x);
    float delta = Math::NormalizeAngle(mouseAngleNow - dragGrabAngleOffset);
    if (selection_type == BONE) {
      bones[selection_index]->LocalRotation() =
          Math::NormalizeAngle(rotateStartValue + delta);
    } else if (selection_type == SPRITE) {
      auto& s = sprites[selection_index];
      if (s->AttachedBoneId() >= 0)
        s->LocalRotationOffset() =
            Math::NormalizeAngle(rotateStartValue + delta);
      else
        s->Rotation() = Math::NormalizeAngle(rotateStartValue + delta);
    }
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragging = false;
}

inline void Resize(AppState& state) {
  auto& sprites = state.GetSprites();
  auto& bones = state.GetBones();
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  auto& dragging = state.GetDrag();
  auto& boneResizeBaseProjected = state.GetProjectedBoneResize();
  auto& boneResizeBaseLength = state.GetBaseBoneLength();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
  auto& spriteResizeBaseDist = state.GetSpriteResizeBaseDistance();
  auto& spriteResizeBaseScale = state.GetSpriteResizeBaseScale();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    int bi = Bone::FindNearestTip(state, mouseWorld, 14.0f / camera.zoom);
    int si = (bi < 0) ? Sprite::FindNearestHandle(state, mouseWorld,
                                                  16.0f / camera.zoom)
                      : -1;
    if (bi >= 0) {
      selection_type = BONE;
      selection_index = bi;
    } else if (si >= 0) {
      selection_type = SPRITE;
      selection_index = si;
    }

    if (selection_type != NONE) {
      dragging = true;
      if (selection_type == BONE) {
        auto& b = bones[selection_index];
        Vector2 dir = {cosf(b->WorldRotation()), sinf(b->WorldRotation())};
        Vector2 toMouse = Math::V2Sub(mouseWorld, b->WorldStart());
        float projected = toMouse.x * dir.x + toMouse.y * dir.y;
        boneResizeBaseProjected = fabsf(projected) < 4.0f
                                      ? (projected < 0 ? -4.0f : 4.0f)
                                      : projected;
        boneResizeBaseLength = b->Length();
      } else {
        auto& s = sprites[selection_index];
        spriteResizeBaseDist =
            std::max(4.0f, Math::V2Len(Math::V2Sub(mouseWorld, s->Position())));
        spriteResizeBaseScale = s->Scale();
      }
    }
  }
  if (dragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    if (selection_type == BONE) {
      auto& b = bones[selection_index];
      Vector2 dir = {cosf(b->WorldRotation()), sinf(b->WorldRotation())};
      Vector2 toMouse = Math::V2Sub(mouseWorld, b->WorldStart());
      float projected = toMouse.x * dir.x + toMouse.y * dir.y;
      float ratio = projected / boneResizeBaseProjected;
      b->Length() = std::max(8.0f, boneResizeBaseLength * ratio);
    } else if (selection_type == SPRITE) {
      auto& s = sprites[selection_index];
      float d =
          std::max(4.0f, Math::V2Len(Math::V2Sub(mouseWorld, s->Position())));
      s->Scale() =
          std::max(0.05f, spriteResizeBaseScale * (d / spriteResizeBaseDist));
    }
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragging = false;
}

inline void Create(AppState& state) {
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    float dBone;
    int bi = Bone::FindNearest(state, mouseWorld, 14.0f / camera.zoom, &dBone);
    state.CreateParentId() = bi;
    state.StartWorld() = mouseWorld;
    state.CreateDrag() = true;
  }
  if (state.CreateDrag() && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    Vector2 endWorld = mouseWorld;
    if (Math::V2Len(Math::V2Sub(endWorld, state.StartWorld())) < 6.0f) {
      endWorld = Math::V2Add(state.StartWorld(), {60, 0});
    }
    int idx = Bone::CreateBone(state, state.StartWorld(), endWorld,
                               state.CreateParentId());
    selection_type = BONE;
    selection_index = idx;
    state.CreateDrag() = false;
  }
}

inline void Delete(AppState& state) {
  auto& sprites = state.GetSprites();
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    float dBone;
    int bi = Bone::FindNearest(state, mouseWorld, 10.0f / camera.zoom, &dBone);
    if (bi >= 0) {
      Bone::DeleteBone(state, bi);
    } else {
      int si = Sprite::FindNearest(state, mouseWorld, 1.0f);
      if (si >= 0) {
        UnloadTexture(sprites[si]->Texture());
        sprites.erase(sprites.begin() + si);
        if (selection_type == SPRITE) {
          if (selection_index == si) {
            selection_type = NONE;
            selection_index = -1;
          } else if (selection_index > si)
            selection_index--;
        }
      }
    }
  }
}

inline void Ik(AppState& state) {
  Vector2 mouseScreen = GetMousePosition();
  Camera2D& camera = state.GetCamera();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
  auto& dragging = state.GetDrag();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    float dBone;
    int bi = Bone::FindNearest(state, mouseWorld, 12.0f / camera.zoom, &dBone);
    if (bi >= 0) {
      selection_type = BONE;
      selection_index = bi;
      dragging = true;
    }
  }
  if (dragging && selection_type == BONE &&
      IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    Math::SolveIK(state, selection_index, mouseWorld);
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragging = false;
}
}  // namespace ModeEvent
}  // namespace SIK