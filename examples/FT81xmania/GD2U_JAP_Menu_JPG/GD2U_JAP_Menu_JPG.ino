#include <GD23ZU.h>

int SalirdeMP, DespXMP=-490;   //MP       480x272 px
int SaliraMP, DespX=-350;      //M1 y M2  335x250 px

int VelDespX=10;

int movx, velocidadx;

void setup()
{
  GD.begin(GD_STORAGE);
  //GD.cmd_setrotate(1);
  //GD.self_calibrate();
  
  MP();
}
void loop(){}

int BX=100, BY=80;

void MP()
{
  GD.cmd_loadimage(0, 0);
  GD.load("tree.jpg");
  
  while(1)
  {
   GD.ClearColorRGB(0x000055);
   GD.Clear();
   GD.get_inputs();

   GD.SaveContext();
     GD.Begin(BITMAPS);
     GD.Vertex2f(DespXMP*16, 0*16);
    
    if(SalirdeMP==1){DespXMP=DespXMP-VelDespX; if(DespXMP<=-500){SalirdeMP=0; DespX=-350; M1();}}
    if(SalirdeMP==2){DespXMP=DespXMP-VelDespX; if(DespXMP<=-500){SalirdeMP=0; DespX=-820; M2();}}
    if(SalirdeMP==0){DespXMP=DespXMP+VelDespX; if(DespXMP>=0){DespXMP=0;}}
   GD.RestoreContext();

   GD.SaveContext();
    Poly po;   GD.ColorRGB(0x008B4B);
    if (movx==0){movx = 0;}else{movx = movx + velocidadx;}
    GD.Tag(3);   po.begin();  po.v(16*(0+movx),16*0);   po.v(16*(140+movx),16*0);  po.v(16*(80+movx),16*60);  po.v(16*(0+movx),16*60);  po.draw(); GD.Tag(255);
   GD.RestoreContext();

   GD.SaveContext();
   GD.Tag(1); GD.cmd_fgcolor(0x005000);  
    GD.cmd_button((GD.w/2)-100, (GD.h/2)-20, BX, BY, 28, 0,"M1");  
   GD.Tag(255);
   GD.RestoreContext();

   if (GD.inputs.tag==1)
   {
     //M1();
     SalirdeMP=1;
   }

   GD.Tag(2); GD.cmd_fgcolor(0x500000);  
    GD.cmd_button((GD.w/2)+100, (GD.h/2)-20, BX, BY, 28, 0,"M2");  
   GD.Tag(255);

   if (GD.inputs.tag==2)
   {
     //M2();
     SalirdeMP=2;
   }   

   GD.swap();
  } 
}

void M1()
{
  GD.cmd_loadimage(0, 0);
  GD.load("selfie.jpg");
  
  while(1)
  {
   GD.ClearColorRGB(0x005000);
   GD.Clear();
   GD.get_inputs();

   GD.SaveContext();
     GD.Begin(BITMAPS);
     GD.Vertex2f(DespX*16, 0*16);

    if(SaliraMP==2){DespX=DespX-VelDespX; if(DespX<=-360){SaliraMP=0;  DespX=-820; M2();}}
    if(SaliraMP==1){DespX=DespX-VelDespX; if(DespX<=-360){SaliraMP=0; MP();}}
    if(SaliraMP==0){DespX=DespX+VelDespX; if(DespX>=0){DespX=0;}}
   GD.RestoreContext();

   GD.Tag(3); GD.cmd_fgcolor(0x500000);  
    GD.cmd_button((GD.w/2)-100, (GD.h/2)-100, BX, BY, 28, 0,"M2");  
   GD.Tag(255);

   if (GD.inputs.tag==3)
   {
     //M2();
     SaliraMP=2;
   }

   GD.Tag(4); GD.cmd_fgcolor(0x000050);  
    GD.cmd_button((GD.w/2)+100, (GD.h/2)+50, BX, BY, 28, 0,"MP");  
   GD.Tag(255);

   if (GD.inputs.tag==4)
   {
     //MP();
     SaliraMP=1;
   }

   GD.swap();
  } 
}



void M2()
{
  GD.cmd_loadimage(0, 0);
  //GD.load("selfie3.jpg");
  GD.load("Fn1.jpg");

  while(1)
  {
   GD.ClearColorRGB(0x500000);
   GD.Clear();
   GD.get_inputs();

   GD.SaveContext();
     GD.Begin(BITMAPS);
     GD.Vertex2f(DespX*16, 0*16);

    if(SaliraMP==2){DespX=DespX-VelDespX; if(DespX<=-815){SaliraMP=0; DespX=-350; M1();}}
    if(SaliraMP==1){DespX=DespX-VelDespX; if(DespX<=-815){SaliraMP=0; MP();}}
    if(SaliraMP==0){DespX=DespX+VelDespX; if(DespX>=0){DespX=0;}}
   GD.RestoreContext();

   GD.Tag(5); GD.cmd_fgcolor(0x005000);  
    GD.cmd_button((GD.w/2)+100, (GD.h/2)-100, BX, BY, 28, 0,"M1");  
   GD.Tag(255);

   if (GD.inputs.tag==5)
   {
     SaliraMP=2;
   }

   GD.Tag(6); GD.cmd_fgcolor(0x000050);  
    GD.cmd_button((GD.w/2)-100, (GD.h/2)+50, BX, BY, 28, 0,"MP");  
   GD.Tag(255);

   if (GD.inputs.tag==6)
   {
     SaliraMP=1;
   }

   GD.swap();
  } 
}
