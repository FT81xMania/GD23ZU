#include <GD23ZU.h>

static int a[256];

#include "walk_assets.h"

void setup()
{
  GD.begin(0);
  LOAD_ASSETS();
  for (int i = 0; i < 256; i++)
    a[i] = GD.random(512);
}

void loop()
{
  GD.ClearColorRGB(0x000050);
  GD.Clear();
  GD.Begin(BITMAPS);
  for (int i = 0; i < 256; i++) {
    GD.ColorRGB(i, i, i);
    GD.Vertex2ii(a[i], i, WALK_HANDLE, (a[i] >> 2) & 7);
    a[i] = (a[i] + 1) & 511;
  }
  GD.swap();
}

/*
    GD.Vertex2ii(x, y, WALK_HANDLE);
*/
