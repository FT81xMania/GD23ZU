#include <GD23ZU.h>

class scroller {
public:
  signed short dragprev;
  int vel;      // velocity
  long base;    // screen x coordinate, in 1/16ths pixel
  long limit;
  void init(uint32_t lim) {
    dragprev = -32768;
    vel = 0;      // velocity
    base = 0;     // screen x coordinate, in 1/16ths pixel
    limit = lim;
  }
  void run(bool touching, int16_t sx) {
    if (touching & (dragprev != -32768)) {
      vel = (dragprev - sx) << 4;
    } else {
      int change = max(1, abs(vel) >> 5);
      if (vel < 0)
        vel += change;
      if (vel > 0)
        vel -= change;
    }
    dragprev = touching ? sx : -32768;
    base += vel;
    base = max(0, min(base, limit));
  }
  uint16_t getval() {
    return base >> 4;
  }
};

char lines[60][10];
int num_jpgs = 0;
scroller yscroll;

void setup()
{
  for(int i=0;i<50;i++) {
    char wk[8];
    sprintf(wk,"Line# %d",i+1);
    strcpy(lines[i],wk);
    num_jpgs++;
  }
  GD.begin();
}

static void showdir0(char lines[][10], int num_jpgs, int yb, int sel)
{
  GD.Tag(0);
  GD.SaveContext();
  GD.ScissorSize(GD.w, GD.h);
  GD.cmd_gradient(0, 0, 0x3030c0, 0, GD.h, 0x000000);
  GD.RestoreContext();

  GD.LineWidth(48);
  GD.Begin(RECTS);
  GD.ColorRGB(0xffffff);
  for (int j = 0; j < num_jpgs; j++) {
    int y = 40 * j - yb;
    if ((-50 < y) && (y < GD.h)) {
      GD.ColorA((sel == j) ? 0xff : 0xa0);
      GD.Vertex2f(16 * (GD.w/2 - 100), 16 * (y + 6));
      GD.Vertex2f(16 * (GD.w/2 + 100), 16 * (y + 40 - 6));
    }
  }
}

static void showdir1(char lines[][10], int num_jpgs, int yb, int sel)
{
  GD.ColorA(0xff);
  GD.ColorRGB(0x000000);
  for (int j = 0; j < num_jpgs; j++) {
    int y = 40 * j - yb;
    if ((-50 < y) && (y < GD.h)) {
      GD.Tag(128 + j);
      GD.cmd_text(GD.w/2, y + 18, 28, OPT_CENTER, lines[j]);
    }
  }
}

//#define displayLines 7
//#define displayLines 12

void loop()
{
  static int picked = -1;
  int displayLines = 7;
  if (GD.model != 800) displayLines = 12;
//  yscroll.init((40L * (num_jpgs - 4)) << 4);
  if (picked == -1) yscroll.init((40L * (num_jpgs - displayLines)) << 4);

  do {
    GD.get_inputs();
    byte touching = (GD.inputs.x != -32768) && (GD.inputs.tag < 128);
    yscroll.run(touching, GD.inputs.y);

    showdir0(lines, num_jpgs, yscroll.base >> 4, picked);
    showdir1(lines, num_jpgs, yscroll.base >> 4, picked);
    GD.swap();
  } while (GD.inputs.tag < 128);
  picked = GD.inputs.tag - 128;

  Serial.println("picked=" + String(picked));


}

