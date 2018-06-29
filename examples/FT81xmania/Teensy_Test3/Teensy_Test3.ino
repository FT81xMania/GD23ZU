#include "Arduino.h"
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
//#include <itoa.h>
#include <GD23ZU.h>

#include "hassets.h"


xy center;

uint16_t ai = 0; // currently only used to demo moving red glow

bool demo = true;

int16_t RPM = 0;
uint8_t SPEED = 0;

uint8_t speedRANGE = 120;
uint16_t rpmRANGE = 12000;

#define speedSTEP 5
#define rpmSTEP 1000

#define speedSTEPS  (speedRANGE / speedSTEP)
#define rpmSTEPS  (rpmRANGE / rpmSTEP)

float ODO = 0;
float TRIP = 1888.15;

uint8_t GEAR = 0;
uint8_t oldGEAR = 0;
uint8_t frameGEAR = 0;

uint16_t NORTH = 0; // angle that north is at

uint8_t tilt_angle = 0;
uint8_t volt = 120; //10=1v, 100=10v
uint8_t watertemp = 80;

bool highBEAM = false;
bool LEFT = false;
bool RIGHT = false;
bool HAZ = false;
bool FUEL = false;
bool OIL = false;
bool HAZ_ON = false;
bool FUEL_ON = false;

uint8_t indANI = 0;
uint8_t hazANI = 0;
uint8_t fuelANI = 0;

float oldDELTASPEED = 0;
float oldDELTARPM = 0;

uint16_t speedPOSLENGTH = 231; // number of pixels from centre point to placement arc position

void polar(xy &p, int radius, int angle)
{
  p.x = center.x - GD.rsin(radius, angle);
  p.y = center.y + GD.rcos(radius, angle);
}

char* astext(int n)
{
  static char buffer[5];
  itoa(n, buffer, 10);
  return buffer;
}

void polar_line(int r0, int r1, int angle)
{
  xy p;

  GD.Begin(LINES);
  polar(p, r0, angle);
  p.draw();
  polar(p, r1, angle);
  p.draw();
}

void rotateTEXT(char* text, uint8_t font_handle, uint16_t x, uint16_t y, int16_t angle,
    uint8_t bmp_width, uint8_t bmp_height, uint8_t space, uint8_t scaleX, uint8_t scaleY)
{
  /*
   * Bmp_width/height hopefully would be automated in time. But currently manually inputed.
   *
   * space is the space between characters, how close they bunch up.
   *
   * 100 = 100% normal spacing 50 = 50% spacing bringing characters closer by half
   *
   * 0 would place all characters at the same point.
   *
   * scaleX scales the text in X reducing or enlarging its width, y does it in y.
   * When scaling X the whole text block is scaled together.
   *
   * If scaling X and Y together you can get finer font sizes between set fonts, at the cost of text bitmap image quality when enlarging.
   * Useful for minor font size changes.
   *
   */

  uint16_t frame_width = space * (scaleX * bmp_width / 100) / 100;
  uint16_t frame_height = space * (scaleY * bmp_height / 100) / 100;

  int frame_size = (frame_width > frame_height ? frame_width : frame_height);

  x = x / 16;
  y = y / 16;

  x = x - frame_size / 2 - (15 * frame_size / 100); // - (frame_size / 12); //(bmp_width*0.3);
  y = y - frame_size / 2 - (15 * frame_size / 100); // - (frame_size / 17); //(bmp_height*0.3);//(bmp_height*.2);

  int size = strlen(text);

  GD.SaveContext();
  GD.Begin(BITMAPS);

  for (uint8_t i = 0; i < size; i++)
  {
    GD.cmd_loadidentity();

    GD.cmd_translate((frame_size * 65536) / 2 + (15 * frame_size / 100 * 65536),
        (frame_size * 65536) / 2 + (15 * frame_size / 100 * 65536));
    GD.cmd_rotate(angle);
    GD.cmd_translate(-(frame_size * 65536) / 2 + (15 * frame_size / 100 * 65536),
        -(frame_size * 65536) / 2 + (15 * frame_size / 100 * 65536));

    if (scaleX != 100 || scaleY != 100)
    {
      GD.cmd_translate((frame_size * 65536) / 2, (frame_size * 65536) / 2);
      GD.cmd_scale(65536 / 100 * scaleX, 65536 / 100 * scaleY);
      GD.cmd_translate(-(frame_size * 65536) / 2, -(frame_size * 65536) / 2);
    }

    GD.cmd_setmatrix();

    GD.BitmapHandle(font_handle);
    GD.BitmapSize(BILINEAR, BORDER, BORDER, (frame_size * 1.3), (frame_size * 1.3));

    GD.Cell(static_cast<int>(text[i]));

    uint16_t radius = 0;

    if (size % 2 == 0)
    {
      radius = -(frame_width * (size / 2)) + (frame_width * (i + .5));
    }
    else
    {
      radius = -(frame_width * (size / 2)) + (frame_width * (i));
    }

    uint16_t ax = 16 * x + GD.rcos(16 * radius, angle);
    uint16_t ay = 16 * y + GD.rsin(16 * radius, angle);

    //uint16_t ax = (x + (radius) * cos(angleX)) * 16;
    //int16_t ay = (y+ (radius) * sin(angleX)) * 16;

    GD.Vertex2f(ax, ay);

  }

  //GD.cmd_loadidentity();
  //GD.cmd_setmatrix();
  GD.End();
  GD.RestoreContext();
}

void setup()
{
//  Serial.begin(1000000);
  //Serial.println("Setup Begun");
  //accelINIT();
  GD.begin(GD_STORAGE);
  //SD.begin();

  //GD.wr16(REG_HCYCLE, 928);
//  GD.wr16(REG_HCYCLE, 980);
//  GD.wr16(REG_HOFFSET, 88);
//  GD.wr16(REG_HSIZE, 800);
//  GD.wr16(REG_VCYCLE, 525);//525
//  GD.wr16(REG_VOFFSET, 32);
//  GD.wr16(REG_VSIZE, 480);
//  GD.wr16(REG_CSPREAD, 0);
//  GD.wr16(REG_ROTATE, 0);
//  GD.wr16(REG_PCLK, 2); //4
//  GD.wr16(REG_SWIZZLE, 1);
  
  
//  GD.w = 800;
//  GD.h = 480;

//  Serial.println("Display Set");

  center.set(6380, 5640); //tweaked centre point as stuff just wasnt alinging up... the bitmaps seem correct. But the centre point just ploaced objects a little off....

//    MoviePlayer mp;
//
//    mp.begin("spa-1500.avi");
//    mp.play();

//
//    for (int i = 127; i >= 0; i--)
//  {
//  GD.wr(REG_PWM_DUTY, i);
//  delay(2);
//  }
//
//    GD.begin();
//    GD.wr(REG_PWM_DUTY, 0);
//
//    GD.wr16(REG_HCYCLE, 928);
//    GD.wr16(REG_HOFFSET, 88);
//    GD.wr16(REG_HSIZE, 800);
//    GD.wr16(REG_VCYCLE, 525);
//    GD.wr16(REG_VOFFSET, 32);
//    GD.wr16(REG_VSIZE, 480);
//    GD.wr16(REG_CSPREAD, 0);
//    GD.wr16(REG_ROTATE, 0);
//    GD.wr16(REG_PCLK, 2); //4
//    //GD.wr16(REG_SWIZZLE, 0);
//    GD.w = 800;
//    GD.h = 480;


  LOAD_ASSETS();
  //GD.wr(REG_PWM_DUTY, 128);
//  char num[] ="01234";
//  while(1)
//  {
//    for (int i =0 ;i < 360 ;i++)
//  {
//      GD.Clear();
//      GD.Begin(POINTS);
//      GD.Vertex2f(PIXELS(400),PIXELS(240));
//      GD.End();
//      rotateTEXT(num,6,PIXELS(400),PIXELS(240),DEGREES(i),0,12,17);
//
//      GD.swap();
//  }
//  }

}

void gearDISPLAY()
{    //static const shape_t OCRASTD80_SHAPE = {8, 58, 82, 0};
  uint16_t x = 400;
  uint16_t y = 255;
  uint8_t frame_size = 58;
  uint8_t frameCNT = 20;
  char* gearchar;    //= (GEAR == 0 ? "N" : astext(GEAR));

  GD.SaveContext();

  if (oldGEAR != GEAR)
  {
    for (uint8_t i = 0; i <= 1; i++)    //loop to handle both new and old gear graphics
    {
      if ((i == 0 ? GEAR : oldGEAR) == 0)
      {
        GD.ColorRGB(0, 255, 0);
        gearchar = "/";
      }
      else
      {
        GD.ColorRGB(255, 255, 255);
        gearchar = (i == 0 ? astext(GEAR) : astext(oldGEAR));
      }

      GD.cmd_loadidentity();
      GD.cmd_translate((frame_size * 65536) / 2, (frame_size * 65536) / 2);

      if (i == 0)    //new gear
      {
        if (GEAR < oldGEAR)
        {
          x = 400 + ((frameGEAR - frameCNT) * frame_size / frameCNT / 2);
        }
        else
        {
          x = 400 - ((frameGEAR - frameCNT) * frame_size / frameCNT / 2);
        }
        y = 255 - ((frameGEAR - frameCNT) * 41 / frameCNT);
        GD.ColorA(frameGEAR * 255 / frameCNT);
        GD.cmd_scale(frameGEAR * 65536 / frameCNT, frameGEAR * 65536 / frameCNT);
      }
      else if (i == 1)   //old gear
      {
        if (GEAR < oldGEAR)
        {
          x = 400 + (frameGEAR * frame_size / frameCNT / 2);
        }
        else
        {
          x = 400 - (frameGEAR * frame_size / frameCNT / 2);
        }
        y = 255 + (frameGEAR * 41 / frameCNT);
        GD.ColorA(255 - (frameGEAR * 255 / frameCNT));
        GD.cmd_scale((frameCNT - frameGEAR) * 65536 / frameCNT,
            (frameCNT - frameGEAR) * 65536 / frameCNT);
      }

      GD.cmd_translate(-(frame_size * 65536) / 2, -(frame_size * 65536) / 2);
      GD.cmd_setmatrix();

      GD.cmd_text(x, y, 8, OPT_CENTER, gearchar);

    }

    if (frameGEAR == frameCNT)    //once the animation complete
    {
      oldGEAR = GEAR;
      frameGEAR = 0;
      Serial.println("Finished");
    }

    frameGEAR++;    // increasing animation frame
  }
  else    // display already set gear no animation
  {
    if (GEAR == 0)
    {
      GD.ColorRGB(0, 255, 0);
      gearchar = "/";
    }
    else
    {
      GD.ColorRGB(255, 255, 255);
      gearchar = astext(GEAR);
    }
    GD.cmd_text(x, y, 8, OPT_CENTER, gearchar);
  }

  GD.RestoreContext();
}

void light_glow()
{

//  GD.BitmapHandle(1);
//  GD.cmd_setbitmap(0x5dc00UL, L8, 680, 466);
//  GD.BitmapSize(BILINEAR, BORDER, BORDER, 680, 466);

  GD.SaveContext();
  if (HAZ_ON == true && HAZ == true)    //hazard
  {
    GD.ColorRGB(255, 100, 0);
    bitmaps.high_beam.draw(400, 247, 0);

    GD.ColorRGB(255, 80, 120);

    GD.BlendFunc(SRC_ALPHA, ONE);
    bitmaps.high_beam.draw(400, 247, 0);
  }
  else if (highBEAM == true && HAZ_ON == false)
  {
    GD.ColorRGB(0, 100, 255);
    bitmaps.high_beam.draw(400, 247, 0);
    GD.ColorRGB(255, 255, 255);
    //GD.ColorA(150);
    GD.BlendFunc(SRC_ALPHA, ONE);
    bitmaps.high_beam.draw(400, 247, 0);
  }
  GD.RestoreContext();
}
void high_beam()
{
  if (highBEAM == true)
  {
    GD.ColorRGB(0, 213, 255);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(1);
    GD.Vertex2f(470, 2865);
    GD.End();
  }

  GD.RestoreContext();

}
void left_ind()
{
  if (LEFT == true)
  {
//#define originX = 1366;

    GD.SaveContext();
    GD.ColorRGB(80, 255, 80);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(0);

    int x = 1366 - (indANI * 100);
    indANI = (indANI >= 20 ? 0 : indANI + 1);

    GD.Vertex2f(x, 801);
    GD.End();
    GD.RestoreContext();
  }
}
void right_ind()
{
  if (RIGHT == true)
  {
    GD.SaveContext();
    GD.cmd_loadidentity();
    GD.cmd_translate(1638400, 1638400);
    GD.cmd_rotate(32768);
    GD.cmd_translate(-1638400, -1638400);
    GD.cmd_setmatrix();

    GD.ColorRGB(80, 255, 80);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(0);

    int x = 10590 + (indANI * 100);
    indANI = (indANI >= 20 ? 0 : indANI + 1);

    GD.Vertex2f(x, 801);
    GD.End();
    GD.cmd_loadidentity();
    GD.cmd_setmatrix();
    GD.RestoreContext();
  }
}
void haz_ind()
{
  if (HAZ == true)
  {
    hazANI++;
    if (hazANI == 10)
    {
      hazANI = 0;
      HAZ_ON = !HAZ_ON;
    }
  }
  if (HAZ_ON == true && HAZ == true)    //hazard
  {
    GD.SaveContext();
    GD.ColorRGB(255, 0, 4);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(5);
    GD.Vertex2f(11814, 865);
    GD.End();

    //hazard
    GD.ColorRGB(255, 0, 4);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(5);
    GD.Vertex2f(182, 865);
    GD.End();
    GD.RestoreContext();
  }

}
void fuel_ind()
{
  if (FUEL == true)
  {
    fuelANI++;
    if (fuelANI == 10)
    {
      fuelANI = 0;
      FUEL_ON = !FUEL_ON;
    }
  }
  if (FUEL_ON == true && FUEL == true)
  {
    GD.SaveContext();
    GD.ColorRGB(255, 0, 4);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(4);
    GD.Vertex2f(166, 2017);
    GD.End();
    GD.RestoreContext();
  }
  else
  {
    GD.SaveContext();
    GD.ColorRGB(10, 1, 2);
    GD.Begin(BITMAPS);
    GD.BitmapHandle(3);
    GD.Cell(4);
    GD.Vertex2f(166, 2017);
    GD.End();
    GD.RestoreContext();
  }
}
void oil_ind()
{
  GD.SaveContext();
  GD.ColorRGB(255, 0, 4);
  GD.Begin(BITMAPS);
  GD.BitmapHandle(3);
  GD.Cell(3);
  GD.Vertex2f(11462, 2913);
  GD.End();
  GD.RestoreContext();
}
void temp_ind()
{
  GD.SaveContext();
  GD.ColorRGB(0, 187, 255);
  GD.Begin(BITMAPS);
  GD.BitmapHandle(3);
  GD.Cell(7);
  GD.Vertex2f(11798, 2081);
  GD.End();
  GD.RestoreContext();
}
void water_temp()
{
  GD.SaveContext();
  GD.ColorRGB(255, 255, 255);
  GD.cmd_text(547, 408, 5, 0, "60c");
  GD.cmd_text(557, 363, 5, 0, "80c");
  GD.cmd_text(548, 319, 5, 0, "100c");
  GD.cmd_loadidentity();
  GD.cmd_scale(32768, 32768);
  GD.cmd_setmatrix();

  GD.ColorRGB(0, 145, 255);

  GD.Begin(BITMAPS);
  GD.BitmapHandle(3);
  GD.Cell(6);
  GD.Vertex2f(8406, 6977);
  GD.End();

  GD.cmd_loadidentity();
  GD.cmd_setmatrix();
  GD.RestoreContext();
}
void batt_ind()
{
  GD.SaveContext();
  GD.ColorRGB(215, 255, 251);
  GD.cmd_text(227, 408, 5, 0, "10v");
  GD.cmd_text(216, 363, 5, 0, "12v");
  GD.cmd_text(218, 319, 5, 0, "14v");
  GD.cmd_loadidentity();
  GD.cmd_scale(32768, 32768);
  GD.cmd_setmatrix();

  GD.ColorRGB(255, 0, 4);

  GD.Begin(BITMAPS);
  GD.BitmapHandle(3);
  GD.Cell(2);
  GD.Vertex2f(4022, 7041);
  GD.End();

  GD.cmd_loadidentity();
  GD.cmd_setmatrix();
  GD.RestoreContext();
}
void comp_disp()
{
  GD.SaveContext();

  GD.ScissorXY(210, 160);
  GD.ScissorSize(380, 125);

////////////////////////POSITION AND RENDER COMPASS///////////////////////////////
  GD.ColorRGB(255, 255, 255);
  uint16_t angle;
  xy p;
  for (uint8_t i = 0; i < 16; i++)
  {
    angle = map(i, 0, 16, 0, DEGREES(360)); // - DEGREES(180);

    polar(p, PIXELS(174), angle + DEGREES(NORTH) - DEGREES(180));

    if (p.y < 4800)
    {
      switch (i)
      {
      case 0:
        rotateTEXT("N", 7, p.x, p.y, angle + DEGREES(NORTH), 23, 33, 100, 100, 100);
        break;
      case 1:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;
      case 2:
        rotateTEXT("NE", 6, p.x, p.y, angle + DEGREES(NORTH), 12, 17, 100, 100, 100);
        break;
      case 3:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;

      case 4:
        rotateTEXT("E", 7, p.x, p.y, angle + DEGREES(NORTH), 23, 33, 100, 100, 100);
        break;
      case 5:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;

      case 6:
        rotateTEXT("SE", 6, p.x, p.y, angle + DEGREES(NORTH), 12, 17, 100, 100, 100);
        break;
      case 7:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;

      case 8:
        rotateTEXT("S", 7, p.x, p.y, angle + DEGREES(NORTH), 23, 33, 100, 100, 100);
        break;
      case 9:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;

      case 10:
        rotateTEXT("SW", 6, p.x, p.y, angle + DEGREES(NORTH), 12, 17, 100, 100, 100);
        break;
      case 11:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;

      case 12:
        rotateTEXT("W", 7, p.x, p.y, angle + DEGREES(NORTH), 23, 33, 100, 100, 100);
        break;
      case 13:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;

      case 14:
        rotateTEXT("NW", 6, p.x, p.y, angle + DEGREES(NORTH), 12, 17, 100, 100, 100);
        break;
      case 15:
        GD.Begin(POINTS);
        GD.PointSize(50);
        GD.Vertex2f(p.x, p.y);
        GD.End();
        break;
      }
    }
  }

  GD.RestoreContext();
}
void dispTRIP()
{
  uint32_t trip = (TRIP * 100); // taking the float to 2 decimal places and placing that into a whole number int, 32bit as its not unrealistic as before decimal could be over 64k
  // c6, c5, c4, c3, c2, '.' c1,( unseen final decimal)

  char c[9];
  itoa(trip, c, 10);

  c[0] = (trip % 10);
  c[1] = (trip / 10 % 10);
  c[2] = 46;
  c[3] = (trip / 100 % 10);
  c[4] = (trip / 1000 % 10);
  c[5] = (trip / 10000 % 10);
  c[6] = (trip / 100000 % 10);
  c[7] = (trip / 1000000 % 10);
  c[8] = (trip / 10000000 % 10);

  GD.SaveContext();
  GD.Begin(BITMAPS);
  //23 x 33 font size
  uint16_t y = 17;
  uint16_t x = 758;
  int16_t by = 0;
  for (uint8_t i = 0; i < 9; i++)
  {
    GD.BitmapHandle(7);

    if (i == 2)
    {
      GD.Cell(46);
      GD.Vertex2f((758 - 23) * 16, 18 * 16);

    }
    else if (i != 0)
    {
      int16_t ay = 0;
      int16_t ax = 0;

      char b = 0;

      b = (c[i] < 0 ? 9 : c[i]);
      b = (c[i] > 9 ? 0 : c[i]);
      GD.Cell(48 + b);
      ax = (x - (23 * (i - 1)));

      if (i == 1)
      {
        ay = y + (33 * c[0] / 10);
        GD.Vertex2f(ax * 16, ay * 16);

        b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
        GD.Cell(b + 48);
        ay = (ay - 33);
        GD.Vertex2f(ax * 16, ay * 16);

      }
      else
      {

        switch (i)

        {

        case 3:
          if (c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 4:
          if (c[3] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 5:
          if (c[4] == 9 && c[3] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 6:
          if (c[5] == 9 && c[4] == 9 && c[3] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 7:
          if (c[6] == 9 && c[5] == 9 && c[4] == 9 && c[3] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 8:
          if (c[7] == 9 && c[6] == 9 && c[5] == 9 && c[4] == 9 && c[3] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;
        }

        GD.Vertex2f(ax * 16, ay * 16);

      }

    }

  }

  GD.ColorRGB(0, 0, 0);
  GD.Begin(RECTS);
  GD.Vertex2f(12800, 200);
  GD.Vertex2f(9000, 0);
  GD.End();

  GD.Begin(RECTS);
  GD.Vertex2f(12800, 1360);
  GD.Vertex2f(9000, 752);
  GD.End();

  GD.RestoreContext();
  GD.End();

}
void dispODO()
{
  uint32_t odo = (ODO * 10); // taking the float to 2 decimal places and placing that into a whole number int, 32bit as its not unrealistic as before decimal could be over 64k
  // c6, c5, c4, c3, c2, '.' c1,( unseen final decimal)

  char c[10];
  itoa(odo, c, 10);

  c[0] = (odo % 10);
  c[1] = (odo / 10 % 10);
  c[2] = (odo / 100 % 10);
  c[3] = (odo / 1000 % 10);
  c[4] = (odo / 10000 % 10);
  c[5] = (odo / 100000 % 10);
  c[6] = (odo / 1000000 % 10);
  c[7] = (odo / 10000000 % 10);

  GD.SaveContext();
  GD.Begin(BITMAPS);
  //23 x 33 font size
  uint16_t y = 17;
  uint16_t x = 157;
  int16_t by = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    GD.BitmapHandle(7);

    if (i != 0)
    {
      int16_t ay = 0;
      int16_t ax = 0;

      char b = 0;

      b = (c[i] < 0 ? 9 : c[i]);
      b = (c[i] > 9 ? 0 : c[i]);
      GD.Cell(48 + b);
      ax = (x - (23 * (i - 1)));

      if (i == 1)
      {
        ay = y + (33 * c[0] / 10);
        GD.Vertex2f(ax * 16, ay * 16);

        b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
        GD.Cell(b + 48);
        ay = (ay - 33);
        GD.Vertex2f(ax * 16, ay * 16);

      }
      else
      {

        switch (i)

        {

        case 2:
          if (c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 3:
          if (c[2] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 4:
          if (c[3] == 9 && c[2] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 5:
          if (c[4] == 9 && c[3] == 9 && c[2] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 6:
          if (c[5] == 9 && c[4] == 9 && c[3] == 9 && c[2] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 7:
          if (c[6] == 9 && c[5] == 9 && c[4] == 9 && c[3] == 9 && c[2] == 9 && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;

        case 8:
          if (c[7] == 9 && c[6] == 9 && c[5] == 9 && c[4] == 9 && c[3] == 9 && c[2] == 9
              && c[1] == 9)
          {
            ay = y + (33 * c[0] / 10);

            GD.Vertex2f(ax * 16, ay * 16);

            b = (c[i] + 1 > 9 ? 0 : c[i] + 1);
            GD.Cell(b + 48);
            ay = (ay - 33);
            GD.Vertex2f(ax * 16, ay * 16);
          }
          else
          {
            ay = y;
          }
          break;
        }

        GD.Vertex2f(ax * 16, ay * 16);

      }

    }

  }

  GD.ColorRGB(0, 0, 0);
  GD.Begin(RECTS);
  GD.Vertex2f(3000, 200);
  GD.Vertex2f(0, 0);
  //GD.End();

  //GD.Begin(RECTS);
  GD.Vertex2f(2960, 1360);
  GD.Vertex2f(0, 752);
  //GD.End();

  GD.RestoreContext();
  GD.End();

}

void rpmFILLER(int16_t angle)
{

  int16_t x = center.x - GD.rsin(PIXELS(350), angle);
  int16_t y = center.y + GD.rcos(PIXELS(350), angle);

  GD.SaveContext();
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(DECR, INCR);
  GD.Begin(POINTS);
  GD.PointSize(5168);
  GD.Vertex2f(center.x, center.y);

  GD.StencilOp(ZERO, DECR);
  GD.Begin(POINTS);
  GD.PointSize(4640);
  GD.Vertex2f(center.x, center.y);

  GD.StencilOp(ZERO, INCR);

  if (angle >= 0 && angle <= 16384)
  {
    //6-9
    GD.Begin(EDGE_STRIP_B);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(x, y);

  }

  else if (angle >= 16384 && angle <= 32768)
  {
    //6-9
    GD.Begin(EDGE_STRIP_B);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(824, 5640);

    //9-12
    GD.Begin(EDGE_STRIP_L);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(x + 16, y);

  }
  else
  {
    //6-9
    GD.Begin(EDGE_STRIP_B);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(824, 5640);

    //9-12
    GD.Begin(EDGE_STRIP_L);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(6400, 338);

    //12-4:30ish

    GD.Begin(EDGE_STRIP_A);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(x, y);

  }

  GD.StencilFunc(GEQUAL, 2, 255);
  GD.ColorMask(1, 1, 1, 1);
  GD.ColorRGB(255, 0, 0);
  GD.ColorA(255);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, 0, 0);

  GD.Clear(0, 1, 0);
  GD.RestoreContext();
}
void speedFILLER(int16_t angle)
{
///////////////////////////////////////////////
  int16_t x = center.x - GD.rsin(PIXELS(350), angle);
  int16_t y = center.y + GD.rcos(PIXELS(350), angle);

  GD.SaveContext();
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(DECR, INCR);
  GD.Begin(POINTS);
  GD.PointSize(4032);
  GD.Vertex2f(center.x, center.y);

  GD.StencilOp(ZERO, DECR);
  GD.Begin(POINTS);
  GD.PointSize(3648);
  GD.Vertex2f(center.x, center.y);

  GD.StencilOp(ZERO, INCR);

  if (angle >= 0 && angle <= 16384)
  {
    //6-9
    GD.Begin(EDGE_STRIP_B);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(x, y);

  }

  else if (angle >= 16384 && angle <= 32768)
  {
    //6-9
    GD.Begin(EDGE_STRIP_B);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(1824, 5640);

    //9-12
    GD.Begin(EDGE_STRIP_L);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(x + 16, y);

  }
  else
  {
    //6-9
    GD.Begin(EDGE_STRIP_B);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(1000, 5640);

    //9-12
    GD.Begin(EDGE_STRIP_L);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(6400, 338);

    //12-4:30ish

    GD.Begin(EDGE_STRIP_A);
    GD.Vertex2f(center.x, center.y);
    GD.Vertex2f(x, y);

  }

  GD.StencilFunc(EQUAL, 2, 255);
  GD.ColorMask(1, 1, 1, 1);
  GD.ColorRGB(255, 0, 0);
  GD.ColorA(255);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, 0, 0);
////6-9
//  GD.Begin(EDGE_STRIP_B);
//  GD.Vertex2f(6400, 5680);
//  GD.Vertex2f(1824, 5666);
//  GD.End();
//
////9-12
//  GD.Begin(EDGE_STRIP_L);
//  GD.Vertex2f(6400, 5680);
//  GD.Vertex2f(6448, 338);
//  GD.End();
//
////12-4:30ish
//
//  GD.Begin(EDGE_STRIP_A);
//  GD.Vertex2f(6400, 5680);
//  GD.Vertex2f(11968, 9058);
//  GD.End();

//  GD.ColorRGB(255, 255, 255);
//  GD.Begin(BITMAPS);
//  GD.BlendFunc(SRC_ALPHA, ONE);
//  GD.Vertex2f(0, 0);
//  GD.End();
  GD.RestoreContext();
  GD.Clear(0, 1, 0);
}

void buildUI()
{
  uint16_t angle = 0;
//uint16_t anglex = 0;
  xy p;

  uint16_t x;
  uint16_t y;

  if (tilt_angle >= 38)
  {
    tilt_angle = 0;
  }

  if (watertemp >= 110)
  {
    watertemp = 40;
  }

  if (volt >= 160)
  {
    volt = 80;
  }

  GD.Clear();
//////////////////////////////////////////////////////////////////////////////////
  GD.BitmapHandle(3);
  GD.cmd_setbitmap(0xd2aa0UL, L4, 50, 50);
  GD.BitmapSize(BILINEAR, BORDER, BORDER, 50, 50);
//////////////////////////////////////////////////////////////////////////////////
  //temp code to animate red ball rotation
  uint16_t rx = ((80 * cos(6.283 / 360 * ai)) + 270) * 16;
  uint16_t ry = ((80 * sin(6.283 / 360 * ai)) + 215) * 16;

  GD.SaveContext();
////// tRed G spot ball
  GD.ColorRGB(255, 0, 0);
  GD.cmd_loadidentity();
  GD.cmd_scale(294912, 294912);
  GD.cmd_setmatrix();
  GD.Begin(BITMAPS);
  GD.BitmapHandle(4);
  GD.BitmapSize(BILINEAR, BORDER, BORDER, 300, 300);
  GD.Vertex2f(rx, ry);

///////
///////////////////////////////////////////////////

  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(KEEP, DECR);
  GD.Begin(POINTS);
  GD.PointSize(2376);
  GD.Vertex2f(6400, 5646);

  //Black Background with globe cutout
  GD.ColorMask(1, 1, 1, 1);
  GD.Begin(RECTS);
  GD.StencilFunc(EQUAL, 0, 255);
  GD.ColorRGB(0, 0, 0);
  GD.Vertex2f(0, 0);
  GD.Vertex2f(12800, 7680);

  GD.Clear(0, 1, 0);
  GD.ColorRGB(0, 0, 0);
  GD.Begin(RECTS);
  GD.Vertex2f(4021, 7482);
  GD.Vertex2f(9544, 7814);
  GD.End();

  //GD.cmd_loadidentity();
  //GD.cmd_setmatrix();

  GD.RestoreContext();

//  /////////////////////////////////////////////

  dispTRIP();
  dispODO();

  //chrome circle////////////////////////////////////////////////////////
  GD.SaveContext();
  GD.Clear(0, 1, 0);
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.Begin(POINTS);
  GD.PointSize(5472);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, DECR);
  GD.Begin(POINTS);
  GD.PointSize(5168);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.PointSize(4128);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, DECR);
  GD.PointSize(4032);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.PointSize(3520);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.ColorMask(1, 1, 1, 1);
  GD.StencilFunc(EQUAL, 1, 255);
  //bitmaps.chrome_edges.draw(400, 240, 0);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, 0, 0);
  GD.End();
  GD.RestoreContext();

  //////////////////////////////////////////////////////////////

  //chrometop////////////////////////////////////////////////////////
  GD.SaveContext();
  GD.Clear(0, 1, 0);
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.Begin(POINTS);
  GD.PointSize(5472);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.Begin(RECTS);
  GD.Vertex2f(0, 1360);
  GD.Vertex2f(12800, 7680);

  GD.ColorMask(1, 1, 1, 1);
  GD.StencilFunc(EQUAL, 0, 255);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, 0, 0);
  GD.End();
  GD.RestoreContext();
  //////////////////////////////////////////////////////////////

  //chrome mid left////////////////////////////////////////////////////////
  GD.SaveContext();
  GD.Clear(0, 1, 0);
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(ZERO, DECR);
  GD.Begin(POINTS);
  GD.PointSize(5472);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.Begin(RECTS);
  GD.Vertex2f(0, 1360);
  GD.Vertex2f(2400, 2720);

  GD.ColorMask(1, 1, 1, 1);
  GD.StencilFunc(EQUAL, 1, 255);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, 0, 0);
  GD.End();
  GD.RestoreContext();
  //////////////////////////////////////////////////////////////

  //chrome mid right////////////////////////////////////////////////////////
  GD.SaveContext();
  GD.Clear(0, 1, 0);
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(ZERO, DECR);
  GD.Begin(POINTS);
  GD.PointSize(5472);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.Begin(RECTS);
  GD.Vertex2f(10400, 1360);
  GD.Vertex2f(12800, 2720);

  GD.ColorMask(1, 1, 1, 1);
  GD.StencilFunc(EQUAL, 1, 255);
  GD.Begin(BITMAPS);
  GD.Vertex2ii(0, 0, 0, 0);
  GD.End();
  GD.RestoreContext();
  //////////////////////////////////////////////////////////////

  //speed rom bars
  GD.SaveContext();
  GD.Clear(0, 1, 0);
  GD.ColorMask(0, 0, 0, 0);

  GD.StencilOp(ZERO, INCR);
  GD.Begin(POINTS);
  GD.PointSize(5168);
  GD.Vertex2ii(400, 352, 0, 0);

  GD.ColorMask(1, 1, 1, 1);
  GD.StencilFunc(EQUAL, 1, 255);
  GD.Begin(BITMAPS);
  GD.ColorA(50);
  GD.Vertex2ii(0, 0, 0, 0);

  GD.RestoreContext();




  GD.Clear(0, 1, 0);
  bitmaps.globe.draw(400, 338, 0);

  gearDISPLAY();

  GD.ColorRGB(255, 255, 255);
  GD.cmd_text(400, 380, 8, OPT_CENTER | OPT_RIGHTX, astext(SPEED));
  GD.cmd_text(400, 450, 7, OPT_CENTER | OPT_RIGHTX, astext(RPM));
  GD.cmd_text(400, 312, 7, OPT_CENTER | OPT_RIGHTX, "12:00");
  GD.cmd_text(168, 57, 6, OPT_CENTER | OPT_RIGHTX, "ODO");
  GD.cmd_text(628, 57, 6, OPT_CENTER | OPT_RIGHTX, "TRIP");

//speed gauge elements

  GD.ColorRGB(255, 255, 255);
  GD.LineWidth(10);
  for (uint8_t i = 0; i <= speedSTEPS; i++)
  {
    angle = map(i, 0, speedSTEPS, DEGREES(60), DEGREES(360 - 60));
    polar(p, PIXELS(213), angle);

    rotateTEXT(astext(i * 5), 6, p.x, p.y, angle - DEGREES(180), 12, 17, 90, 90, 100);

    if (i != 0 && i != speedSTEPS)
    {
      polar_line(PIXELS(235), PIXELS(245), angle);
    }
  }

  GD.End();

//RPM Gauge elements
  for (uint8_t i = 0; i <= rpmSTEPS; i++)
  {
    angle = map(i, 0, rpmSTEPS, DEGREES(68), DEGREES(360 - 68));

    polar(p, PIXELS(275), angle);

    rotateTEXT(astext(i), 7, p.x, p.y, angle - DEGREES(180), 23, 33, 80, 100, 100);

    //if (i != 0 && i != rpmSTEPS)
    //   {
    polar_line(PIXELS(295), PIXELS(310), angle);
    //  }
  }
  GD.End();

/////////////////////////////////////////////////////////////////////////////////
//SpeedBar

  float delta = ((SPEED) - oldDELTASPEED) / 10 + oldDELTASPEED;
  oldDELTASPEED = delta;

  angle = map(delta * 10, 0, speedRANGE * 10, DEGREES(60), DEGREES(360 - 60));
  GD.ColorRGB(255, 0, 0);
  GD.LineWidth(60);
  polar_line(PIXELS(210), PIXELS(250), angle);
//  GD.ColorRGB(255, 255, 255);
//  GD.LineWidth(10);
//  polar_line(PIXELS(210), PIXELS(250), angle);
//  GD.End();
  speedFILLER(angle);

//RPmBAR
  delta = ((RPM) - oldDELTARPM) / 10 + oldDELTARPM;
  oldDELTARPM = delta;
  angle = map(delta, 0, rpmRANGE, DEGREES(68), DEGREES(360 - 68));
  GD.ColorRGB(255, 0, 0);
  GD.LineWidth(60);
  polar_line(PIXELS(265), PIXELS(318), angle);
  //GD.ColorRGB(255, 255, 255);
  //GD.LineWidth(20);
  //polar_line(PIXELS(265), PIXELS(315), angle);
  GD.End();
  rpmFILLER(angle);

  int mapping = 0;
////////VOLT Gauge /////////
// 47' = 8v
// 113' = 16v

  mapping = map(volt, 80, 160, 47, 113);

  x = center.x - GD.rsin(PIXELS(161), DEGREES(mapping));
  y = center.y + GD.rcos(PIXELS(161), DEGREES(mapping));

  if (volt > 120)
  {
    GD.ColorRGB(map(volt, 120, 150, 0, 255), map(volt, 120, 150, 255, 0), 0);
    if (volt >= 150)
    {
      GD.ColorRGB(255, 0, 0);
    }

  }
  if (volt <= 120)
  {
    GD.ColorRGB(map(volt, 120, 110, 0, 255), map(volt, 120, 110, 255, 0), 0);
    if (volt < 110)
    {
      GD.ColorRGB(255, 0, 0);
    }
  }

  GD.Begin(LINES);
  GD.LineWidth(70);
  GD.Vertex2f(x, y);
  GD.Vertex2f(x - 448, y);
  GD.End();

////////////////////////////

////////Water Temp Gauge /////////
// 247' = 40
// 313' = 120
  mapping = map(watertemp, 40, 110, 247, 313);

  if (watertemp >= 80)
  {
    GD.ColorRGB(map(watertemp, 80, 120, 0, 255), map(watertemp, 80, 120, 255, 0), 0);
  }
  if (watertemp <= 80)
  {
    GD.ColorRGB(map(watertemp, 80, 40, 0, 255), map(watertemp, 80, 40, 255, 0), 0);
  }

  x = center.x - GD.rsin(PIXELS(161), DEGREES(mapping));
  y = center.y + GD.rcos(PIXELS(161), DEGREES(mapping));

  GD.Begin(LINES);
  GD.LineWidth(70);
  GD.Vertex2f(x, y);
  GD.Vertex2f(x + 448, y);
  GD.End();

////////////////////////////
  haz_ind();
  light_glow();

  left_ind();
  right_ind();
  oil_ind();
  fuel_ind();
  high_beam();
  temp_ind();
  water_temp();
  batt_ind();
////////tilt Gauge /////////
// 47' = 8v
// 247' = 16v

  x = center.x - GD.rsin(PIXELS(352), DEGREES(90 - 19 + tilt_angle));
  y = center.y + GD.rcos(PIXELS(352), DEGREES(90 - 19 + tilt_angle));
  GD.ColorRGB(255, 0, 0);
  GD.Begin(LINES);
  GD.LineWidth(70);
  GD.Vertex2f(x, y);
  GD.Vertex2f(x - 450, y);
  GD.ColorRGB(255, 255, 255);
  GD.LineWidth(15);
  GD.Vertex2f(x, y);
  GD.Vertex2f(x - 450, y);
  GD.End();

  x = center.x - GD.rsin(PIXELS(352), DEGREES(270 - 19 + tilt_angle));
  y = center.y + GD.rcos(PIXELS(352), DEGREES(270 - 19 + tilt_angle));
  GD.ColorRGB(255, 0, 0);
  GD.Begin(LINES);
  GD.LineWidth(70);
  GD.Vertex2f(x, y);
  GD.Vertex2f(x + 450, y);
  GD.ColorRGB(255, 255, 255);
  GD.LineWidth(15);
  GD.Vertex2f(x, y);
  GD.Vertex2f(x + 450, y);
  GD.End();

////////////////////////////

////Speedbar filler
//    GD.ColorRGB(255, 0, 0);
//    GD.LineWidth(20);
//  for (int i = 0; i < SPEED; i++) {
//    angle = map(i, 0, speedRANGE, 10922, 54613);
//    polar_line(PIXELS(232), PIXELS(250), angle);
//  }
//  GD.End();
//  //RPM bar filler
//  /*For some reason adding this extra filler code causes the GPU to crash...
//    Before when i was just using the vectors and GD vertex commands i was able
//    to run all these elements without trouble... Is this a bug?        */
//  for (int i = 0; i < RPM/100; i++) {
//    angle = map(i, 0, rpmRANGE/100, DEGREES(68), DEGREES(360 - 68));
//    polar_line(PIXELS(280), PIXELS(310), angle);
//
//  }
//  GD.End();

  comp_disp();

  GD.swap();

}
//void checkSERIAL()
//{
//  char com[10];
//
//  char val[5];
//
//  int c = 0;
//  int d = 0;
//  bool b = false;
//  while (Serial.available() > 0)
//  {
//
//    char o = Serial.read();
//
//    Serial.print(o);
//
//    if (o == '=')
//    {
//      b = true;
//      o = Serial.read();  // moves o to the next character, should be the values
//      Serial.print(o);
//
//      c = 0;
//    }
//
//    if (b == false)
//    {
//      com[c] = o;
//      com[c + 1] = '\0';
//    }
//
//    if (b == true)
//    {
//      val[c] = o;
//      val[c + 1] = '\0';
//    }
//    c++;
//
//    if (Serial.peek() == -1)
//    {
//
//      Serial.print("Command is : ");
//      Serial.println(com);
//      Serial.print("Value is : ");
//      Serial.println(val);
//
//      if (strcmp(com, "gear") == 0)
//
//      {
//        Serial.print("Setting gear to ");
//        Serial.println(val);
//        GEAR = atoi(val);
//      }
//
//      else if (strcmp(com, "left") == 0)
//      {
//        if (LEFT == false)
//        {
//          LEFT = true;
//          Serial.print("Left indicator Active");
//        }
//        else
//        {
//          LEFT = false;
//          Serial.print("Left indicator Deactivated");
//        }
//
//      }
//
//      else if (strcmp(com, "right") == 0)
//      {
//        if (RIGHT == false)
//        {
//          RIGHT = true;
//          Serial.print("Right indicator Active");
//        }
//        else
//        {
//          RIGHT = false;
//          Serial.print("Right indicator Deactivated");
//        }
//      }
//
//      else if (strcmp(com, "speed") == 0)
//      {
//        Serial.print("Setting speed to ");
//        Serial.println(val);
//
//        SPEED = atoi(val);
//      }
//
//      else if (strcmp(com, "rpm") == 0)
//      {
//        Serial.print("Setting RPM to ");
//        Serial.println(val);
//        RPM = atoi(val);
//      }
//      else if (strcmp(com, "odo") == 0)
//      {
//        Serial.print("Setting ODO to ");
//        Serial.println(val);
//
//        ODO = atof(val);
//      }
//      else if (strcmp(com, "trip") == 0)
//      {
//        Serial.print("Setting TRIP to ");
//        Serial.println(val);
//
//        TRIP = atof(val);
//      }
//      else if (strcmp(com, "demo") == 0)
//      {
//        demo = !demo;
//        if (demo == true)
//        {
//          Serial.print("Demo Mode On");
//        }
//        else
//        {
//          Serial.print("Demo Mode Off");
//          SPEED = 0;
//          RPM = 0;
//        }
//      }
//
//      else if (strcmp(com, "hb") == 0)
//      {
//        if (highBEAM == false)
//        {
//          highBEAM = true;
//          Serial.print("High Beam Active");
//        }
//        else
//        {
//          highBEAM = false;
//          Serial.print("High Beam Deactivated");
//        }
//      }
//
//      else if (strcmp(com, "haz") == 0)
//      {
//        if (HAZ == false)
//        {
//          HAZ = true;
//          Serial.print("Hazard indicator Active");
//        }
//        else
//        {
//          HAZ = false;
//          Serial.print("Hazard indicator Deactivated");
//        }
//
//      }
//      else if (strcmp(com, "fuel") == 0)
//      {
//        if (FUEL == false)
//        {
//          FUEL = true;
//          Serial.print("Fuel Warning Active");
//        }
//        else
//        {
//          FUEL = false;
//          Serial.print("Fuel Warning Deactivated");
//        }
//
//      }
//
//      else
//      {
//        Serial.println("Command not recognised!");
//      }
//    }
//  }
//  while (Serial.available() > 0)  //flushing anything left...
//  {
//    Serial.read();
//  }
//
//}
void loop()
{

//  checkSERIAL();
// accelUPDATE();

//    if (SPEED > speedRANGE)
//  {
//  SPEED = 0;
//  }
//
//    if (RPM >= rpmRANGE)
//  {
//  RPM = 0;
//  }

  if (NORTH >= 360)
  {
    NORTH = 0;
  }

  buildUI();

  if (demo == true)
  {

    RPM = random((((RPM - 500) > 0 ? (RPM - 500) : RPM)), (RPM + 500));
    RPM = (RPM < 0) ? 0 : RPM;
    RPM = (RPM > rpmRANGE) ? rpmRANGE : RPM;

    SPEED = random(((SPEED - 10 > 0 ? SPEED - 10 : SPEED)), (SPEED + 10));
    SPEED = (SPEED < 0) ? 0 : SPEED;
    SPEED = (SPEED > speedRANGE) ? speedRANGE : SPEED;

    ai++;
    NORTH++;
    volt++;
    watertemp++;
    //SPEED++;
    //RPM += 50;
    tilt_angle++;
    TRIP += 0.01;
    ODO += 0.1;
  }
  /*  else
   {
   SPEED = 0;
   RPM = 0;
   }*/
}
