#include <bone.h>
#include <hierarchy_window.h>
#include <imgui.h>
#include <sprite.h>

namespace SIK {

void HierarchyWindow::Draw(AppState& state) {
  if (!state.IsViewActive(Views::HIERARCHY)) return;

  bool& show_view = state.GetView(Views::HIERARCHY);
  auto& bones = state.GetBones();

  ImGui::Begin(HIERARCHY_WINDOW.c_str(), &show_view);
  for (size_t i = 0; i < bones.size(); ++i) {
    if (bones[i]->ParentBoneID() < 0) {  // Root bones
      DrawBoneNode(state, (int)i);
    }
  }
  ImGui::End();
}

void HierarchyWindow::DrawBoneNode(AppState& state, int boneIndex) {
  std::vector<std::unique_ptr<Bone>>& bones = state.GetBones();

  auto& b = bones[boneIndex];
  ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow |
                                 ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                 ImGuiTreeNodeFlags_SpanAvailWidth;

  auto& selection_type = state.GetSelectionType();
  auto& selection_index = state.GetIndex();
  if (selection_type == BONE && selection_index == boneIndex) {
    nodeFlags |= ImGuiTreeNodeFlags_Selected;
  }

  bool hasChildren = false;
  for (size_t i = 0; i < bones.size(); ++i) {
    if (bones[i]->ParentBoneID() == boneIndex) {
      hasChildren = true;
      break;
    }
  }

  if (!hasChildren) {
    nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
  }

  const char* icon = ICON_FA_BONE;
  bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)boneIndex, nodeFlags,
                                    "%s %s", icon, b->Name().c_str());

  if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
    selection_type = BONE;
    selection_index = boneIndex;
  }

  if (nodeOpen && hasChildren) {
    for (size_t i = 0; i < bones.size(); ++i) {
      if (bones[i]->ParentBoneID() == boneIndex) {
        DrawBoneNode(state, (int)i);
      }
    }
    ImGui::TreePop();
  }
}

}  // namespace SIK