#pragma once
#include <appstate.h>
#include <console_window.h>
#include <global_menu.h>
#include <hierarchy_window.h>
#include <properties_window.h>
#include <raylib.h>
#include <textures_window.h>
#include <tools_window.h>
#include <viewport.h>

#include <memory>
#include <vector>

namespace SIK {
class RigEditor {
 public:
  static std::unique_ptr<RigEditor> Create();

  RigEditor();
  ~RigEditor();

  RigEditor(const RigEditor&) = delete;
  RigEditor& operator=(const RigEditor&) = delete;

  RigEditor(RigEditor&&) = delete;
  RigEditor& operator=(RigEditor&&) = delete;

  int Run();

 private:
  void DrawUI();
  void LoadFonts();

  AppState& GetState() { return m_state; }

  ConsoleWindow console;
  GlobalMenu menu;
  ToolsWindow tools;
  PropertiesWindow properties;
  HierarchyWindow hierarchy;
  TexturesWindow textures;
  Viewport viewport;

  AppState m_state;
};
}  // namespace SIK
