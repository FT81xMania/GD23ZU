#include <GD23ZU.h>

char nombre[32];
char estado[40];
int inicio=1;  int fin=2378;  //SW

int fcount=inicio;

const long FPS=60;  //permite la máxima tasa de refresco posible
long prevFTime;
long currFTime;
long frmTime=1000 / FPS;

//variables para llevar cuenta del tiempo de reproducción:
int hrs=0;
int mins=0;
int secs=0;

uint32_t ram;
float FRAM, SRAM;

long currMillis;
long prevMillis;
int contador=0;
int frms=0;

unsigned long start;

void setup()
{
  GD.begin(GD_STORAGE);
  prevMillis=millis();
  prevFTime=millis();
  STM32a();
}

void loop(){
  currMillis=millis();

  if(currMillis-prevMillis > 1000){
    prevMillis=currMillis;
    verEstado();
    contador=0;
  }
  
  if(fcount > fin)
  {
    fcount=inicio;
  }
  
  currFTime=millis();
  if(currFTime-prevFTime > frmTime)
  {
      prevFTime=currFTime;
      snprintf(nombre, 30, "SW%04d.jpg", fcount);  //SW
      GD.cmd_loadimage(0, 0);
      GD.load(nombre);
      
      contador++;
      fcount++;
      frms++;
  }
  if (frms == FPS){
    frms=0;
    secs++;
  }
  if (secs == 60)
  {
    secs=0;
    mins++;
  }
  if (mins == 60)
  {
    mins=0;
    hrs++;
  }

}

void verEstado()
{
   sprintf(estado, "H:%02d M:%02d S:%02d Fps:%02d", hrs,mins,secs,contador); 
   GD.Clear();
   GD.Begin(BITMAPS);  
   GD.Vertex2ii(0, 0); 
   GD.ColorRGB(0,255,0);  GD.cmd_text(600,465,21,OPT_CENTER, estado);              
   STM32();              
   GD.swap();
}
