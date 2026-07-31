#include <bone.h>
#include <imgui.h>
#include <rlImGui.h>
#include <sprite.h>
#include <textures_window.h>

namespace SIK {

void TexturesWindow::Draw(AppState& state) {
  if (!state.IsViewActive(Views::TEXTURES)) return;

  bool& show_view = state.GetView(Views::TEXTURES);
  auto& sprites = state.GetSprites();
  auto& selection_type = state.GetSelectionType();
  auto& selection_index = state.GetIndex();

  ImGui::Begin(TEXTURES_WINDOW.c_str(), &show_view);
  for (size_t i = 0; i < sprites.size(); ++i) {
    auto& s = sprites[i];
    ImGui::PushID((int)i);
    if (ImGui::Selectable(
            s->Name().c_str(),
            selection_type == SPRITE && selection_index == (int)i, 0,
            ImVec2(0, 50))) {
      selection_type = SPRITE;
      selection_index = (int)i;
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 60);
    Texture2D t = s->Texture();
    rlImGuiImageSize(&t, 40, 40);
    ImGui::PopID();
  }
  ImGui::End();
}

}  // namespace SIK