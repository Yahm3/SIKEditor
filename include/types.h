#pragma once

#include <rlImGui.h>

#include <string>

namespace SIK {
enum class Mode {
  SELECT = 0,
  MOVE = 1,
  ROTATE = 2,
  RESIZE = 3,
  CREATE = 4,
  DELETE = 5,
  IK = 6,
  COUNT
};

inline const char* ModeName(Mode m) {
  switch (m) {
    case Mode::SELECT:
      return "Select";
    case Mode::MOVE:
      return "Move";
    case Mode::ROTATE:
      return "Rotate";
    case Mode::RESIZE:
      return "Resize";
    case Mode::CREATE:
      return "Create";
    case Mode::DELETE:
      return "Delete";
    case Mode::IK:
      return "IK";
    default:          return "?";
  }
}

inline const char* ModeIcon(Mode m) {
  switch (m) {
    case Mode::SELECT:
      return ICON_FA_ARROW_POINTER;
    case Mode::MOVE:
      return ICON_FA_UP_DOWN_LEFT_RIGHT;
    case Mode::ROTATE:
      return ICON_FA_ROTATE;
    case Mode::RESIZE:
      return ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER;
    case Mode::CREATE:
      return ICON_FA_CIRCLE_PLUS;
    case Mode::DELETE:
      return ICON_FA_TRASH;
    case Mode::IK:
      return ICON_FA_BONE;
    case Mode::COUNT:
      return ICON_FA_X;
  }

  return ICON_FA_QUESTION;
}

inline std::string CONSOLE_WINDOW = std::string(ICON_FA_TERMINAL) + " Console";
inline std::string HIERARCHY_WINDOW =
    std::string(ICON_FA_FOLDER_TREE) + " Hierarchy";
inline std::string PROPERTIES_WINDOW =
    std::string(ICON_FA_GEAR) + " Properties";
inline std::string TEXTURES_WINDOW = std::string(ICON_FA_IMAGE) + " Textures";
inline std::string TOOLS_WINDOW = std::string(ICON_FA_TOOLBOX) + " Tools";
enum SelectionType { NONE, BONE, SPRITE };
}  // namespace SIK
