#include <imgui.h>
#include <tools_window.h>

namespace SIK {

void ToolsWindow::Draw(AppState& state) {
  ImGuiWindowFlags toolsFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
  ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 30.0f),
                          ImGuiCond_Always, ImVec2(0.5f, 0.0f));
  ImGui::SetNextWindowBgAlpha(0.8f);

  ImGui::Begin(TOOLS_WINDOW.c_str(), nullptr, toolsFlags);

  Mode& mode = state.GetMode();
  for (int m = 0; m < static_cast<int>(Mode::COUNT); m++) {
    if (m > 0) ImGui::SameLine();
    char id[32];
    snprintf(id, sizeof(id), "%s##mode%d", ModeIcon((Mode)m), m);

    if (ImGui::RadioButton(id, static_cast<int>(mode) == m)) {
      mode = (Mode)m;
      state.GetDrag() = false;
      state.CreateDrag() = false;
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
      ImGui::SetTooltip("%s", ModeName((Mode)m));
  }
  ImGui::End();
}

}  // namespace SIK