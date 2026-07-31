#include <bone.h>
#include <imgui.h>
#include <properties_window.h>
#include <sik_math.h>
#include <sprite.h>

namespace SIK {

void PropertiesWindow::Draw(AppState& state) {
  bool& showProperties = state.GetView(Views::PROPERTIES);

  ImGui::Begin(PROPERTIES_WINDOW.c_str(), &showProperties);

  Mode& mode = state.GetMode();
  bool& ikSmoothing = state.GetIkSmoothing();
  int& selection_index = state.GetIndex();
  SelectionType& selection_type = state.GetSelectionType();
  auto& bones = state.GetBones();
  auto& sprites = state.GetSprites();

  if (mode == Mode::IK) {
    ImGui::Checkbox("IK Smoothing", &ikSmoothing);
    if (selection_type == BONE && selection_index >= 0 &&
        selection_index < (int)bones.size()) {
      auto& sb = bones[selection_index];
      int fullChain = Bone::AncestorChainLength(state, selection_index);
      int cur = sb->IKChainSize() > 0 ? sb->IKChainSize() : fullChain;
      ImGui::Text("IK chain length: %d / %d", cur, fullChain);
      if (ImGui::Button("-")) {
        cur = std::max(1, cur - 1);
        sb->IKChainSize() = (cur >= fullChain) ? -1 : cur;
      }
      ImGui::SameLine();
      if (ImGui::Button("+")) {
        cur = std::min(fullChain, cur + 1);
        sb->IKChainSize() = (cur >= fullChain) ? -1 : cur;
      }
      ImGui::SameLine();
      if (ImGui::Button("Auto")) {
        sb->IKChainSize() = -1;
      }
    }
  }

  ImGui::Separator();
  if (selection_type == BONE && selection_index >= 0 &&
      selection_index < (int)bones.size()) {
    auto& b = bones[selection_index];
    ImGui::InputText("Name", &b->Name());
    ImGui::Text("Parent: %s", b->ParentBoneID() < 0
                                  ? "(root)"
                                  : bones[b->ParentBoneID()]->Name().c_str());
    ImGui::Text("Length: %.0f", b->Length());
    if (ImGui::Button("Delete Bone")) {
      Bone::DeleteBone(state, selection_index);
    }
  } else if (selection_type == SPRITE && selection_index >= 0 &&
             selection_index < (int)sprites.size()) {
    auto& s = sprites[selection_index];
    ImGui::InputText("Name", &s->Name());
    ImGui::Text("Size: %dx%d", s->Texture().width, s->Texture().height);
    ImGui::Text("Attached: %s", s->AttachedBoneId() >= 0
                                    ? bones[s->AttachedBoneId()]->Name().c_str()
                                    : "(free)");

    float scale = s->Scale();
    if (ImGui::SliderFloat("Scale", &scale, 0.05f, 5.0f)) s->Scale() = scale;

    if (s->AttachedBoneId() >= 0) {
      if (ImGui::Button("Detach")) s->SetAttachedBoneId(-1);
    }

    ImGui::Text("Attach to:");
    for (size_t bi = 0; bi < bones.size(); bi++) {
      if (ImGui::Selectable(bones[bi]->Name().c_str(),
                            s->AttachedBoneId() == (int)bi)) {
        s->SetAttachedBoneId((int)bi);
        auto& b = bones[bi];
        s->LocalOffset() = Math::V2Rotate(
            Math::V2Sub(s->Position(), b->WorldStart()), -b->WorldRotation());
        s->LocalRotationOffset() =
            Math::NormalizeAngle(s->Rotation() - b->WorldRotation());
      }
    }
  } else {
    ImGui::Text("Nothing selected");
  }

  ImGui::End();  // End Properties
}

}  // namespace SIK