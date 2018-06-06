#include <GD23ZU.h>

void setup()
{
  GD.begin(GD_STORAGE);
}

void loop()
{
  MoviePlayer mp;

  int randNumber = random(300);
  int fileNumber = randNumber % 5;
  if (fileNumber == 0) mp.begin("fun-1500.avi");
  if (fileNumber == 1) mp.begin("dj-1500.avi");
  if (fileNumber == 2) mp.begin("spa-1500.avi");
  if (fileNumber == 3) mp.begin("tra-1500.avi");
  if (fileNumber == 4) mp.begin("tub-1500.avi");
  mp.play();
}
