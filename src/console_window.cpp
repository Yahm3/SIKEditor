#include <console_window.h>

namespace SIK {
ConsoleWindow::ConsoleWindow() {}
ConsoleWindow::~ConsoleWindow() {}

void ConsoleWindow::Log(const std::string& msg) {
  messages.push_back(msg);
}

void ConsoleWindow::Draw(AppState& state) {
  if (!state.IsViewActive(Views::CONSOLE)) return;

  bool& show_view = state.GetView(Views::CONSOLE);
  static int selectedLine = -1;

  ImGui::Begin(CONSOLE_WINDOW.c_str(), &show_view);

  ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false,
                    ImGuiWindowFlags_HorizontalScrollbar);

  for (size_t i = 0; i < messages.size(); ++i) {
    bool selected = (selectedLine == (int)i);

    if (ImGui::Selectable(messages[i].c_str(), selected,
                          ImGuiSelectableFlags_SpanAvailWidth)) {
      selectedLine = (int)i;
    }
  }

  if (ImGui::BeginPopupContextWindow()) {
    if (ImGui::MenuItem("Copy Line")) {
      if (selectedLine > -1) {
        ImGui::SetClipboardText(messages[selectedLine].c_str());
        Log("Copied to clipboard");
      }
    }

    if (ImGui::MenuItem("Copy Console")) {
      std::string text;
      for (const auto& msg : messages) {
        text += msg;
        text += '\n';
      }
      if (text.size() > 0) {
        ImGui::SetClipboardText(text.c_str());
        Log("Copied to clipboard");
      }
    }

    if (ImGui::MenuItem("Clear")) {
      messages.clear();
      selectedLine = -1;
    }

    ImGui::EndPopup();
  }

  ImGui::EndChild();
  ImGui::End();
}
}
