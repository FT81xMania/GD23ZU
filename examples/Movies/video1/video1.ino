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
  if (fileNumber == 0) mp.begin("hands.avi");
  if (fileNumber == 1) mp.begin("niki.avi");
  if (fileNumber == 2) mp.begin("utopia.avi");
  if (fileNumber == 3) mp.begin("sultans.avi");
  if (fileNumber == 4) mp.begin("gost.avi");
  mp.play();
}
