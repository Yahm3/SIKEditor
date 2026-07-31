#pragma once

#include <memory>
#include "rig_editor.h"

namespace SIK {
class App {
 public:
  static App& Instance();
  static std::unique_ptr<App> Create();

  int Run();
  RigEditor& Editor();
  bool IsRunning() const;
  void ShutDown();

  App();
  ~App();

 private:

  std::unique_ptr<RigEditor> editor_;
  bool running_ = true;
};
}  // namespace SIK
