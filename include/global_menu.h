#pragma once
#include <appstate.h>

namespace SIK {
class GlobalMenu {
 public:
  GlobalMenu();
  ~GlobalMenu();

  void Draw(AppState&);

 private:
  void FileMenu(AppState&);
  void WindowMenu(AppState&);
  void EditMenu(AppState&);
  void ToolsMenu(AppState&);
  void HelpMenu(AppState&);
};
}
