#if defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

char TXP[50];

void STM32a()
{
  ram = FreeStack();
  FRAM=ram;

  if(FRAM<=196608){SRAM=327680;}
              else{SRAM=524288;}
}

void STM32()
{
  GD.ColorRGB(0x00ff00); sprintf(TXP,"F_CPU: %d MHz", (F_CPU/1000000));  GD.cmd_text(25, 465, 26, 0, TXP);
  GD.ColorRGB(0x00ff00); sprintf(TXP,"F_RAM:  %d byte", (ram)); // GD.cmd_text(320, 465, 26, 0, TXP);
  GD.cmd_text(320, 465, 26, 0, "U_RAM:               %");  //GD.printNfloat(420, 465, SRAM-FRAM, 2, 26);
  GD.printNfloat(390, 465, 100*(SRAM-FRAM)/SRAM, 2, 26);
}
