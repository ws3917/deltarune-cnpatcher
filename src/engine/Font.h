#pragma once
#include <string>

struct BlockCommon
{
    uint16_t lineHeight;
    uint16_t base;
    uint16_t scaleW;
    uint16_t scaleH;
    uint16_t _1;
    uint8_t _2;
    uint8_t _3;
    uint8_t _4;
    uint8_t _5;
    uint8_t _6;
};

// Block type 4: 字符信息 (对应文档 Version 3)
struct BlockChar
{
    uint32_t id; // 4 bytes (Version 3)
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t xoffset;
    int16_t yoffset;
    int16_t xadvance;
    uint8_t _1;
    uint8_t _2;
};

class Font
{
  public:
    bool load(const std::string &path);
};