#include <sik_math.h>
#include <viewport.h>

namespace SIK {
void Viewport::Draw(AppState& state) {
  DrawGrid(state);
  DrawSprites(state);
  DrawBones(state);
}

void Viewport::DrawContextMenu(AppState& state) {
  ImGuiIO& io = ImGui::GetIO();
  if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !io.WantCaptureMouse) {
    ImGui::OpenPopup("ViewportContext");
  }

  if (ImGui::BeginPopup("ViewportContext")) {
    if (ImGui::BeginMenu(ICON_FA_CIRCLE_PLUS " Add")) {
      if (ImGui::MenuItem(ICON_FA_BONE " Bone"))
        ;
      if (ImGui::MenuItem(ICON_FA_IMAGE " Texture"))
        ;
      ImGui::EndMenu();
    }
    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save"))
      ;

    if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save As"))
      ;

    if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save All"))
      ;

    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_XMARK " Close file"))
      ;

    ImGui::Separator();
    if (ImGui::BeginMenu(ICON_FA_TOOLBOX " Tools")) {
      if (ImGui::MenuItem(ICON_FA_ARROWS_ROTATE " Move"))
        ;
      if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Rotate"))
        ;
      if (ImGui::MenuItem(ICON_FA_EXPAND " Scale"))
        ;

      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_PERSON_RUNNING " IK"))
        ;
      ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_COMPACT_DISC " Bone Map"))
      ;

    if (ImGui::BeginMenu("Zoom")) {
      if (ImGui::MenuItem(ICON_FA_MAGNIFYING_GLASS_PLUS " Zoom In"))
        ;
      if (ImGui::MenuItem(ICON_FA_MAGNIFYING_GLASS_MINUS " Zoom Out"))
        ;
      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_MAGNIFYING_GLASS " Reset Zoom"))
        ;

      ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_COMPACT_DISC " Preferences"))
      ;

    ImGui::EndPopup();
  }
}

void Viewport::DrawGrid(AppState& state) {
  Camera2D& camera = state.GetCamera();
  Vector2 topLeft = GetScreenToWorld2D({0, 0}, camera);
  Vector2 bottomRight = GetScreenToWorld2D(
      {(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);
  float startX = floorf(topLeft.x / 50.0f) * 50.0f;
  float endX = ceilf(bottomRight.x / 50.0f) * 50.0f;
  float startY = floorf(topLeft.y / 50.0f) * 50.0f;
  float endY = ceilf(bottomRight.y / 50.0f) * 50.0f;

  for (float gx = startX; gx <= endX; gx += 50.0f) {
    Color c =
        (fabs(gx) < 1.0f) ? Color{90, 90, 100, 255} : Color{45, 45, 52, 255};
    DrawLine((int)gx, (int)startY, (int)gx, (int)endY, c);
  }
  for (float gy = startY; gy <= endY; gy += 50.0f) {
    Color c =
        (fabs(gy) < 1.0f) ? Color{90, 90, 100, 255} : Color{45, 45, 52, 255};
    DrawLine((int)startX, (int)gy, (int)endX, (int)gy, c);
  }
}

void Viewport::DrawSprites(AppState& state) {
  auto& sprites = state.GetSprites();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();

  for (size_t i = 0; i < sprites.size(); i++) {
    auto& s = sprites[i];
    Rectangle src = {0, 0, (float)s->Texture().width,
                     (float)s->Texture().height};
    Rectangle dst = {s->Position().x, s->Position().y,
                     s->Texture().width * s->Scale(),
                     s->Texture().height * s->Scale()};
    Vector2 origin = {dst.width / 2.0f, dst.height / 2.0f};
    DrawTexturePro(s->Texture(), src, dst, origin, s->Rotation() * RAD2DEG,
                   WHITE);

    if (selection_type == SPRITE && (int)i == selection_index) {
      float halfW = dst.width / 2.0f, halfH = dst.height / 2.0f;
      Vector2 c0 = Math::V2Add(s->Position(),
                               Math::V2Rotate({-halfW, -halfH}, s->Rotation()));
      Vector2 c1 = Math::V2Add(s->Position(),
                               Math::V2Rotate({halfW, -halfH}, s->Rotation()));
      Vector2 c2 = Math::V2Add(s->Position(),
                               Math::V2Rotate({halfW, halfH}, s->Rotation()));
      Vector2 c3 = Math::V2Add(s->Position(),
                               Math::V2Rotate({-halfW, halfH}, s->Rotation()));
      DrawLineEx(c0, c1, 2.0f, YELLOW);
      DrawLineEx(c1, c2, 2.0f, YELLOW);
      DrawLineEx(c2, c3, 2.0f, YELLOW);
      DrawLineEx(c3, c0, 2.0f, YELLOW);
    }
  }
}

void Viewport::DrawBoneShape(const Bone& b, Color fill, Color outline) {
  Vector2 start = b.WorldStart();
  Vector2 end = b.WorldEnd();
  Vector2 dir = Math::V2Norm(Math::V2Sub(end, start));
  Vector2 perp = {-dir.y, dir.x};
  float w = fminf(b.Length() * 0.16f, 10.0f);
  float widePoint = fminf(b.Length() * 0.18f, 18.0f);

  Vector2 p1 = start;
  Vector2 p2 = Math::V2Add(Math::V2Add(start, Math::V2Scale(dir, widePoint)),
                           Math::V2Scale(perp, w));
  Vector2 p3 = end;
  Vector2 p4 = Math::V2Sub(Math::V2Add(start, Math::V2Scale(dir, widePoint)),
                           Math::V2Scale(perp, w));

  DrawTriangle(p1, p2, p3, fill);
  DrawTriangle(p1, p3, p4, fill);
  DrawLineEx(p1, p2, 1.5f, outline);
  DrawLineEx(p2, p3, 1.5f, outline);
  DrawLineEx(p3, p4, 1.5f, outline);
  DrawLineEx(p4, p1, 1.5f, outline);

  DrawCircleV(start, fminf(w * 0.9f, 7.0f), outline);
  DrawCircleV(start, fminf(w * 0.9f, 7.0f) - 2.0f, fill);
  DrawCircleV(end, 3.0f, outline);
}

void Viewport::DrawBones(AppState& state) {
  auto& bones = state.GetBones();
  auto& sprites = state.GetSprites();
  SelectionType& selection_type = state.GetSelectionType();
  int& selection_index = state.GetIndex();
  Mode& currentMode = state.GetMode();
  Camera2D& camera = state.GetCamera();

  for (size_t i = 0; i < bones.size(); i++) {
    auto& b = bones[i];
    bool isSel = (selection_type == BONE && (int)i == selection_index);
    Color fill = b->ParentBoneID() < 0 ? Color{210, 140, 70, 255}
                                       : Color{100, 170, 220, 255};
    Color outline = Color{20, 20, 24, 255};
    if (isSel) {
      fill = Color{250, 210, 90, 255};
      outline = Color{255, 255, 255, 255};
    }
    DrawBoneShape(*b.get(), fill, outline);
  }

  if (currentMode == Mode::RESIZE) {
    for (size_t i = 0; i < bones.size(); i++) {
      bool isSelected = (selection_type == BONE && (int)i == selection_index);
      Vector2 tip = bones[i]->WorldEnd();
      float hs = 5.0f / camera.zoom;
      Color hc = isSelected ? YELLOW : Color{200, 200, 210, 255};
      DrawRectangleV({tip.x - hs, tip.y - hs}, {hs * 2, hs * 2}, hc);
      DrawRectangleLines((int)(tip.x - hs), (int)(tip.y - hs), (int)(hs * 2),
                         (int)(hs * 2), BLACK);
    }
    for (size_t i = 0; i < sprites.size(); i++) {
      auto& s = sprites[i];
      bool isSelelected =
          (selection_type == SPRITE && (int)i == selection_index);
      Texture2D texture = s->Texture();
      float scale = s->Scale();
      float halfW = texture.width * scale * 0.5f;
      float halfH = texture.height * scale * 0.5f;
      Color boxColor =
          isSelelected ? Color{250, 210, 90, 200} : Color{150, 150, 160, 150};
      Vector2 c0 = Math::V2Add(s->Position(),
                               Math::V2Rotate({-halfW, -halfH}, s->Rotation()));
      Vector2 c1 = Math::V2Add(s->Position(),
                               Math::V2Rotate({halfW, -halfH}, s->Rotation()));
      Vector2 c2 = Math::V2Add(s->Position(),
                               Math::V2Rotate({halfW, halfH}, s->Rotation()));
      Vector2 c3 = Math::V2Add(s->Position(),
                               Math::V2Rotate({-halfW, halfH}, s->Rotation()));
      DrawLineEx(c0, c1, 1.5f, boxColor);
      DrawLineEx(c1, c2, 1.5f, boxColor);
      DrawLineEx(c2, c3, 1.5f, boxColor);
      DrawLineEx(c3, c0, 1.5f, boxColor);

      Vector2 handle = Sprite::ResizeHandlePosition(*s.get());
      float hs = 6.0f / camera.zoom;
      Color hc = isSelelected ? YELLOW : Color{200, 200, 210, 255};
      DrawRectangleV({handle.x - hs, handle.y - hs}, {hs * 2, hs * 2}, hc);
      DrawRectangleLines((int)(handle.x - hs), (int)(handle.y - hs),
                         (int)(hs * 2), (int)(hs * 2), BLACK);
    }
  }
}
}  // namespace SIK