#include <bone.h>
#include <global_menu.h>
#include <sprite.h>

#include <iostream>

namespace SIK {
GlobalMenu::GlobalMenu() {}
GlobalMenu::~GlobalMenu() {}

void GlobalMenu::Draw(AppState& state) {
  if (ImGui::BeginMainMenuBar()) {
    FileMenu(state);
    EditMenu(state);
    ToolsMenu(state);
    WindowMenu(state);
    HelpMenu(state);
    ImGui::EndMainMenuBar();
  }
}

void GlobalMenu::FileMenu(AppState& state) {
  if (ImGui::BeginMenu("File")) {
    if (ImGui::BeginMenu(ICON_FA_FILE " New")) {
      if (ImGui::MenuItem(ICON_FA_CUBE " Project"))
        ;

      if (ImGui::MenuItem(ICON_FA_FILE " Skeleton"))
        ;
      ImGui::EndMenu();
    }

    if (ImGui::MenuItem(ICON_FA_FOLDER_PLUS " Open Project"))
      ;

    if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open Skeleton"))
      ;

    ImGui::Separator();
    if (ImGui::BeginMenu(ICON_FA_ARROW_ROTATE_LEFT " Recent Files")) {
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
    if (ImGui::BeginMenu(ICON_FA_FILE_EXPORT " Export")) {
      if (ImGui::MenuItem("JSON"))
        ;

      if (ImGui::MenuItem("Blob"))
        ;

      if (ImGui::MenuItem(ICON_FA_CODE " Header file"))
        ;

      ImGui::EndMenu();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_RIGHT_FROM_BRACKET " Quit"))
      ;

    ImGui::EndMenu();
  }
}

void GlobalMenu::WindowMenu(AppState& state) {
  if (ImGui::BeginMenu("Window")) {
    ImGui::MenuItem(ICON_FA_TOOLBOX " Tools", nullptr,
                    state.GetView(Views::TOOLS));
    ImGui::MenuItem(ICON_FA_GEAR " Properties", nullptr,
                    state.GetView(Views::PROPERTIES));
    ImGui::MenuItem(ICON_FA_FOLDER_TREE " Hierarchy", nullptr,
                    state.GetView(Views::HIERARCHY));
    ImGui::MenuItem(ICON_FA_IMAGE " Textures", nullptr,
                    state.GetView(Views::TEXTURES));
    ImGui::MenuItem(ICON_FA_TERMINAL " Console", nullptr,
                    state.GetView(Views::CONSOLE));
    ImGui::EndMenu();
  }
}

void GlobalMenu::EditMenu(AppState& state) {
  if (ImGui::BeginMenu("Edit")) {
    if (ImGui::MenuItem(ICON_FA_ARROW_ROTATE_LEFT " Undo"))
      ;

    if (ImGui::MenuItem(ICON_FA_ARROW_ROTATE_RIGHT " Redo"))
      ;

    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_SCISSORS " Cut"))
      ;
    if (ImGui::MenuItem(ICON_FA_COPY " Copy"))
      ;
    if (ImGui::MenuItem(ICON_FA_PASTE " Paste"))
      ;
    ImGui::Separator();

    if (ImGui::BeginMenu("Delete")) {
      if (ImGui::MenuItem(ICON_FA_BONE " Selected Bone"))
        ;
      if (ImGui::MenuItem(ICON_FA_IMAGE " Selected Texture"))
        ;

      if (ImGui::BeginMenu("Bones")) {
        std::vector<std::unique_ptr<Bone>>& bones = state.GetBones();

        int delete_index = -1;
        for (size_t i = 0; i < bones.size(); ++i) {
          std::unique_ptr<Bone>& bone = bones[i];
          std::string id = "xxbone#menu" + std::to_string(i) + bone->Name();
          ImGui::PushID(id.c_str());

          std::string menu_text = std::string(ICON_FA_BONE) + bone->Name();
          if (ImGui::MenuItem(menu_text.c_str())) {
            delete_index = i;
          }
          ImGui::PopID();
        }

        if (delete_index > -1) {
          std::cout << "Delete: " << delete_index << std::endl;
          Bone::DeleteBone(state, delete_index);
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Delete all bones")) {
          while (bones.size() > 0) {
            int last_index = bones.size() - 1;
            Bone::DeleteBone(state, last_index);
          }
        }

        ImGui::EndMenu();
      }

      if (ImGui::MenuItem(ICON_FA_IMAGES " All Textures"))
        ;

      ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_GEARS " Preferences"))
      ;

    ImGui::EndMenu();
  }
}

void GlobalMenu::ToolsMenu(AppState& state) {
  if (ImGui::BeginMenu("Tools")) {
    if (ImGui::BeginMenu("Pallete")) {
      if (ImGui::MenuItem(ICON_FA_LOCATION_ARROW " Select"))
        ;

      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_ARROWS_ROTATE " Move"))
        ;
      if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Rotate"))
        ;
      if (ImGui::MenuItem(ICON_FA_EXPAND " Scale"))
        ;

      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_CIRCLE_PLUS " Create"))
        ;
      if (ImGui::MenuItem(ICON_FA_TRASH_CAN " Delete"))
        ;

      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_PERSON_RUNNING " IK"))
        ;

      ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_COMPACT_DISC " Bone Map"))
      ;
    ImGui::EndMenu();
  }
}

void GlobalMenu::HelpMenu(AppState&) {
  if (ImGui::BeginMenu("Help")) {
    if (ImGui::MenuItem(ICON_FA_BOOK " Tutorials"))
      ;
    if (ImGui::MenuItem(ICON_FA_BOOK_BIBLE " Documentation"))
      ;
    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_GLOBE " Github Repository"))
      ;
    ImGui::Separator();
    if (ImGui::MenuItem(ICON_FA_INFO " About"))
      ;

    ImGui::EndMenu();
  }
}
}  // namespace SIK
