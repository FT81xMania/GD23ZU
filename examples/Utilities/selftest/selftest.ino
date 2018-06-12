#include <GD23ZU.h>

#define UART_SPEED 9600

#include "selftest_assets.h"

#define SCREEN_ADDR 0x30000UL

byte x, y;

static void log(const char*s)
{
  while (*s) {
    char c = *s++;
    if (c == '\n') {
      x = 0;
      y++;
    } else {
      uint32_t dst = SCREEN_ADDR + (((x + (y * 48)) << 1));
      GD.wr16(dst, 0x0f00 | c);
      x++;
    }
  };
}

void setup()
{
  delay(1000);Serial.begin(UART_SPEED);
  Serial.println("---- FT800/810 SELFTEST ----");
  EEPROM.format();
}

static void ramp(int y, uint32_t color)
{
  GD.ScissorSize(400, 8);
  GD.ScissorXY(40, y);
  GD.cmd_gradient(40, 0, 0x000000, 440, 0, color);
}

void testcard(int pass, const char *message)
{
  // GD.ClearColorRGB(0x204060);

  GD.get_inputs();
  GD.Clear();
  if (GD.model == 800) GD.cmd_text(GD.w / 2, 12, 28, OPT_CENTER,"FT800 Self test");
  if (GD.model != 800) GD.cmd_text(GD.w / 2, 12, 28, OPT_CENTER,"FT810 Self test");
  GD.cmd_text(GD.w - 2, 12, 27, OPT_CENTERY | OPT_RIGHTX, GD2_VERSION);
  int y;

  y = PIXELS(50);
  GD.Begin(POINTS);
  for (int i = 0; i < 6; i++) {
    byte l = 4 << i;
    int x = map(1 + i, 0, 7, 0, PIXELS(GD.w));
    GD.PointSize(280);
    GD.ColorRGB(0xffffff);
    GD.Vertex2f(x, y);

    GD.PointSize(240);
    GD.ColorRGB(l, l, l);
    GD.Vertex2f(x, y);
  }
  y += 30;

  ramp(y, 0xff0000);  y += 12;
  ramp(y, 0x00ff00);  y += 12;
  ramp(y, 0x0000ff);  y += 12;
  ramp(y, 0xffffff);  y += 12;
  GD.RestoreContext();

  // GD.Begin(BITMAPS);
  // GD.Vertex2ii(0, 272 - (8 * 16), 1, 0);
  // GD.Vertex2ii(480 - LENA_WIDTH, 272 - LENA_WIDTH, 0, 0);

  if (pass == -1)
    GD.ColorRGB(0x808000);
  else
    GD.ColorRGB(pass ? 0x40ff40 : 0xff4040);
  GD.cmd_text(GD.w / 2, 180, 31, OPT_CENTER, message);

  GD.ColorRGB(0xffffff);
  GD.Begin(LINES);

  GD.Vertex2f(PIXELS(GD.inputs.x), PIXELS(0));
  GD.Vertex2f(PIXELS(GD.inputs.x), PIXELS(GD.h));

  GD.Vertex2f(PIXELS(0),    PIXELS(GD.inputs.y));
  GD.Vertex2f(PIXELS(GD.w), PIXELS(GD.inputs.y));

  GD.swap();
  GD.finish();
}

#define SCREENTEST(NAME) \
  do { \
  Serial.println(#NAME); \
  testcard(-1, #NAME); \
  r = test_##NAME(); \
  const char* msg = r ? (#NAME ": pass") : (#NAME ": FAIL"); \
  Serial.println(msg); \
  testcard(r, msg); \
  while (!r) ; \
  } while (0)

int test_ident()
{
  byte id = GD.rd(REG_ID);
  if (id != 0x7c) {
    Serial.println(id, HEX);
    return 0;
  }
  return 1;
}

int test_clock()
{
  int SPEEDUP = 8;

  GD.rd32(REG_CLOCK); // warm-up
  delay(10);
  long t1 = GD.rd32(REG_CLOCK);
  delay(1000 / SPEEDUP);
  long t2 = GD.rd32(REG_CLOCK);

  float measured = float(t2 - t1);
  // measured should be 48e6, within 2%

  //float expected = (GD3 ? 60e6 : 48e6) / SPEEDUP;
  float expected = 60e6 / SPEEDUP;
  if (GD.model == 800) 48e6 / SPEEDUP;
  Serial.println(measured, DEC);
  Serial.println(expected, DEC);
  float diff = measured - expected;
  float percent = fabs(100 * (diff / expected));
  return percent < 5.0;
}

int test_tune()
{
  GD.tune();
  return 1;
}

static byte test_RAM(void)
{
  uint32_t a;
  for (a = 0; a < 0x40000U; a += 947)
    GD.wr(a, a);
  for (a = 0; a < 0x40000U; a += 947)
    if (GD.rd(a) != (a & 0xff))
      return 0;
  return 1;
}

static byte test_PWM(void)
{
  for (int i = 128; i >= 0; i--) {
    GD.wr(REG_PWM_DUTY, i);
    delay(2);
  }
  GD.wr(REG_PWM_DUTY, 128);
  return 1;
}

static byte test_storage(void)
{
  GD.storage();
  return test_ident();
}

static byte test_SDcard(void)
{
  for (byte i = 0; i < 2; i++) {
    GD.safeload("selftest.gd2");
    uint32_t pcrc = GD.cmd_memcrc(0, ASSETS_END);
    GD.finish();
    uint32_t crc = GD.rd32(pcrc);
    if (crc != KITTEN_CRC)
      return 0;
  }
  return 1;
}

#if 0
static void collect(int &rx, int &ry, int &rz)
{
  uint16_t ax = 0, ay = 0, az = 0;
  for (byte i = 32; i; i--) {
    int x = 0, y = 0, z = 0;
#ifdef A2
    x = analogRead(A2);
#endif
#ifdef A1
    y = analogRead(A1);
#endif
#ifdef A0
    z = analogRead(A0);
#endif
    ax += x;
    ay += y;
    az += z;
  }
  rx = ax >> 5;
  ry = ay >> 5;
  rz = az >> 5;
}

static byte test_accel2(void)
{
  while (1) {
    GD.finish();
    if ((millis() % 2000) < 1000)
      GD.wr(REG_GPIO, 0x80);
    else
      GD.wr(REG_GPIO, 0x81);
    int x, y, z;
    collect(x, y, z);
    GD.Clear();
    GD.cmd_number(0,  40, 26, 3, x);
    GD.cmd_slider(50,  40, 400, 10, 0, x, 512);

    GD.cmd_number(0,   70, 26, 3, y);
    GD.cmd_slider(50,  70, 400, 10, 0, y, 512);

    GD.cmd_number(0,  100, 26, 3, GD.rd(REG_GPIO));
    GD.cmd_slider(50, 100, 400, 10, 0, z, 512);
    GD.swap();
  }
  return 1;
}

static byte test_accel(void)
{
  int x0, y0, z0;
  int x1, y1, z1;

  GD.wr(REG_GPIO, 0x80);
  collect(x0, y0, z0);
  delay(100);
  GD.wr(REG_GPIO, 0x81);
  delay(100);
  collect(x1, y1, z1);

  Serial.print(x0); Serial.print(" "); Serial.print(y0); Serial.print(" "); Serial.println(z0);
  Serial.print(x1); Serial.print(" "); Serial.print(y1); Serial.print(" "); Serial.println(z1);

  // if ((x0 > x1) || (y0 > y1) || (z0 > z1)) return 0;

  int d;
  d = abs(x0 - x1);
  if ((d < 30) || (120 < d))
    return 0;

  d = abs(y0 - y1);
  if ((d < 30) || (120 < d))
    return 0;

  d = abs(z0 - z1);
  if ((d < 50) || (200 < d))
    return 0;

  z0 %= 37;
  while (z0--)
    GD.random();

  return 1;
}
#endif

static void play(uint16_t n)
{
  GD.wr16(REG_SOUND, n);
  GD.wr(REG_PLAY, 1);
}

static void play_wait(uint16_t n)
{
  play(n);
  while (GD.rd(REG_PLAY))
    ;
}

static byte test_touch(void)
{
#if 0
  if (!GD3) {
    GD.self_calibrate();
    // write the new calibration back to EEPROM
    uint16 Status;
    uint16_t Data;

    for (int i = 0; i < 24; i++) { 
      Data = GD.rd(REG_TOUCH_TRANSFORM_A + i);
//Serial.print("Data.rd[" + String(i) + "]=");
//Serial.println(Data, HEX);
      Status = EEPROM.write(0x20 + i, Data);
//Serial.println("Status=" + String(Status));
      if (Status != 0) GD.alert("calibrate data save error");
    }
    Status = EEPROM.write(0x10, 0x7c);  // is written!
//Serial.println("Status=" + String(Status));
    if (Status != 0) GD.alert("calibrate data save error");
  }
#endif 

  byte hit = 0;
  while (hit != 0x0f) {
    GD.finish();
    byte tag = GD.rd(REG_TOUCH_TAG);
    if ((1 <= tag) && (tag <= 4)) {
      play(0x50);
      hit |= (1 << (tag - 1));
    }
    if (tag == 77)
      return 0;
    GD.ClearTag(77);
    GD.Clear();
    GD.PointSize(20 * 16);
    GD.Begin(POINTS);
    for (byte i = 1; i <= 4; i++) {
      if (hit & (1 << (i - 1))) {
        GD.ColorRGB(0x00ff00);
        GD.Tag(0xff);
      } else {
        GD.ColorRGB(0x808080);
        GD.Tag(i);
      }
      switch (i) {
        case 1: GD.Vertex2ii(20,  20, 0, 0);  break;
        case 2: GD.Vertex2ii(460, 20, 0, 0);  break;
        case 3: GD.Vertex2ii(20,  250, 0, 0);  break;
        case 4: GD.Vertex2ii(460, 250, 0, 0);  break;
      }
    }
    GD.random();  // scramble PRN state for later
    GD.swap();
  }
  return 1;
}

static const PROGMEM uint32_t digits[11] = {
  DIGIT_0,
  DIGIT_1,
  DIGIT_2,
  DIGIT_3,
  DIGIT_4,
  DIGIT_5,
  DIGIT_6,
  DIGIT_7,
  DIGIT_8,
  DIGIT_9,
  DIGIT_9 + DIGIT_9_LENGTH
};

static void saydigit(byte n)
{
  GD.wr32(REG_PLAYBACK_FREQ, 8000);
  GD.wr32(REG_PLAYBACK_FORMAT, ADPCM_SAMPLES);

  uint32_t dstart = pgm_read_dword(digits + n);
  uint32_t dend = pgm_read_dword(digits + n + 1);
  GD.wr32(REG_PLAYBACK_START, dstart);
  GD.wr32(REG_PLAYBACK_LENGTH, dend - dstart);

  GD.wr(REG_PLAYBACK_PLAY, 1);
}

static void blank(int n)
{
  for (int i = 0; i < n; i++) {
    GD.get_inputs();
    GD.cmd_gradient(0, 0, 0xb0b0a0, 0, 272, 0x404040);
    GD.swap();
  }
}

static byte getkey()
{
  byte prev_tag;
  do {
    prev_tag = GD.inputs.tag;
    GD.get_inputs();
    if (GD.inputs.x & 1)
      GD.random();
    GD.cmd_gradient(0, 0, 0xb0b0a0, 0, 272, 0x404040);
    for (int i = 0; i < 9; i++) {
      byte digit = i + 1;
      int x = 120 + 80 * (i % 3);
      int y = 20 + 80 * (i / 3);
      GD.Tag(digit);
      char msg[2] = { '0' + digit, 0 };
      GD.cmd_fgcolor((digit == GD.inputs.tag) ? 0xc08000 : 0x003870);
      GD.cmd_button(x, y, 70, 70, 31, 0, msg);
    }
    GD.swap();
  } while (!((GD.inputs.tag == 0) && (1 <= prev_tag) && (prev_tag <= 9)));
  return prev_tag;
}

static byte test_audio(void)
{
  // Stir up the PRN
  for (int i = micros() % 97; i; i--)
    GD.random();
  blank(20);

  for (int i = 0; i < 3; i++) {
    byte d = 1 + GD.random(9);
    saydigit(d);
    blank(12);
    if (getkey() != d)
      return 0;
  }

  return 1;
}

static struct {
  byte t, note;
} pacman[]  = {
  { 0, 71 },
  { 2, 83 },
  { 4, 78 },
  { 6, 75 },
  { 8, 83 },
  { 9, 78 },
  { 12, 75 },
  { 16, 72 },
  { 18, 84 },
  { 20, 79 },
  { 22, 76 },
  { 24, 84 },
  { 25, 79 },
  { 28, 76 },
  { 32, 71 },
  { 34, 83 },
  { 36, 78 },
  { 38, 75 },
  { 40, 83 },
  { 41, 78 },
  { 44, 75 },
  { 48, 75 },
  { 49, 76 },
  { 50, 77 },
  { 52, 77 },
  { 53, 78 },
  { 54, 79 },
  { 56, 79 },
  { 57, 80 },
  { 58, 81 },
  { 60, 83 },
  { 255, 255 }
};

static const uint8_t GD3_43__init[128] = {
255, 255, 1, 1, 26, 255, 255, 255, 12, 32, 48, 0, 4, 0, 0, 0, 0, 135,
147, 3, 26, 255, 255, 255, 80, 33, 48, 0, 24, 0, 0, 0, 46, 46, 46, 46,
46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
46, 46, 46, 34, 255, 255, 255, 88, 32, 48, 0, 120, 156, 99, 100, 96,
96, 216, 198, 200, 192, 0, 68, 12, 204, 12, 16, 26, 132, 217, 128, 24,
0, 17, 207, 0, 197, 0, 0, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255,
255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0,
255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255
};
static const uint8_t GD3_7__init[128] = {
255, 255, 1, 1, 26, 255, 255, 255, 12, 32, 48, 0, 4, 0, 0, 0, 0, 39,
134, 3, 26, 255, 255, 255, 80, 33, 48, 0, 24, 0, 0, 0, 46, 46, 46, 46,
46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
46, 46, 46, 34, 255, 255, 255, 44, 32, 48, 0, 120, 156, 59, 201, 204,
192, 16, 193, 192, 192, 160, 0, 164, 65, 192, 0, 136, 89, 152, 128,
124, 32, 253, 128, 17, 34, 6, 149, 98, 0, 113, 183, 49, 66, 104, 100,
49, 160, 114, 6, 0, 164, 38, 3, 65, 0, 0, 0, 0, 255, 255, 255, 0, 255,
255, 255, 0, 255, 255, 255, 0, 255, 255, 255
};
static const uint8_t GD3_VGA__init[128] = {
255, 255, 1, 2, 26, 255, 255, 255, 12, 32, 48, 0, 4, 0, 0, 0, 64, 210,
223, 3, 26, 255, 255, 255, 148, 32, 48, 0, 4, 0, 0, 0, 16, 0, 0, 0,
34, 255, 255, 255, 44, 32, 48, 0, 120, 156, 115, 96, 101, 96, 208, 96,
100, 96, 96, 96, 97, 0, 131, 14, 32, 86, 99, 102, 96, 80, 6, 113, 152,
33, 98, 108, 12, 248, 1, 72, 59, 0, 77, 136, 1, 81, 0, 0, 0, 255, 255,
255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255,
255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0,
255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255
};
#define FREQUENCY_OFFSET 16
#define CALIBRATION_OFFSET 32

uint8_t gpio, gpio_dir;

void set_SDA(byte n)
{
  GD.wr(REG_GPIO_DIR, gpio_dir | (0x03 - n));    // Drive SCL, SDA low
}

void set_SCL(byte n)
{
  GD.wr(REG_GPIO, gpio | (n << 1));
}

int get_SDA(void)
{
  return GD.rd(REG_GPIO) & 1;
}

void i2c_start(void)
{
  set_SDA(1);
  set_SCL(1);
  set_SDA(0);
  set_SCL(0);
}

void i2c_stop(void)
{
  set_SDA(0);
  set_SCL(1);
  set_SDA(1);
  set_SCL(1);
}

int i2c_rx1()
{
  set_SDA(1);
  set_SCL(1);
  byte r = get_SDA();
  set_SCL(0);
  return r;
}

void i2c_tx1(byte b)
{
  set_SDA(b);
  set_SCL(1);
  set_SCL(0);
}

int i2c_tx(byte x)
{
  for (int i = 7; i >= 0; i--)
    i2c_tx1(1 & (x >> i));
  return i2c_rx1();
}

int i2c_rx(int nak)
{
  byte r = 0;
  for (byte i = 0; i < 8; i++)
    r = (r << 1) | i2c_rx1();
  i2c_tx1(nak);
  return r;
}

void i2c_begin(void)
{
  gpio = GD.rd(REG_GPIO) & ~3;
  gpio_dir = GD.rd(REG_GPIO_DIR) & ~3;

  // 2-wire software reset
  i2c_start();
  i2c_rx(1);
  i2c_start();
  i2c_stop();
}

#define ADDR  0xa0

void ram_write(const uint8_t *v)
{
  for (byte i = 0; i < 128; i += 8) {
    i2c_start();
    i2c_tx(ADDR);
    i2c_tx(i);
    for (byte j = 0; j < 8; j++)
      i2c_tx(*v++);
    i2c_stop();
    delay(6);
  }
}

byte ram_read(byte a)
{
  i2c_start();
  i2c_tx(ADDR);
  i2c_tx(a);

  i2c_start();
  i2c_tx(ADDR | 1);
  byte r = i2c_rx(1);
  i2c_stop();
  return r;
}

void ramdump(void)
{
  for (int i = 0; i < 128; i++) {
    byte v = ram_read(i);
    Serial.print(i, HEX);
    Serial.print(" ");
    Serial.println(v, HEX);
  }
}

void ram_get(byte *v)
{
  i2c_start();
  i2c_tx(ADDR);
  i2c_tx(0);

  i2c_start();
  i2c_tx(ADDR | 1);
  for (int i = 0; i < 128; i++) {
    *v++ = i2c_rx(i == 127);
    // Serial.println(v[-1], DEC);
  }
  i2c_stop();
}

static void load_flash(uint8_t *config)
{
    byte b[128];

    i2c_begin();
    ram_write(config);
    ram_get(b);
    int diff = memcmp(config, b, 128);
    if (diff != 0) {
      Serial.println("Flash fault");
      GD.Clear();
      GD.cmd_text(GD.w / 2, GD.h / 2, 30, OPT_CENTERX, "Flash fault");
      GD.swap();
      for (;;);
    }
    Serial.println("Flash verified OK");
    GD.begin(0);
}

static void setup_flash()
{
Serial.println("setup_flash");
  GD.begin(0);

  GD.self_calibrate();
  GD.finish();

}

void loop()
{
//  setup_flash();
  GD.begin(FORCE_CALIBRATE);

  x = y = 0;
  testcard(1, "Starting tests");
  GD.finish();

  byte r, pass = 1;

  {
    SCREENTEST(ident);
    SCREENTEST(tune);
    if (GD.model != 800) SCREENTEST(clock);
    SCREENTEST(RAM);
    SCREENTEST(PWM);
    SCREENTEST(storage);
    SCREENTEST(SDcard);
    //  SCREENTEST(accel);
    SCREENTEST(touch);
    //  SCREENTEST(audio);
    {
      int i = 0, t = 0;
      for (;;) {
        testcard(1, "* ALL PASS *");
        if (t == 4 * pacman[i].t)
          GD.play(HARP, pacman[i++].note - 12);
        if (++t == 256) {
          t = 0;
          i = 0;
        }
      }
    }
  }

  if (pass) {
    char msg[60];
    log("All tests passed\n");
    long seconds = millis() / 1000;
    int minutes = seconds / 60;
    sprintf(msg, "%d minutes", minutes);
    log(msg);
  } else {
    for (;;)
      ;
  }
  delay(5000);
}

