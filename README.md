![FT81xMania](logo.png?raw=true "GD23ZU")

# MexSpa Team contribution for [FT81xMania](https://ft81xmania.com/comunidad/)

# GD23ZU Library
Library for STM32F1x, STM32F4x, STM32F7x and Teensy 3.5/6 based on library Gameduino 3 Playback videos **.avi** with sound, from a microSD reader **for FTDI; FT810, FT811** and **FT813** screens.

# History
We are an enthusiastic team of screens based on [FTDI chip](http://www.ftdichip.com/EVE.htm). We like programming and share our advances. We have reached this goal, our only interest is to give the opportunity to operate this library to multiple platforms of MCU's in the market. Wait for you to enjoy the codes, of our, your time.
Two friends, in Mexico and another in Spain, the water or the puddle does not scare us ...
Last month of May, a member of the Team, we left. We move on.
Without any interest, we publish this library altruistically.

# June 7, Release initial.

1. Library based on [James Bowman](https://github.com/jamesbowman/gd2-lib)
2. Library based on [lightcalamar GD23STM32_F103x](https://github.com/lightcalamar/GD23STM32_F103x) > programmed and modified by [@TFTLCDCyg](https://ft81xmania.com/comunidad/Usuario-tftlcdcyg)
3. Library based on [STM32_GD2](https://github.com/nopnop2002/STM32_GD2) user **@nopnop2002** this [link](http://stm32duino.com/viewtopic.php?f=9&t=3466#p44477)

Library with function special **playback videos** on MCU's ***STM32F103x***, ***STM32F407x***, ***Nucleo-STM32F767ZI*** and ***Teensy 3.5/6***. This library is compatible with the STM32 families described above. For any other MCU's you can use Unified Library [GD23Z](https://github.com/lightcalamar/GD23Z). All commands are equal to library genuine from James Bowman Gameduino 2/3 last release.

---

[![MexSpan Team](https://i.imgur.com/Tbz341i.png)](https://www.youtube.com/watch?v=O8OUXB02g3wY "Click here to MexSpa Team")

# Video on youtube launching [MexSpa video](https://www.youtube.com/watch?v=O8OUXB02g3w)

---

# STM32F support
    Models Boards;  (This boards SUPPORT playback video, see Wiki)
* STM32F103x (All)
* STM32F407x (BLACK or M4-DEMO)
* STM32F429x (Discovery board)
* Nucleo STM32F767x
* Consult the documentation different boards to pins SPI1 and SPI2 [Follow this board's Wiki](https://github.com/lightcalamar/GD23Z/wiki/STM32Fx-configuration) 
    
# Teensy support 
    Models Boards;  (This boards SUPPORT playback video (see Wiki)
* Teensy 3.5 and Teensy 3.6

# ESP32 support
It is **not yet possible to play videos**. To be working very hard and we hope to give the big surprise with this MCU.
At the moment only work very fine with library  Unified Library [GD23Z](https://github.com/lightcalamar/GD23Z).

---

# Software requirement
Donwload and [use the Core STM32GENERIC by danieleff](https://github.com/danieleff/STM32GENERIC)

# Installation
## Linux
1. Unzip library copy and paste in /home/your_user/arduino-1.8.5/libraries/
2. Install Arduino Zero or Arduino Sam from the board manager for the arm-none-eabi-gcc compiler
3. Download the [GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).
   Recommended use GCC last version, change compiler.path in [platform.txt] 
   
## Windows
1. Unzip library copy and paste path of IDE arduino-1.8.5/libraries/
2. Install Arduino Zero or Arduino Sam from the board manager for the arm-none-eabi-gcc compiler
3. Download the [GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).
   Recommended use GCC ver 5.4.1-2016q2,Change compiler.path in [platform.txt](https://github.com/danieleff/STM32GENERIC/blob/master/STM32/platform.txt#L21) to point to that you downloaded.   
   
# Adjust the library according to your board and screen
See wiki. (Coming soon)

# Extra libraries    
* To calibrate the FT81x screens in STM32Fx we need an external EEPROM. You can download the AT24C32 library from 
 [this link](https://ft81xmania.com/comunidad/Tema-library-eeprom-at24xx)
* Library **SdFat** for STM32F be included in core STMGENERIC. This be edited to work in STM32. **DO NOT USE** the official library. It will not work in STM32 architecture. 

# Tips
* These screens have a problem, being the length of the SPI BUS cables. The purpose is to be very short, between 7 and 10 mm. Also a tip is to use on this same SPI BUS some pull-up resistors. [Be a solution of a forum user](https://ft81xmania.com/comunidad/Tema-reset-power-up-issue-with-hotmmcu-ft811cb) by [@raufjay]( https://ft81xmania.com/comunidad/Usuario-raufjay)

---

# Where to buy FT8xx screens?
* [New Haven Display](http://www.newhavendisplay.com/advanced_search_result.html?y=0&x=0&keyword=eve2&search_in_description=1) Location in the USA, Boston, great finish quality.
* [HotMCU](https://www.hotmcu.com/index.php?main_page=advanced_search_result&search_in_description=1&keyword=ftdi)
 Good prices and good design with frame included. Only produce 5 and 4.3 inches, models FT800, FT810 and FT11.
Origin; Singapore
* [Riverdi Display](https://riverdi.com/product/mikroc-pro-for-ft90x/) Quality and variety of screens, their origin; Poland.
* [Gameduino 3](https://coolcomponents.co.uk/products/gameduino-3) Original Gameduino3 by James Bowman, release in December 2017, 4.3-inch screen and FT810, UK online store.

---

# Where do I find information about programming EVE2 of FTDI?
* [Support Oficial FTDI-Chip](http://www.ftdichip.com/Support/SoftwareExamples/FT800_Projects.htm#FT8xx_Selecting%20an%20LCD%20Display) Complete list of examples and documentation of FTDI-Chip
* [Migration FT80x to FT81x](http://www.ftdichip.com/Support/Documents/AppNotes/AN_390%20FT80x%20To%20FT81x%20Migration%20Guide.pdf) Documentation PDF.
* [FT81x High resolution](http://brtchip.com/wp-content/uploads/Support/Documentation/Datasheets/ICs/EVE/DS_FT81x.pdf), This a PDF documentation for FT81x screens programming guide.

---

# Resource and help for FTDI screens
* A book by [James Bowman](https://ft81xmania.com/comunidad/Tema-gd2-book-by-james-bownman-screen-ftdi?highlight=book)
* [Service Manual by @lightcalamar for Arduino, models; UNO r3 and MEGA (Spanish language)](https://ft81xmania.com/comunidad/Tema-ft8xx-service-manual)
* [Utility for transformation of images and assets](https://github.com/lightcalamar/Asset-Tools-FT81X-v3) on repository 
* [Designer Editor](https://drive.google.com/open?id=1cmoGCe6tMLE_6JSjYLIngVogORFmBwBG) Maybe the best version for FTDI screen design. This version exports the code used to the Gameduino library. Note: this version is out of date, however very useful for your projects. It will greatly reduce the time used in your programming. It is capable of exporting images, to PROGMEM code and many more functions. Discover it!

---

# Donwload files examples videos for your test.
Coming soon ...

---

# Software converter videos
Coming soon ...

---

# Wiki
Coming soon ...

---

# Useful links
* Repository by [Amthony Di Girolamo](https://github.com/AnthonyDiGirolamo/gd2-lib-sdfat-testing)
* Core for STM32GENERIC by [Danieleff](https://github.com/danieleff/STM32GENERIC), installation for IDE Arduino with STM32 boards
* Library GD2 by [James Bowman](https://github.com/jamesbowman/gd2-lib)

# Autors
> [@TFTLCDCyg](https://ft81xmania.com/comunidad/Usuario-tftlcdcyg) and
> [@lightcalamar](https://ft81xmania.com/comunidad/Usuario-lightcalamar) (The MexSpa Team)

# License
> [LICENSE](https://github.com/lightcalamar/GD23Z/blob/master/LICENSE "LICENSE") by James Bowman

---

# Acknowledgments
- Infinite thanks to **@nopnop2002** [STM32_GD2 Library on GitHub](https://github.com/nopnop2002/STM32_GD2)
- Hat tip to anyone who's code was used
- Inspiration
- To the Teensy forum of Paul Stoffregen for **not believing this possible**. According to these we spam ...
  [See this link](https://forum.pjrc.com/threads/51842-New-playback-videos-all-MCU-with-FTDI-screens)
- More time frustation...
- Etc...

---

# Youtube DEMO playback videos on Teensy and STM32Fx

https://www.youtube.com/watch?v=3K4Qc5GNrAg&t=6s

https://www.youtube.com/watch?v=l_VrVL0g6X0

https://www.youtube.com/watch?v=XSeefeQb20A&t=6s


# Coming soon ...
Update June, 4 2018
