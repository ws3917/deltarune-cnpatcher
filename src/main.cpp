#include <physfs.h>

#include <SFML/System.hpp>
#include <fstream>

#include "engine/AssetMgr.hpp"
#include "values.hpp"

int main(int argc, char** argv) {
  PHYSFS_init(argv[0]);
  // 日志
  std::ofstream error_file(V::LOGFILE);
  sf::err().rdbuf(error_file.rdbuf());
  // 存档路径
  const char* pref_dir = PHYSFS_getPrefDir(V::AUTHOR, V::PROJECTNAME);
  PHYSFS_setWriteDir(pref_dir);
  PHYSFS_mount(pref_dir, "/", 0);
  // 资源包
  const std::string data_path = std::string(PHYSFS_getBaseDir()) + V::DATAFILE;
  if (!PHYSFS_mount(data_path.c_str(), "/", 1)) {
    sf::err() << std::format("[C] Can't load data file '{}': {}\n", V::DATAFILE,
                             PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    PHYSFS_deinit();
    return 1;
  }
  sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
  // Load a sprite to display
  sf::Texture tex = am::loadTexture("bg_static");
  sf::Sprite sprite(tex);
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) window.close();
    }
    window.clear();
    window.draw(sprite);
    window.display();
  }
  PHYSFS_deinit();
}