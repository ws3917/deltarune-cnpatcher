#pragma once
// 这里是主线程循环
#include <SDL3/SDL.h>

#include <string>
class Scene;
class GameMgr {
 public:
  // 顶层接口
  void input(SDL_Event* event);
  void update(float delta);
  void draw();
  void changeScene(Scene*);
  GameMgr();
  ~GameMgr();

 private:
  void loadAssets(const std::string& path);
};