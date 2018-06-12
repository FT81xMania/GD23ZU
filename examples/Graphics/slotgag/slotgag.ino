#include <GD23ZU.h>

#include "slotgag_assets.h"

void setup()
{
  delay(1000);Serial.begin(9600);
  GD.begin(GD_STORAGE);
  LOAD_ASSETS();
}

void loop()
{
  GD.Clear();
  GD.ColorMask(1, 1, 1, 0);
  GD.Begin(BITMAPS);
  GD.BitmapHandle(BACKGROUND_HANDLE);
  GD.BitmapSize(NEAREST, REPEAT, REPEAT, 480, 272);
  GD.Vertex2ii(0, 0, BACKGROUND_HANDLE);

  GD.ColorMask(1, 1, 1, 1);
  GD.ColorRGB(0xa0a0a0);
  GD.Vertex2ii(240 - GAMEDUINO_WIDTH / 2,
               136 - GAMEDUINO_HEIGHT / 2,
               GAMEDUINO_HANDLE);

  static int x = 0;
  GD.LineWidth(20 * 16);
  GD.BlendFunc(DST_ALPHA, ONE);
  GD.Begin(LINES);
  GD.Vertex2ii(x, 0);
  GD.Vertex2ii(x + 100, 272);
  x = (x + 20) % 480;

  GD.swap();
}
