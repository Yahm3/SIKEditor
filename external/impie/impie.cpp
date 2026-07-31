
#include "impie.h"

void PieMenu::Open(State& state, const ImVec2& center) {
  state.Open = true;
  state.Center = center;
}

void PieMenu::Close(State& state) { state.Open = false; }

int PieMenu::Draw(State& state, const char* const items[], int itemCount,
                  int* selected) {
  if (!state.Open) return -1;

  ImDrawList* draw = ImGui::GetForegroundDrawList();

  ImGuiIO& io = ImGui::GetIO();
  const ImGuiStyle& style = ImGui::GetStyle();

  constexpr float INNER_RADIUS = 40.0f;
  constexpr float OUTER_RADIUS = 120.0f;
  constexpr float DEAD_ZONE = 20.0f;
  constexpr int MIN_ITEMS = 6;

  ImVec2 mouse = io.MousePos;
  ImVec2 delta(mouse.x - state.Center.x, mouse.y - state.Center.y);

  float dist2 = delta.x * delta.x + delta.y * delta.y;

  float angle = atan2f(delta.y, delta.x);

  if (angle < 0.0f) angle += IM_PI * 2.0f;

  float arcSpan = (2.0f * IM_PI) / ImMax(itemCount, MIN_ITEMS);

  // Ring
  draw->AddCircle(state.Center, (INNER_RADIUS + OUTER_RADIUS) * 0.5f,
                  IM_COL32(40, 40, 40, 255), 64, OUTER_RADIUS - INNER_RADIUS);

  int hovered = -1;

  for (int i = 0; i < itemCount; i++) {
    float a0 = arcSpan * i + 0.02f * arcSpan;
    float a1 = arcSpan * (i + 1) - 0.02f * arcSpan;

    bool isHovered = false;

    if (dist2 > DEAD_ZONE * DEAD_ZONE) isHovered = angle >= a0 && angle < a1;

    if (isHovered) hovered = i;

    bool isSelected = selected && *selected == i;

    int segments = 24;

    draw->PathArcTo(state.Center, OUTER_RADIUS - style.ItemInnerSpacing.x, a0,
                    a1, segments);

    draw->PathArcTo(state.Center, INNER_RADIUS + style.ItemInnerSpacing.x, a1,
                    a0, segments);

    ImU32 color = isHovered    ? IM_COL32(90, 140, 255, 255)
                  : isSelected ? IM_COL32(120, 120, 160, 255)
                               : IM_COL32(70, 70, 70, 230);

    draw->PathFillConvex(color);

    float mid = (a0 + a1) * 0.5f;
    float r = (INNER_RADIUS + OUTER_RADIUS) * 0.5f;

    ImVec2 pos(state.Center.x + cosf(mid) * r, state.Center.y + sinf(mid) * r);

    ImVec2 size = ImGui::CalcTextSize(items[i]);

    pos.x -= size.x * 0.5f;
    pos.y -= size.y * 0.5f;

    draw->AddText(pos, IM_COL32_WHITE, items[i]);
  }

  // Centre circles
  draw->AddCircle(state.Center, INNER_RADIUS, IM_COL32(255, 255, 255, 120), 64,
                  2.0f);

  draw->AddCircle(state.Center, OUTER_RADIUS, IM_COL32(255, 255, 255, 120), 64,
                  2.0f);

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
    state.Open = false;

    if (hovered != -1 && selected) *selected = hovered;

    return hovered;
  }

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
    state.Open = false;
    return -1;
  }

  return -1;
}
