/*
 * FT81X FTDI screens Library for Teensy 3.5/6 and STM32F1x, F4 and F7
 * based on STM32_GD2 by nopnop2002
 * https://github.com/nopnop2002/STM32_GD2
 *
 * FT800/81X TFT Library for STM32F103
 * based on GD23STM32_F103x by lightcalamar
 * https://github.com/lightcalamar/GD23STM32_F103x
 *
 * GD23STM32_F103x has been developed based on Gameduino 2 library.
 * Copyright (C) 2013-2016 by James Bowman <jamesb@excamera.com>
 * Gameduino 2 library for Arduino, Arduino Due, Raspberry Pi.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * GD23ZU Library for STM32Fx and Teensy 3.5/6
 *
 * MexSpa Team contribution for https://ft81xmania.com
 * June 7, 2018
*/

#ifndef _GD23ZU_H_INCLUDED
#define _GD23ZU_H_INCLUDED

#include "Arduino.h"
#include "SdFat.h"

// Define GPIO

#define POR_PIN     24  // POWER ON RESET SCREEN HOTMCU ONLY PWM pin is corect !!!       
                        // by lightcalamar
#define TFT_CS      10  // 
#define SD_PIN       5  // 


#if defined(ARDUINO_ARCH_STM32)
#define POR_PIN       PB1   // POWER ON RESET SCREEN HOTMCU   Pin PB1 para (FOR) STM32F103C8T6   
                            // ONLY PWM pin is corect !!!    by lightcalamar
#define TFT_CS        PA4   // Chip Select for TFT

#define STM32_CPU     767    //767--Nucleo F767ZI    429--F429I_DISCO

  #if(STM32_CPU == 429) 
     #define SD_PIN        PA15  //PA15   SPI3 on F429 and Core7XXI
  #endif

  #if(STM32_CPU == 767) 
     #define SD_PIN        PB11  //PB11 SPI3-F767, PA11 SPI2-F767
  #endif
#endif


// Define Rotation

#define ORIENTACION     1  // 0, 1, 2, 3, FT81X 
#define PROTO           0  // 0 FT80x alternatives, 1 gameduino2
#define ROTACION        0  // 0,1         FT80x

#define SizeFT813       5   //5-5", 43-4.3", 35-3.5"


#define RGB(r, g, b)    ((uint32_t)((((r) & 0xffL) << 16) | (((g) & 0xffL) << 8) | ((b) & 0xffL)))
#define F8(x)           (int((x) * 256L))
#define F16(x)          ((int32_t)((x) * 65536L))

// Define begin options

#define GD_NONE         0
#define GD_CALIBRATE    1
#define GD_TRIM         2
#define GD_STORAGE      4
#define FORCE_CALIBRATE 8


////////////////////////////////////////////////////////////////////////

class xy {
public:
  int x, y;
  void set(int _x, int _y);
  void rmove(int distance, int angle);
  int angleto(class xy &other);
  void draw(byte offset = 0);
  void rotate(int angle);
  int onscreen(void);
  class xy operator<<=(int d);
  class xy operator+=(class xy &other);
  class xy operator-=(class xy &other);
  long operator*(class xy &other);
  class xy operator*=(int);
  int nearer_than(int distance, xy &other);
};

class Bitmap {
public:
  xy size, center;
  uint32_t source;
  uint8_t format;
  int8_t handle;

  void fromtext(int font, const char* s);
  void fromfile(const char *filename, int format = 7);

  void bind(uint8_t handle);

  void wallpaper();
  void draw(int x, int y, int16_t angle = 0);
  void draw(const xy &pos, int16_t angle = 0);

private:
  void defaults(uint8_t f);
  void setup(void);
};

class Bitmap __fromatlas(uint32_t addr);


////////////////////////////////////////////////////////////////////////

class GDClass {
//RndMnkIII
private:
    static const uint16_t TAM_BUFFER_SD; //8192 si se aumenta de tamaño se mejora en eficiencia de lectura 
    static const uint16_t TAM_BUFFER_FT;
    static byte buf[];
    static byte FTbuf[];
public:
  int w, h;
  int model;
uint32_t loadptr;

  void begin(uint8_t options = (GD_CALIBRATE | GD_TRIM | GD_STORAGE));

  uint16_t random();
  uint16_t random(uint16_t n);
  void seed(uint16_t n);
  int16_t rsin(int16_t r, uint16_t th);
  int16_t rcos(int16_t r, uint16_t th);
  void polar(int &x, int &y, int16_t r, uint16_t th);
  uint16_t atan2(int16_t y, int16_t x);

  void copy(const PROGMEM uint8_t *src, int count);
  void copyram(byte *src, int count);

  void self_calibrate(void);

  void swap(void);
  void flush(void);
  void finish(void);

  void play(uint8_t instrument, uint8_t note = 0);
  void sample(uint32_t start, uint32_t len, uint16_t freq, uint16_t format, int loop = 0);

  void get_inputs(void);
  void get_accel(int &x, int &y, int &z);
  struct {
    uint16_t track_tag;
    uint16_t track_val;
    uint16_t rz;
    uint16_t __dummy_1;
    int16_t y;
    int16_t x;
    int16_t tag_y;
    int16_t tag_x;
    uint8_t tag;
    uint8_t ptag;
    uint8_t touching;
    xy xytouch;
  } inputs;

  void AlphaFunc(byte func, byte ref);
  void Begin(byte prim);
  void BitmapHandle(byte handle);
  void BitmapLayout(byte format, uint16_t linestride, uint16_t height);
  void BitmapSize(byte filter, byte wrapx, byte wrapy, uint16_t width, uint16_t height);
  void BitmapSource(uint32_t addr);
  void BitmapTransformA(int32_t a);
  void BitmapTransformB(int32_t b);
  void BitmapTransformC(int32_t c);
  void BitmapTransformD(int32_t d);
  void BitmapTransformE(int32_t e);
  void BitmapTransformF(int32_t f);
  void BlendFunc(byte src, byte dst);
  void Call(uint16_t dest);
  void Cell(byte cell);
  void ClearColorA(byte alpha);
  void ClearColorRGB(byte red, byte green, byte blue);
  void ClearColorRGB(uint32_t rgb);
  void Clear(byte c, byte s, byte t);
  void Clear(void);
  void ClearStencil(byte s);
  void ClearTag(byte s);
  void ColorA(byte alpha);
  void ColorMask(byte r, byte g, byte b, byte a);
  void ColorRGB(byte red, byte green, byte blue);
  void ColorRGB(uint32_t rgb);
  void Display(void);
  void End(void);
  void Jump(uint16_t dest);
  void LineWidth(uint16_t width);
  void Macro(byte m);
  void PointSize(uint16_t size);
  void RestoreContext(void);
  void Return(void);
  void SaveContext(void);
  void ScissorSize(uint16_t width, uint16_t height);
  void ScissorXY(uint16_t x, uint16_t y);
  void StencilFunc(byte func, byte ref, byte mask);
  void StencilMask(byte mask);
  void StencilOp(byte sfail, byte spass);
  void TagMask(byte mask);
  void Tag(byte s);
  void Vertex2f(int16_t x, int16_t y);
  void Vertex2ii(uint16_t x, uint16_t y, byte handle = 0, byte cell = 0);

  void VertexFormat(byte frac);
  void BitmapLayoutH(byte linestride, byte height);
  void BitmapSizeH(byte width, byte height);
  void PaletteSource(uint32_t addr);
  void VertexTranslateX(uint32_t x);
  void VertexTranslateY(uint32_t y);
  void Nop(void);

  // Higher-level graphics commands

  void cmd_append(uint32_t ptr, uint32_t num);
  void cmd_bgcolor(uint32_t c);
  void cmd_button(int16_t x, int16_t y, uint16_t w, uint16_t h, byte font, uint16_t options, const char *s);
  void cmd_calibrate(void);
  void cmd_clock(int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms);
  void cmd_coldstart(void);
  void cmd_dial(int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t val);
  void cmd_dlstart(void);
  void cmd_fgcolor(uint32_t c);
  void cmd_gauge(int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range);
  void cmd_getmatrix(void);
  void cmd_getprops(uint32_t &ptr, uint32_t &w, uint32_t &h);
  void cmd_getptr(void);
  void cmd_gradcolor(uint32_t c);
  void cmd_gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1);
  void cmd_inflate(uint32_t ptr);
  void cmd_interrupt(uint32_t ms);
  void cmd_keys(int16_t x, int16_t y, int16_t w, int16_t h, byte font, uint16_t options, const char*s);
  void cmd_loadidentity(void);
  void cmd_loadimage(uint32_t ptr, int32_t options);
  void cmd_memcpy(uint32_t dest, uint32_t src, uint32_t num);
  void cmd_memset(uint32_t ptr, byte value, uint32_t num);
  uint32_t cmd_memcrc(uint32_t ptr, uint32_t num);
  void cmd_memwrite(uint32_t ptr, uint32_t num);
  void cmd_regwrite(uint32_t ptr, uint32_t val);
  void cmd_number(int16_t x, int16_t y, byte font, uint16_t options, uint32_t n);
  void printNfloat(int16_t x, int16_t y, float f, int16_t Presc, byte font);
  void cmd_progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);
  void cmd_regread(uint32_t ptr);
  void cmd_rotate(int32_t a);
  void cmd_scale(int32_t sx, int32_t sy);
  void cmd_screensaver(void);
  void cmd_scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t size, uint16_t range);
  void cmd_setfont(byte font, uint32_t ptr);
  void cmd_setmatrix(void);
  void cmd_sketch(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t ptr, uint16_t format);
  void cmd_slider(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, uint16_t range);
  void cmd_snapshot(uint32_t ptr);
  void cmd_spinner(int16_t x, int16_t y, byte style, byte scale);
  void cmd_stop(void);
  void cmd_swap(void);
  void cmd_text(int16_t x, int16_t y, byte font, uint16_t options, const char *s);
  void cmd_toggle(int16_t x, int16_t y, int16_t w, byte font, uint16_t options, uint16_t state, const char *s);
  void cmd_track(int16_t x, int16_t y, uint16_t w, uint16_t h, byte tag);
  void cmd_translate(int32_t tx, int32_t ty);

  void cmd_playvideo(int32_t options);
  void cmd_romfont(uint32_t font, uint32_t romslot);
  void cmd_mediafifo(uint32_t ptr, uint32_t size);
  void cmd_setbase(uint32_t b);
  void cmd_videoframe(uint32_t dst, uint32_t ptr);
  void cmd_snapshot2(uint32_t fmt, uint32_t ptr, int16_t x, int16_t y, int16_t w, int16_t h);
  void cmd_setfont2(uint32_t font, uint32_t ptr, uint32_t firstchar);
  void cmd_setrotate(uint32_t r);
  void cmd_videostart();
  void cmd_setbitmap(uint32_t source, uint16_t fmt, uint16_t w, uint16_t h);
  void cmd_sync();

  byte rd(uint32_t addr);
  void wr(uint32_t addr, uint8_t v);
  uint16_t rd16(uint32_t addr);
  void wr16(uint32_t addr, uint16_t v);
  uint32_t rd32(uint32_t addr);
  void wr32(uint32_t addr, uint32_t v);
  void wr_n(uint32_t addr, byte *src, uint32_t n);

  void cmd32(uint32_t b);

  void bulkrd(uint32_t a);
  void resume(void);
  void __end(void);
  void reset(void);

  void dumpscreen(void);
  
  //RndMnkIII: método loadSdFat para carga de assets desde lector SDIO tarjetas STM32
  byte loadSdFat(File& archivo, void (*progress)(long, long) = NULL);
  byte load(const char *filename, void (*progress)(long, long) = NULL);
  void safeload(const char *filename);
  //RndMnkIII: método safeloadSdFat para carga de assets desde lector SDIO tarjetas STM32
  void safeloadSdFat(File& archivo);
  void alert(const char *message);
  
  void textsize(int &w, int &h, int font, const char *s);

  //sdcard SD;

  void storage(void);
  void tune(void);

private:
  static void cFFFFFF(byte v);
  static void cI(uint32_t);
  static void ci(int32_t);
  static void cH(uint16_t);
  static void ch(int16_t);
  static void cs(const char *);
  static void fmtcmd(const char *fmt, ...);

  static void align(byte n);
  void cmdbyte(uint8_t b);

  uint32_t measure_freq(void);

  uint16_t rseed;
};

extern GDClass GD;
extern byte ft8xx_model;

typedef struct {
  byte handle;
  uint16_t w, h;
  uint16_t size;
} shape_t;

// convert integer pixels to subpixels
#define PIXELS(x)  int((x) * 16)

// Convert degrees to Furmans
#define DEGREES(n) ((65536UL * (n)) / 360)

#define NEVER                0
#define LESS                 1
#define LEQUAL               2
#define GREATER              3
#define GEQUAL               4
#define EQUAL                5
#define NOTEQUAL             6
#define ALWAYS               7

#define ARGB1555             0
#define L1                   1
#define L4                   2
#define L8                   3
#define RGB332               4
#define ARGB2                5
#define ARGB4                6
#define RGB565               7
#define PALETTED             8
#define TEXT8X8              9
#define TEXTVGA              10
#define BARGRAPH             11

#define NEAREST              0
#define BILINEAR             1

#define BORDER               0
#define REPEAT               1

#define KEEP                 1
#define REPLACE              2
#define INCR                 3
#define DECR                 4
#define INVERT               5

#define DLSWAP_DONE          0
#define DLSWAP_LINE          1
#define DLSWAP_FRAME         2

#define INT_SWAP             1
#define INT_TOUCH            2
#define INT_TAG              4
#define INT_SOUND            8
#define INT_PLAYBACK         16
#define INT_CMDEMPTY         32
#define INT_CMDFLAG          64
#define INT_CONVCOMPLETE     128

#define TOUCHMODE_OFF        0
#define TOUCHMODE_ONESHOT    1
#define TOUCHMODE_FRAME      2
#define TOUCHMODE_CONTINUOUS 3

#define ZERO                 0
#define ONE                  1
#define SRC_ALPHA            2
#define DST_ALPHA            3
#define ONE_MINUS_SRC_ALPHA  4
#define ONE_MINUS_DST_ALPHA  5

#define BITMAPS              1
#define POINTS               2
#define LINES                3
#define LINE_STRIP           4
#define EDGE_STRIP_R         5
#define EDGE_STRIP_L         6
#define EDGE_STRIP_A         7
#define EDGE_STRIP_B         8
#define RECTS                9

#define OPT_MONO             1
#define OPT_NODL             2
#define OPT_FLAT             256
#define OPT_CENTERX          512
#define OPT_CENTERY          1024
#define OPT_CENTER           (OPT_CENTERX | OPT_CENTERY)
#define OPT_NOBACK           4096
#define OPT_NOTICKS          8192
#define OPT_NOHM             16384
#define OPT_NOPOINTER        16384
#define OPT_NOSECS           32768
#define OPT_NOHANDS          49152
#define OPT_RIGHTX           2048
#define OPT_SIGNED           256
#define OPT_SOUND            32       // ADD OPT_SOUND by @lightcalamar

#define OPT_NOTEAR           4
#define OPT_FULLSCREEN       8
#define OPT_MEDIAFIFO        16

#define LINEAR_SAMPLES       0
#define ULAW_SAMPLES         1
#define ADPCM_SAMPLES        2

// 'instrument' argument to GD.play()

#define SILENCE              0x00

#define SQUAREWAVE           0x01
#define SINEWAVE             0x02
#define SAWTOOTH             0x03
#define TRIANGLE             0x04

#define BEEPING              0x05
#define ALARM                0x06
#define WARBLE               0x07
#define CAROUSEL             0x08

#define PIPS(n)              (0x0f + (n))

#define HARP                 0x40
#define XYLOPHONE            0x41
#define TUBA                 0x42
#define GLOCKENSPIEL         0x43
#define ORGAN                0x44
#define TRUMPET              0x45
#define PIANO                0x46
#define CHIMES               0x47
#define MUSICBOX             0x48
#define BELL                 0x49

#define CLICK                0x50
#define SWITCH               0x51
#define COWBELL              0x52
#define NOTCH                0x53
#define HIHAT                0x54
#define KICKDRUM             0x55
#define POP                  0x56
#define CLACK                0x57
#define CHACK                0x58

#define MUTE                 0x60
#define UNMUTE               0x61

#define RAM_PAL              1056768UL

/*
 FT800 Series Programmer Guide
 http://www.ftdichip.com/Support/Documents/ProgramGuides/FT800%20Programmers%20Guide.pdf
 
 FT81x Series Programmers Guide
 http://brtchip.com/wp-content/uploads/Support/Documentation/Programming_Guides/ICs/EVE/FT81X_Series_Programmer_Guide.pdf
*/

#define RAM_CMD               (ft8xx_model ? 0x308000UL : 0x108000UL)
#define RAM_DL                (ft8xx_model ? 0x300000UL : 0x100000UL)
#define REG_CLOCK             (ft8xx_model ? 0x302008UL : 0x102408UL)
#define REG_CMD_DL            (ft8xx_model ? 0x302100UL : 0x1024ecUL)
#define REG_CMD_READ          (ft8xx_model ? 0x3020f8UL : 0x1024e4UL)
#define REG_CMD_WRITE         (ft8xx_model ? 0x3020fcUL : 0x1024e8UL)
#define REG_CPURESET          (ft8xx_model ? 0x302020UL : 0x10241cUL)
#define REG_CSPREAD           (ft8xx_model ? 0x302068UL : 0x102464UL)
#define REG_DITHER            (ft8xx_model ? 0x302060UL : 0x10245cUL)
#define REG_DLSWAP            (ft8xx_model ? 0x302054UL : 0x102450UL)
#define REG_FRAMES            (ft8xx_model ? 0x302004UL : 0x102404UL)
#define REG_FREQUENCY         (ft8xx_model ? 0x30200cUL : 0x10240cUL)
#define REG_GPIO              (ft8xx_model ? 0x302094UL : 0x102490UL)
#define REG_GPIO_DIR          (ft8xx_model ? 0x302090UL : 0x10248cUL)
#define REG_HCYCLE            (ft8xx_model ? 0x30202cUL : 0x102428UL)
#define REG_HOFFSET           (ft8xx_model ? 0x302030UL : 0x10242cUL)
#define REG_HSIZE             (ft8xx_model ? 0x302034UL : 0x102430UL)
#define REG_HSYNC0            (ft8xx_model ? 0x302038UL : 0x102434UL)
#define REG_HSYNC1            (ft8xx_model ? 0x30203cUL : 0x102438UL)
#define REG_ID                (ft8xx_model ? 0x302000UL : 0x102400UL)
#define REG_INT_EN            (ft8xx_model ? 0x3020acUL : 0x10249cUL)
#define REG_INT_FLAGS         (ft8xx_model ? 0x3020a8UL : 0x102498UL)
#define REG_INT_MASK          (ft8xx_model ? 0x3020b0UL : 0x1024a0UL)
#define REG_MACRO_0           (ft8xx_model ? 0x3020d8UL : 0x1024c8UL)
#define REG_MACRO_1           (ft8xx_model ? 0x3020dcUL : 0x1024ccUL)
#define REG_OUTBITS           (ft8xx_model ? 0x30205cUL : 0x102458UL)
#define REG_PCLK              (ft8xx_model ? 0x302070UL : 0x10246cUL)
#define REG_PCLK_POL          (ft8xx_model ? 0x30206cUL : 0x102468UL)
#define REG_PLAY              (ft8xx_model ? 0x30208cUL : 0x102488UL)
#define REG_PLAYBACK_FORMAT   (ft8xx_model ? 0x3020c4UL : 0x1024b4UL)
#define REG_PLAYBACK_FREQ     (ft8xx_model ? 0x3020c0UL : 0x1024b0UL)
#define REG_PLAYBACK_LENGTH   (ft8xx_model ? 0x3020b8UL : 0x1024a8UL)
#define REG_PLAYBACK_LOOP     (ft8xx_model ? 0x3020c8UL : 0x1024b8UL)
#define REG_PLAYBACK_PLAY     (ft8xx_model ? 0x3020ccUL : 0x1024bcUL)
#define REG_PLAYBACK_READPTR  (ft8xx_model ? 0x3020bcUL : 0x1024acUL)
#define REG_PLAYBACK_START    (ft8xx_model ? 0x3020b4UL : 0x1024a4UL)
#define REG_PWM_DUTY          (ft8xx_model ? 0x3020d4UL : 0x1024c4UL)
#define REG_PWM_HZ            (ft8xx_model ? 0x3020d0UL : 0x1024c0UL)
#define REG_ROTATE            (ft8xx_model ? 0x302058UL : 0x102454UL)
#define REG_SOUND             (ft8xx_model ? 0x302088UL : 0x102484UL)
#define REG_SWIZZLE           (ft8xx_model ? 0x302064UL : 0x102460UL)
#define REG_TAG               (ft8xx_model ? 0x30207cUL : 0x102478UL)
#define REG_TAG_X             (ft8xx_model ? 0x302074UL : 0x102470UL)
#define REG_TAG_Y             (ft8xx_model ? 0x302078UL : 0x102474UL)
#define REG_TOUCH_ADC_MODE    (ft8xx_model ? 0x302108UL : 0x1024f4UL)
#define REG_TOUCH_CHARGE      (ft8xx_model ? 0x30210cUL : 0x1024f8UL)
#define REG_TOUCH_DIRECT_XY   (ft8xx_model ? 0x30218cUL : 0x102574UL)
#define REG_TOUCH_DIRECT_Z1Z2 (ft8xx_model ? 0x302190UL : 0x102578UL)
#define REG_TOUCH_MODE        (ft8xx_model ? 0x302104UL : 0x1024f0UL)
#define REG_TOUCH_OVERSAMPLE  (ft8xx_model ? 0x302114UL : 0x102500UL)
#define REG_TOUCH_RAW_XY      (ft8xx_model ? 0x30211cUL : 0x102508UL)
#define REG_TOUCH_RZ          (ft8xx_model ? 0x302120UL : 0x10250cUL)
#define REG_TOUCH_RZTHRESH    (ft8xx_model ? 0x302118UL : 0x102504UL)
#define REG_TOUCH_SCREEN_XY   (ft8xx_model ? 0x302124UL : 0x102510UL)
#define REG_TOUCH_SETTLE      (ft8xx_model ? 0x302110UL : 0x1024fcUL)
#define REG_TOUCH_TAG         (ft8xx_model ? 0x30212cUL : 0x102518UL)
#define REG_TOUCH_TAG_XY      (ft8xx_model ? 0x302128UL : 0x102514UL)
#define REG_TOUCH_TRANSFORM_A (ft8xx_model ? 0x302150UL : 0x10251cUL)
#define REG_TOUCH_TRANSFORM_B (ft8xx_model ? 0x302154UL : 0x102520UL)
#define REG_TOUCH_TRANSFORM_C (ft8xx_model ? 0x302158UL : 0x102524UL)
#define REG_TOUCH_TRANSFORM_D (ft8xx_model ? 0x30215cUL : 0x102528UL)
#define REG_TOUCH_TRANSFORM_E (ft8xx_model ? 0x302160UL : 0x10252cUL)
#define REG_TOUCH_TRANSFORM_F (ft8xx_model ? 0x302164UL : 0x102530UL)
#define REG_TRACKER           (ft8xx_model ? 0x309000UL : 0x109000UL)
#define REG_TRIM              (ft8xx_model ? 0x302180UL : 0x10256cUL)
#define REG_VCYCLE            (ft8xx_model ? 0x302040UL : 0x10243cUL)
#define REG_VOFFSET           (ft8xx_model ? 0x302044UL : 0x102440UL)
#define REG_VOL_PB            (ft8xx_model ? 0x302080UL : 0x10247cUL)
#define REG_VOL_SOUND         (ft8xx_model ? 0x302084UL : 0x102480UL)
#define REG_VSIZE             (ft8xx_model ? 0x302048UL : 0x102444UL)
#define REG_VSYNC0            (ft8xx_model ? 0x30204cUL : 0x102448UL)
#define REG_VSYNC1            (ft8xx_model ? 0x302050UL : 0x10244cUL)

//There is no register FT800 Series
//#define REG_MEDIAFIFO_READ    0x309014
//#define REG_MEDIAFIFO_WRITE   0x309018
#define REG_MEDIAFIFO_READ    (ft8xx_model ? 0x309014UL : 0x000000UL)
#define REG_MEDIAFIFO_WRITE   (ft8xx_model ? 0x309018UL : 0x000000UL)

#define VERTEX2II(x, y, handle, cell) \
        ((2UL << 30) | (((x) & 511UL) << 21) | (((y) & 511UL) << 12) | (((handle) & 31) << 7) | (((cell) & 127) << 0))

#define ROM_PIXEL_FF        0xc0400UL


class Poly {
    int x0, y0, x1, y1;
    int x[8], y[8];
    byte n;
    void restart() {
      n = 0;
      x1 = 0;
      y1 = 0;
    }
    void perim() {
      for (byte i = 0; i < n; i++)
        GD.Vertex2f(x[i], y[i]);
      GD.Vertex2f(x[0], y[0]);
    }
  public:
    void begin() {
      restart();

      GD.ColorMask(0,0,0,0);
      GD.StencilOp(KEEP, INVERT);
      GD.StencilFunc(ALWAYS, 255, 255);
    }
    void v(int _x, int _y) {
      x0 = min(x0, _x >> 4);
      x1 = max(x1, _x >> 4);
      y0 = min(y0, _y >> 4);
      y1 = max(y1, _y >> 4);
      x[n] = _x;
      y[n] = _y;
      n++;
    }
    void paint() {
      x0 = max(0, x0);
      y0 = max(0, y0);
      x1 = min(16 * 800, x1);
      y1 = min(16 * 480, y1);
      GD.ScissorXY(x0, y0);
      GD.ScissorSize(x1 - x0 + 1, y1 - y0 + 1);
      GD.Begin(EDGE_STRIP_B);
      perim();
    }
    void finish() {
      GD.ColorMask(1,1,1,1);
      GD.StencilFunc(EQUAL, 255, 255);

      GD.Begin(EDGE_STRIP_B);
      GD.Vertex2f(0, 0);
      GD.Vertex2f(800*16, 0);
    }
    void draw() {
      paint();
      finish();
    }
    void outline() {
      GD.Begin(LINE_STRIP);
      perim();
    }
};


class Streamer {
public:
  void begin(const char *rawsamples,
             uint16_t freq = 44100,   //8000,  // 44100
             byte format =  ADPCM_SAMPLES, // ULAW_SAMPLES,  // ADPCM_SAMPLES
             uint32_t _base = (0x40000UL - 8192), uint16_t size = 8192) {
    GD.__end();
    //r.openfile(rawsamples);
    if (!r.open(rawsamples)) {
//Serial.println("Streamer open failed");
      GD.alert("Stream file open failed");
    } else {
//Serial.println("Streamer open ok");
      GD.resume();

      base = _base;
      mask = size - 1;
      wp = 0;

      //for (byte i = 10; i; i--)
      for (byte i = 0; i<10; i++) {
//Serial.println("i=" + String(i));
        feed();
      }
//Serial.println("Streamer feed end");
      GD.sample(base, size, freq, format, 1);
    }
  }
  int feed() {
    uint16_t rp = GD.rd32(REG_PLAYBACK_READPTR) - base;
    uint16_t freespace = mask & ((rp - 1) - wp);
//Serial.println("freespace=" + String(freespace));
    if (freespace >= 512) {  // If freespace is larg than 512, then read file
      // REPORT(base);
      // REPORT(rp);
      // REPORT(wp);
      // REPORT(freespace);
      // Serial.println();
      byte buf[512];
      // uint16_t n = min(512, r.size - r.offset);
      // n = (n + 3) & ~3;   // force 32-bit alignment
      GD.__end();
      //r.readsector(buf);
      int32_t c = r.curPosition();
      int32_t p = r.peek();
//Serial.print("curPosition=" + String(c));
//Serial.print(" peek=" + String(p));
      int n = r.read(buf,512);
//Serial.println(" n=" + String(n));

      GD.resume();
      GD.cmd_memwrite(base + wp, 512);
      GD.copyram(buf, 512);
      wp = (wp + 512) & mask;
    }
    uint32_t m = r.fileSize();
    uint32_t p = r.curPosition();
    //return r.offset < r.size;
    return p < m;
  }
  void progress(uint16_t &val, uint16_t &range) {
    //uint32_t m = r.size;
    uint32_t m = r.fileSize();
    //uint32_t p = min(r.offset, m);
    uint32_t p = r.curPosition();
    //p = min(p, m);
    while (m > 0x10000) { // large than 16 bit
      m >>= 1;
      p >>= 1;
    }
    val = p;
    range = m;
  }
private:
  //Reader r;
  File r;
  uint32_t base;
  uint16_t mask;
  uint16_t wp;
};


class MoviePlayer
{
  uint32_t mf_size, mf_base, wp;
  //Reader r;
  File r;
  void loadsector() {
    byte buf[512];
    GD.__end();
    //r.readsector(buf);
    int32_t c = r.curPosition();
    int32_t p = r.peek();
//Serial.print("curPosition=" + String(c));
//Serial.print(" peek=" + String(p));
    int n = r.read(buf,512);
//Serial.println(" n=" + String(n));
    GD.resume();
    GD.wr_n(mf_base + wp, buf, 512);
    wp = (wp + 512) & (mf_size - 1);
//Serial.println("wp=" + String(wp));
  }

public:
  int begin(const char *filename) {
//Serial.println("MoviePlayer begin filanem=" + String(filename));
    if (ft8xx_model == 0) GD.alert("FT800 is not support MoviePlayer");
    mf_size = 0x40000UL;
    mf_base = 0x100000UL - mf_size;
    GD.__end();
    //if (!r.openfile(filename)) {
    if (!r.open(filename)) {
  //    Serial.println("MoviePlayer open failed");
      return 0;
    }
//Serial.println("MoviePlayer open ok");
    GD.resume();

    wp = 0;
    while (wp < (mf_size - 512)) {
      loadsector();
    }
//Serial.println("MoviePlayer loadsector ok");

    GD.cmd_mediafifo(mf_base, mf_size);
//Serial.println("MoviePlayer mediafifo ok");
//Serial.print("REG_MEDIAFIFO_WRITE=");
//Serial.println(REG_MEDIAFIFO_WRITE,HEX);
    GD.cmd_regwrite(REG_MEDIAFIFO_WRITE, wp);
    GD.finish();

    return 1;
  }
  int service() {
    //if (r.eof()) {
    if (r.peek() == -1) {
      return 0;
    } else {
      //byte buf[512];

      uint32_t fullness = (wp - GD.rd32(REG_MEDIAFIFO_READ)) & (mf_size - 1);
      while (fullness < (mf_size - 512)) {
        loadsector();
        fullness += 512;
        GD.wr32(REG_MEDIAFIFO_WRITE, wp);
      }
      return 1;
    }
  }
  void play() {
//Serial.println("MoviePlayer play start");
//    GD.cmd_playvideo(OPT_MEDIAFIFO | OPT_FULLSCREEN | OPT_SOUND);  // ADD OPT_SOUND by @lightcalamar
    GD.cmd_playvideo(OPT_MEDIAFIFO | OPT_SOUND);  // ADD OPT_SOUND by @lightcalamar 11/06/2018
    // Delete OPT_FULLSCREEN
    GD.flush();
    while (service())
      ;
    GD.cmd_memcpy(0, 0, 4);
    GD.finish();
//Serial.println("MoviePlayer play end");
  }
};
#endif
