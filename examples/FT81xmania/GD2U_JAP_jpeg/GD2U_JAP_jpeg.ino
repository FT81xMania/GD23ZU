#include <GD23ZU.h>

void setup()
{

  GD.begin(GD_STORAGE);
  GD.cmd_loadimage(0, 0);
  GD.load("tree.jpg");
}

void loop()
{
  GD.Clear();
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0);
  GD.swap();
}
