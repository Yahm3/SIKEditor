#include <bone.h>
#include <imgui.h>
#include <modeevent.h>
#include <rig_editor.h>
#include <sprite.h>


namespace SIK {

std::unique_ptr<RigEditor> RigEditor::Create() {
  return std::make_unique<RigEditor>();
}

RigEditor::RigEditor() {}

RigEditor::~RigEditor() {}



void RigEditor::DrawUI() {
  ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + 20, viewport->Pos.y + 40));
  ImGui::SetNextWindowSize(
      ImVec2(viewport->Size.x - 40, viewport->Size.y - 70));
  ImGui::SetNextWindowViewport(viewport->ID);
  
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
                                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
                                  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                  ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
  
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Window", nullptr, window_flags);
  ImGui::PopStyleVar();

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::End();

  static bool first_time = true;
  if (first_time) {
      first_time = false;
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, (ImGuiDockNodeFlags)((int)ImGuiDockNodeFlags_DockSpace | (int)ImGuiDockNodeFlags_PassthruCentralNode));
      ImGui::DockBuilderSetNodeSize(dockspace_id,
                                    ImVec2(viewport->Size.x, viewport->Size.y));

      ImGuiID dock_main_id = dockspace_id;

      float split_ratio = 0.20f;
      ImGuiID dock_left =
          ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left,
                                      split_ratio - 0.05f, NULL, &dock_main_id);
      ImGuiID gap_left;
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.02f, &gap_left,
                                  &dock_main_id);

      ImGuiID dock_right = ImGui::DockBuilderSplitNode(
          dock_main_id, ImGuiDir_Right, split_ratio, NULL, &dock_main_id);
      ImGuiID gap_right;
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.02f,
                                  &gap_right, &dock_main_id);

      ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(
          dock_main_id, ImGuiDir_Down, split_ratio, NULL, &dock_main_id);
      ImGuiID gap_bottom;
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.02f,
                                  &gap_bottom, &dock_main_id);

      ImGuiID dock_right_top = ImGui::DockBuilderSplitNode(
          dock_right, ImGuiDir_Up, 0.60f, NULL, &dock_right);

      ImGui::DockBuilderDockWindow(HIERARCHY_WINDOW.c_str(), dock_left);
      ImGui::DockBuilderDockWindow(PROPERTIES_WINDOW.c_str(), dock_right_top);
      ImGui::DockBuilderDockWindow(TEXTURES_WINDOW.c_str(), dock_right);
      ImGui::DockBuilderDockWindow(CONSOLE_WINDOW.c_str(), dock_bottom);

      ImGui::DockBuilderFinish(dockspace_id);
  }

  AppState& state = GetState();
  menu.Draw(state);
  console.Draw(state);

  if (state.IsViewActive(Views::TOOLS)) {
    tools.Draw(state);
  }
  if (state.IsViewActive(Views::PROPERTIES)) {
    properties.Draw(state);
  }

  if (state.IsViewActive(Views::HIERARCHY)) {
    hierarchy.Draw(state);
  }
  if (state.IsViewActive(Views::TEXTURES)) {
    textures.Draw(state);
  }
}

void RigEditor::LoadFonts() {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("assets/DroidSans.ttf", 18.0f);

  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

  ImFontConfig config;
  config.MergeMode = true;
  config.PixelSnapH = true;

  io.Fonts->AddFontFromFileTTF("assets/fa-regular-400.ttf", 18.0f, &config,
                               icons_ranges);
}

int RigEditor::Run() {
  const int screenW = 1280, screenH = 800;
  AppState& state = GetState();

  std::string title = AppState::APPNAME + " | Simple IK Editor";
  console.Log("Setting Window flags");
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  console.Log("Window flags set up successfully");

  console.Log("Setting up window");
  InitWindow(screenW, screenH, title.c_str());
  console.Log("Window set up successfully");
  SetTargetFPS(60);

  console.Log("Enabling dockable");
  rlImGuiSetup(true);
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  console.Log("Setting up fonts");
  LoadFonts();
  console.Log("Fronts set up successfully");

  Camera2D& camera = state.GetCamera();

  camera.zoom = 1.0f;
  camera.offset = {screenW / 2.0f, screenH / 2.0f + 40};
  camera.target = {0, 0};

  console.Log("Creating default done");
  Bone::CreateBone(state, {0, 0}, {0, -200}, -1);

  console.Log("Settarting app up fonts");
  console.Log("App started successfully");
  while (!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(Color{30, 30, 35, 255});

    auto& sprites = state.GetSprites();
    auto& bones = state.GetBones();

    if (IsFileDropped()) {
      FilePathList files = LoadDroppedFiles();
      for (unsigned int i = 0; i < files.count; i++) {
        Texture2D tex = LoadTexture(files.paths[i]);
        if (tex.id != 0) {
          auto s = std::make_unique<Sprite>();
          s->SetTexture(tex);
          s->Name() = "Sprite_" + std::to_string(state.NextSpriteId());
          s->Position() = GetScreenToWorld2D(GetMousePosition(), camera);
          console.Log("Loaded texture: " + s->Name());
          sprites.push_back(std::move(s));
        }
      }
      UnloadDroppedFiles(files);
    }

    Vector2 mouseScreen = GetMousePosition();
    ImGuiIO& io = ImGui::GetIO();
    bool overUI = io.WantCaptureMouse || io.WantCaptureKeyboard;

    if (!overUI) {
      float wheel = GetMouseWheelMove();
      if (wheel != 0) {
        Vector2 beforeZoom = GetScreenToWorld2D(mouseScreen, camera);
        camera.zoom =
            std::max(0.15f, std::min(4.0f, camera.zoom + wheel * 0.1f));
        Vector2 afterZoom = GetScreenToWorld2D(mouseScreen, camera);
        camera.target =
            Math::V2Add(camera.target, Math::V2Sub(beforeZoom, afterZoom));
      }
    }
    if (!overUI && IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
      Vector2 delta = GetMouseDelta();
      camera.target = Math::V2Sub(
          camera.target, Math::V2Scale(delta, 1.0f / camera.zoom));
    }

    SelectionType& selection_type = state.GetSelectionType();
    int& selection_index = state.GetIndex();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    auto& dragging = state.GetDrag();
    Mode& currentMode = state.GetMode();

    if (!overUI) {
      switch (state.GetMode()) {
        case Mode::SELECT:
          ModeEvent::Select(state);
          break;

        case Mode::MOVE:
          ModeEvent::Move(state);
          break;

        case Mode::ROTATE:
          ModeEvent::Rotate(state);
          break;

        case Mode::RESIZE:
          ModeEvent::Resize(state);
          break;

        case Mode::CREATE:
          ModeEvent::Create(state);
          break;

        case Mode::DELETE:
          ModeEvent::Delete(state);
          break;

        case Mode::IK:
          ModeEvent::Ik(state);
          break;
      }

      if (IsKeyPressed(KEY_ONE)) currentMode = Mode::SELECT;
      if (IsKeyPressed(KEY_TWO)) currentMode = Mode::MOVE;
      if (IsKeyPressed(KEY_THREE)) currentMode = Mode::ROTATE;
      if (IsKeyPressed(KEY_FOUR)) currentMode = Mode::RESIZE;
      if (IsKeyPressed(KEY_FIVE)) currentMode = Mode::CREATE;
      if (IsKeyPressed(KEY_SIX)) currentMode = Mode::DELETE;
      if (IsKeyPressed(KEY_SEVEN)) currentMode = Mode::IK;
      if (IsKeyPressed(KEY_DELETE) && selection_type == BONE)
        Bone::DeleteBone(state, selection_index);
    }

    Math::UpdateSkeleton(state);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoInputs;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(
        ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));
    ImGui::Begin("##screen_viewport", nullptr, flags);

    BeginMode2D(camera);

    viewport.Draw(state);

    if (state.CreateDrag()) {
      Color pc = state.CreateParentId() >= 0 ? Color{120, 220, 140, 255}
                                             : Color{220, 220, 120, 255};
      DrawLineEx(state.StartWorld(), mouseWorld, 2.0f, pc);
      DrawCircleV(state.StartWorld(), 5, pc);
      if (state.CreateParentId() >= 0)
        DrawCircleLines((int)bones[state.CreateParentId()]->WorldEnd().x,
                        (int)bones[state.CreateParentId()]->WorldEnd().y, 12,
                        pc);
    }

    if (currentMode == Mode::IK && dragging && selection_type == BONE) {
      DrawCircleLines((int)mouseWorld.x, (int)mouseWorld.y, 8, RED);
      DrawLine((int)mouseWorld.x - 12, (int)mouseWorld.y, (int)mouseWorld.x + 12, (int)mouseWorld.y, RED);
      DrawLine((int)mouseWorld.x, (int)mouseWorld.y - 12, (int)mouseWorld.x, (int)mouseWorld.y + 12, RED);
    }

    EndMode2D();

    // Build the popup after raylib's 2D pass. The popup is still rendered by
    // rlImGuiEnd(), but no ImGui commands are issued while BeginMode2D is
    // active.
    viewport.DrawContextMenu(state);

    DrawUI();

    ImGui::End();
    rlImGuiEnd();
    EndDrawing();
  }

  for (auto& s : state.GetSprites()) UnloadTexture(s->Texture());
  rlImGuiShutdown();
  CloseWindow();
  
  return 0;
}

} // namespace SIK
