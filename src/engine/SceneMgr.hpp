#pragma once
#include <string>
#include <unordered_map>
class Scene;
class SceneMgr {
 public:
  void changeScene(const std::string& name);
  SceneMgr();

 private:
 std::unordered_map<std::string, Scene*> scene_assets = {};
};