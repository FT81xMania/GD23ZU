#include <GD23ZU.h>

void setup()
{
  GD.begin(GD_STORAGE);
}

void loop()
{
  MoviePlayer mp;

  mp.begin("fun-1500.avi");
  mp.play();
}
