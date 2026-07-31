#define NOB_IMPLEMENTATION
#include "nob.h"

#include <dirent.h>
#include <string.h>

#define BUILD_DIR "build"
#define APP "SIK"
#define OUTPUT BUILD_DIR "/" APP

static bool has_cpp_extension(const char* name) {
  size_t len = strlen(name);
  return len >= 4 && strcmp(name + len - 4, ".cpp") == 0;
}

int main(int argc, char** argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!mkdir_if_not_exists(BUILD_DIR)) return 1;

  char* raylib_home = getenv("RAYLIB_HOME");

  if (raylib_home == NULL) {
    nob_log(NOB_ERROR, "RAYLIB_HOME is not set");
    return 1;
  }

  Nob_Cmd cmd = {0};

  nob_cmd_append(&cmd, "g++", "-ggdb", "-std=c++20", "-Iexternal/imgui",
                 "-Iinclude/", "-Iexternal/imnodes", "-Iexternal/rlImGui",
                 "-Iexternal/impie");

  DIR* dir = opendir("src");

  if (!dir) {
    nob_log(NOB_ERROR, "Could not open src directory");
    return 1;
  }

  struct dirent* entry;

  while ((entry = readdir(dir)) != NULL) {
    if (!has_cpp_extension(entry->d_name)) continue;

    size_t len = strlen(entry->d_name);

    char* path = malloc(len + 5 + 1);  // "src/" + name + '\0'

    if (!path) {
      nob_log(NOB_ERROR, "Out of memory");
      closedir(dir);
      return 1;
    }

    sprintf(path, "src/%s", entry->d_name);

    nob_cmd_append(&cmd, path);
  }

  closedir(dir);

  nob_cmd_append(
      &cmd,

      // imgui
      "external/imgui/imgui.cpp", "external/imgui/imgui_draw.cpp",
      "external/imgui/imgui_tables.cpp", "external/imgui/imgui_widgets.cpp",
      "external/imgui/misc/cpp/imgui_stdlib.cpp",

      // rlimgui
      "external/rlImGui/rlImGui.cpp",

      // imnodes
      "external/imnodes/imNodes.cpp", "external/imnodes/imNodesEz.cpp",

      // impie
      "external/impie/impie.cpp");

  char raylib_include[1024];
  snprintf(raylib_include, sizeof(raylib_include), "-I%s/include/",
           raylib_home);

  char raylib_lib[1024];
  snprintf(raylib_lib, sizeof(raylib_lib), "-L%s/lib/", raylib_home);

  nob_cmd_append(&cmd, "-o", OUTPUT, raylib_include, raylib_lib, "-lraylib",
                 "-lopengl32", "-lgdi32", "-lwinmm", "-lshell32");

  if (!nob_cmd_run_sync(cmd)) return 1;

  return 0;
}