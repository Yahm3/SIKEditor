#pragma once
#include <appstate.h>

#include <string>
#include <vector>

namespace SIK {
class ConsoleWindow {
 public:
  ConsoleWindow();
  ~ConsoleWindow();

  void Log(const std::string& msg);
  void Draw(AppState&);

 private:
  std::vector<std::string> messages;
  bool autoScroll = true;
};
}
