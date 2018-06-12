#include <GD23ZU.h>

#include "sprites_assets.h"

void setup()
{
  GD.begin(0);

  LOAD_ASSETS();
}

static byte t;

void loop()
{
  GD.Clear();
  GD.Begin(BITMAPS);
  byte j = t;
  uint32_t v, r;

  int nspr = min(2001, max(256, 19 * t));

  const PROGMEM uint32_t *pv = sprites;
  for (int i = 0; i < nspr; i++) {
    v = pgm_read_dword(pv++);
    r = pgm_read_dword(circle + j++);
    GD.cmd32(v + r);
  }

  GD.ColorRGB(0x000000);
  GD.ColorA(140);
  GD.LineWidth(28 * 16);
  GD.Begin(LINES);
  GD.Vertex2ii(240 - 110, 136, 0, 0);
  GD.Vertex2ii(240 + 110, 136, 0, 0);

  GD.RestoreContext();

  GD.cmd_number(215, 110, 31, OPT_RIGHTX, nspr);
  GD.cmd_text(  229, 110, 31, 0, "sprites");

  GD.swap();
  t++;
}
