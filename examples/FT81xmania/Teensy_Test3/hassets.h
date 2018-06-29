// This file was generated with the command-line:
//    /usr/local/bin/gd3asset -f hassets.gd2 -o hassets.h /home/simon/Chrome_Edges.png,format=L8 /home/simon/High_Beam.png,format=L8 /home/simon/globe.png,format=ARGB4 /home/simon/icons.png,format=L4 /home/simon/redglow.png,format=RGB565 /home/simon/OCRAStd12.ttf,size=12,format=L4 /home/simon/OCRAStd16.ttf,size=16,format=L4 /home/simon/OCRAStd32.ttf,size=32,format=L4 /home/simon/OCRAStd80.ttf,size=80,format=L4,firstchar=47,charset=N0123456789 /home/simon/needles.png,format=L4

//#define LOAD_ASSETS0()  GD.safeloadSdFat(GAsset0); // library GD2UB
#define LOAD_ASSETS()  GD.safeload("hassets.gd2"); // library GD23Z

#define CHROME_EDGES_HANDLE 0
#define CHROME_EDGES_WIDTH 800
#define CHROME_EDGES_HEIGHT 480
#define CHROME_EDGES_CELLS 1
#define HIGH_BEAM_HANDLE 1
#define HIGH_BEAM_WIDTH 680
#define HIGH_BEAM_HEIGHT 466
#define HIGH_BEAM_CELLS 1
#define GLOBE_HANDLE 2
#define GLOBE_WIDTH 300
#define GLOBE_HEIGHT 270
#define GLOBE_CELLS 1
#define ICONS_HANDLE 3
#define ICONS_WIDTH 50
#define ICONS_HEIGHT 50
#define ICONS_CELLS 1
#define REDGLOW_HANDLE 4
#define REDGLOW_WIDTH 56
#define REDGLOW_HEIGHT 55
#define REDGLOW_CELLS 1
#define OCRASTD12_HANDLE 5
#define OCRASTD12_WIDTH 9
#define OCRASTD12_HEIGHT 14
#define OCRASTD12_CELLS 96
#define OCRASTD16_HANDLE 6
#define OCRASTD16_WIDTH 12
#define OCRASTD16_HEIGHT 18
#define OCRASTD16_CELLS 96
#define OCRASTD32_HANDLE 7
#define OCRASTD32_WIDTH 23
#define OCRASTD32_HEIGHT 33
#define OCRASTD32_CELLS 96
#define OCRASTD80_HANDLE 8
#define OCRASTD80_WIDTH 40
#define OCRASTD80_HEIGHT 62
#define OCRASTD80_CELLS 11
#define NEEDLES_HANDLE 9
#define NEEDLES_WIDTH 62
#define NEEDLES_HEIGHT 72
#define NEEDLES_CELLS 1
#define ASSETS_END 951860UL


//#define LOAD_ASSETS()  (GD.safeload("hassets.gd2"), GD.loadptr = ASSETS_END)

static const shape_t CHROME_EDGES_SHAPE = {0, 800, 480, 0};
static const shape_t HIGH_BEAM_SHAPE = {1, 680, 466, 0};
static const shape_t GLOBE_SHAPE = {2, 300, 270, 0};
static const shape_t ICONS_SHAPE = {3, 50, 50, 0};
static const shape_t REDGLOW_SHAPE = {4, 56, 55, 0};
static const shape_t OCRASTD12_SHAPE = {5, 9, 14, 0};
static const shape_t OCRASTD16_SHAPE = {6, 12, 18, 0};
static const shape_t OCRASTD32_SHAPE = {7, 23, 33, 0};
static const shape_t OCRASTD80_SHAPE = {8, 40, 62, 0};
static const shape_t NEEDLES_SHAPE = {9, 62, 72, 0};
struct {
 Bitmap chrome_edges;
 Bitmap high_beam;
 Bitmap globe;
 Bitmap icons;
 Bitmap redglow;
 Bitmap ocrastd12;
 Bitmap ocrastd16;
 Bitmap ocrastd32;
 Bitmap ocrastd80;
 Bitmap needles;
} bitmaps = {
/*     chrome_edges */  {{800, 480}, {400, 240},      0x0UL,  3,  0},
/*        high_beam */  {{680, 466}, {340, 233},  0x5dc00UL,  3,  1},
/*            globe */  {{300, 270}, {150, 135},  0xab1d0UL,  6,  2},
/*            icons */  {{ 50, 50}, { 25, 25},  0xd2aa0UL,  2,  3},
/*          redglow */  {{ 56,  55}, { 28,  27},  0xd5692UL,  7,  4},
/*        ocrastd12 */  {{  9,  14}, {  4,   7},  0xd6ea2UL,  2,  5},
/*        ocrastd16 */  {{ 12,  18}, {  6,   9},  0xd8978UL,  2,  6},
/*        ocrastd32 */  {{ 23,  33}, { 11,  16},  0xdb28cUL,  2,  7},
/*        ocrastd80 */  {{ 40,  62}, { 20,  31},  0xe47a0UL,  2,  8},
/*          needles */  {{ 62,  72}, { 31,  36},  0xe7d7cUL,  2,  9}
};
