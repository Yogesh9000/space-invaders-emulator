#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdint.h>

class Display {
public:
  Display(const char* Title, uint16_t _Width, uint16_t _Height,
          uint16_t _PixelSize);
  ~Display();
  void Update(uint8_t* VRAMMap);

private:
  uint16_t PixelSize;
  uint16_t Width;
  uint16_t Height;
  uint16_t Pixels[224 * 256];
  SDL_Window* MainWindow;
  SDL_Renderer* MainRenderer;
  SDL_Texture* MainTexture;
};

#endif
