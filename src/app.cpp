#include "app.h"

namespace SIK {
namespace {
App* g_appInstance = nullptr;
}

App::App() : editor_(RigEditor::Create()) {}

App::~App() = default;

App& App::Instance() {
  static App instance;
  return instance;
}

std::unique_ptr<App> App::Create() {
  return std::make_unique<App>();
}

int App::Run() {
  if (!editor_) {
    return 1;
  }
  return editor_->Run();
}

RigEditor& App::Editor() {
  return *editor_;
}

bool App::IsRunning() const {
  return running_;
}

void App::ShutDown() {
  running_ = false;
}
}  // namespace SIK
