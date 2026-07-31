#pragma once
#include <imgui.h>
#include <imgui_internal.h>

#include <cmath>

namespace PieMenu {
struct State {
  bool Open = false;
  ImVec2 Center{};
};

void Open(State& state, const ImVec2& center);
void Close(State& state);
int Draw(State& state, const char* const items[], int itemCount,
         int* selected = nullptr);
}  // namespace PieMenu
