#include "SceneMgr.hpp"

#include "../scene/S_Init.hpp"
#include "GlobalVars.hpp"
#include "Scene.hpp"

void SceneMgr::changeScene(const std::string& name) {
  if (!scene_assets.count(name)) {
    SDL_Log("[E] <SceneMgr> Can't change scene to '%s': not found",
            name.c_str());
    return;
  }
  Scene* scene = scene_assets[name];
  if (G::current_scene) G::current_scene->exit();
  G::current_scene = scene;
  G::current_scene->enter();
}
SceneMgr::SceneMgr() {
  scene_assets["init"] = new S_Init;
  // 实例化和创建所有Scene，加载到scene池，并enter第一个（首字母顺序），
  // 如果出现异常则设置G::status = SDL_APP_FAILURE，并return
}