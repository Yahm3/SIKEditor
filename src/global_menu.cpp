#include <global_menu.h>

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
      if (ImGui::MenuItem("Selected Bone"))
        ;
      if (ImGui::MenuItem("Selected Texture"))
        ;

      if (ImGui::MenuItem("All Bones"))
        ;
      if (ImGui::MenuItem("All Textures"))
        ;

      ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Preferences"))
      ;

    ImGui::EndMenu();
  }
}

void GlobalMenu::ToolsMenu(AppState& state) {
  if (ImGui::BeginMenu("Tools")) {
    if (ImGui::BeginMenu("Pallete")) {
      if (ImGui::MenuItem("Select"))
        ;
      if (ImGui::MenuItem("Move"))
        ;
      if (ImGui::MenuItem("Rotate"))
        ;
      if (ImGui::MenuItem("Scale"))
        ;

      ImGui::Separator();
      if (ImGui::MenuItem("Create"))
        ;
      if (ImGui::MenuItem("Delete"))
        ;
      if (ImGui::MenuItem("IK"))
        ;

      ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Bone Map"))
      ;
    ImGui::EndMenu();
  }
}

void GlobalMenu::HelpMenu(AppState&) {
  if (ImGui::BeginMenu("Help")) {
    if (ImGui::MenuItem("Tutorials"))
      ;
    if (ImGui::MenuItem("Documentation"))
      ;
    ImGui::Separator();
    if (ImGui::MenuItem("Github Repository"))
      ;
    ImGui::Separator();
    if (ImGui::MenuItem("About"))
      ;

    ImGui::EndMenu();
  }
}
}  // namespace SIK
