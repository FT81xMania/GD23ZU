#include <GD23ZU.h>

const short  y_data[] = {
939, 940, 941, 942, 944, 945, 946, 947, 951, 956, 
962, 967, 973, 978, 983, 989, 994, 1000, 1005, 1015, 
1024, 1034, 1043, 1053, 1062, 1075, 1087, 1100, 1112, 1121, 
1126, 1131, 1136, 1141, 1146, 1151, 1156, 1164, 1172, 1179, 
1187, 1194, 1202, 1209, 1216, 1222, 1229, 1235, 1241, 1248, 
1254, 1260, 1264, 1268, 1271, 1275, 1279, 1283, 1287, 1286, 
1284, 1281, 1279, 1276, 1274, 1271, 1268, 1266, 1263, 1261, 
1258, 1256, 1253, 1251, 1246, 1242, 1237, 1232, 1227, 1222, 
1218, 1215, 1211, 1207, 1203, 1199, 1195, 1191, 1184, 1178, 
1171, 1165, 1159, 1152, 1146, 1141, 1136, 1130, 1125, 1120, 
1115, 1110, 1103, 1096, 1088, 1080, 1073, 1065, 1057, 1049, 
1040, 1030, 1021, 1012, 1004, 995, 987, 982, 978, 974};

Bitmap title, xaxis, yaxis, Gtipo1E, Gtipo1, Gtipo2E, Gtipo2, Gtipo3E, Gtipo3, Gtipo4E, Gtipo4, Gtipo5E, Gtipo5, Gtipo6E, Gtipo6, Gtipo7E, Gtipo7;

char TX[50];

int graph=1;
int A=50, AC=50, AT=50;
int B=2, BC=2, BT=2;

//Marco para graficar
int PX0=50, PY0=50, PXMAX=450-PX0, PYMAX=PY0+230, PYBase= PY0+225;

int esp=2;

void setup()
{
  //GD.begin();
  GD.begin(GD_STORAGE);

  title.fromtext(30, "Graph test");
  xaxis.fromtext(28, "X axis");
  yaxis.fromtext(28, "Y axis");
  Gtipo1E.fromtext(26, "                2");  Gtipo1.fromtext(28, "y = 0.001x");
  Gtipo2E.fromtext(26, "                2");  Gtipo2.fromtext(28, "y = 0.5x");
  Gtipo3E.fromtext(26, "                          2");  Gtipo3.fromtext(28, "y = 0.5x-0.0008x");
  Gtipo4E.fromtext(26, "                          2");  Gtipo4.fromtext(28, "y = AsinBx");
  Gtipo5E.fromtext(26, "                          2");  Gtipo5.fromtext(28, "y = AcosBx");
  Gtipo6E.fromtext(26, "                          2");  Gtipo6.fromtext(28, "y = AtanBx");
  Gtipo7E.fromtext(26, "                          2");  Gtipo7.fromtext(28, "y = f(array[x])");
}

void loop()
{
  GD.ClearColorRGB(0x000010);
  GD.Clear();  GD.get_inputs();

  GD.ColorRGB(0xffffff);
  title.draw(PXMAX/2, PY0-20);
  yaxis.draw(PX0-20, PYMAX, DEGREES(270));
  xaxis.draw(PXMAX/2, PYMAX+20);

  GD.Tag(1);  Gtipo1E.draw(130+20, 90-25+PYMAX+20);   Gtipo1.draw(100+20, 100-30+PYMAX+20);  GD.Tag(255);
  GD.Tag(2);                                          Gtipo2.draw(88+20, 100+PYMAX+20);      GD.Tag(255);
  GD.Tag(3);  Gtipo3E.draw(175+20, 100+25+PYMAX+20);  Gtipo3.draw(130+20, 100+30+PYMAX+20);  GD.Tag(255);
  GD.Tag(4);                                          Gtipo4.draw(100+20, 100+60+PYMAX+20);  GD.Tag(255);
  GD.Tag(5);                                          Gtipo5.draw(320+20, 100+60+PYMAX+20);  GD.Tag(255);
  GD.Tag(6);                                          Gtipo6.draw(520+20, 100+60+PYMAX+20);  GD.Tag(255);
  GD.Tag(7);                                          Gtipo7.draw(700+20, 100+60+PYMAX+20);  GD.Tag(255);

  MarcoG();

  if(graph==1){
  GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  Trace1();
  GD.ColorRGB(0xffffff); GD.Begin(POINTS);      GD.PointSize(4*16);  Trace1Random();}

  if(graph==2){
  GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  Trace2();
  GD.ColorRGB(0xffffff); GD.Begin(POINTS);      GD.PointSize(4*16);  Trace2();}

  if(graph==3){
  GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  Trace3();
  GD.ColorRGB(0xffffff); GD.Begin(POINTS);      GD.PointSize(4*16);  Trace3();}

  if(graph==4){
    GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  SIN();
    GD.ColorRGB(0xffffff); GD.Begin(POINTS);      GD.PointSize(1*16);  SIN();

    GD.cmd_fgcolor(0x000050);
    GD.Tag(11);  GD.cmd_button(500, 250, 120, 60, 29, 0,  "B-");  GD.Tag(255);
    GD.Tag(12);  GD.cmd_button(660, 250, 120, 60, 29, 0,  "B+");  GD.Tag(255);
    GD.Tag(13);  GD.cmd_button(580, 170, 120, 60, 29, 0,  "A+");  GD.Tag(255);
    GD.Tag(14);  GD.cmd_button(580, 330, 120, 60, 29, 0,  "A-");  GD.Tag(255);

    if (GD.inputs.tag==11)
    {
      B=B-1;
      if(B<=1){B=1;}      
    }

    if (GD.inputs.tag==12)
    {
      B=B+1;
      if(B>=18){B=18;}
    }

    if (GD.inputs.tag==13)
    {
      A=A+10;
      if(A>=150){A=150;}
    }

    if (GD.inputs.tag==14)
    {
      A=A-10;
      if(A<=50){A=50;}
    }    
   }

  if(graph==5){
    GD.get_inputs();
    GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  COS();
    GD.ColorRGB(0xffffff); GD.Begin(POINTS);      GD.PointSize(1*16);  COS();
    
    GD.cmd_fgcolor(0x005000);
    GD.Tag(15);  GD.cmd_button(500, 250, 120, 60, 29, 0,  "B-");  GD.Tag(255);
    GD.Tag(16);  GD.cmd_button(660, 250, 120, 60, 29, 0,  "B+");  GD.Tag(255);
    GD.Tag(17);  GD.cmd_button(580, 170, 120, 60, 29, 0,  "A+");  GD.Tag(255);
    GD.Tag(18);  GD.cmd_button(580, 330, 120, 60, 29, 0,  "A-");  GD.Tag(255);

    if (GD.inputs.tag==15)
    {
      BC=BC-1;
      if(BC<=1){BC=1;}      
    }

    if (GD.inputs.tag==16)
    {
      BC=BC+1;
      if(BC>=18){BC=18;}
    }

    if (GD.inputs.tag==17)
    {
      AC=AC+10;
      if(AC>=150){AC=150;}
    }

    if (GD.inputs.tag==18)
    {
      AC=AC-10;
      if(AC<=50){AC=50;}
    }    
    
   }

  if(graph==6){
    
    //GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  TAN();
    GD.ColorRGB(0x50ffff); GD.Begin(POINTS);      GD.PointSize(1*16);  TAN();
    
    GD.cmd_fgcolor(0x500000);
    GD.Tag(19);  GD.cmd_button(500, 250, 120, 60, 29, 0,  "B-");  GD.Tag(255);
    GD.Tag(20);  GD.cmd_button(660, 250, 120, 60, 29, 0,  "B+");  GD.Tag(255);
    GD.Tag(21);  GD.cmd_button(580, 170, 120, 60, 29, 0,  "A+");  GD.Tag(255);
    GD.Tag(22);  GD.cmd_button(580, 330, 120, 60, 29, 0,  "A-");  GD.Tag(255);

    if (GD.inputs.tag==19)
    {
      BT=BT-1;
      if(BT<=1){BT=1;}      
    }

    if (GD.inputs.tag==20)
    {
      BT=BT+1;
      if(BT>=18){BT=18;}
    }

    if (GD.inputs.tag==21)
    {
      AT=AT+10;
      if(AT>=150){AT=150;}
    }

    if (GD.inputs.tag==22)
    {
      AT=AT-10;
      if(AT<=50){AT=50;}
    }    
   }
   
  if(graph==7){
    
    //GD.ColorRGB(0x50ff00); GD.Begin(LINE_STRIP);    GD.LineWidth(24);  GArray();
    GD.ColorRGB(0x00ff00); GD.Begin(POINTS);  GD.PointSize(1*16);  GArray();
    sprintf(TX,"esp = %d", esp);
    GD.Tag(23); GD.ColorRGB(0xffffff);  GD.cmd_button(500, 250, 120, 60, 29, 0,  TX);  GD.Tag(255);
    
  if (GD.inputs.tag==23)
    {
      delay(95);
      esp=esp+1;
      if(esp>=3){esp=0;}
    }
   }

  if (GD.inputs.tag==1)
    {
      graph=1;
    }

  if (GD.inputs.tag==2)
    {
      graph=2;
    }

  if (GD.inputs.tag==3)
    {
      graph=3;
    }

  if (GD.inputs.tag==4)
    {
      graph=4;
    }        

   if (GD.inputs.tag==5)
   {
      graph=5;
    }

   if (GD.inputs.tag==6)
   {
      graph=6;
    }

   if (GD.inputs.tag==7)
   {
      graph=7;
    }            

  GD.swap();
}

void Trace1()
{
  for (float x = 0; x < (PXMAX-10); x += 20) {
    float y = PYBase - ((0.001*x*(x)));
    GD.Vertex2f((x+PX0)*16, y*16);
  }  
}

void Trace1Random()
{
  for (float x = 0; x < (PXMAX-10); x += 20) {
    float y = PYBase - ((0.001*x*(x+random(-30,30))));
    GD.Vertex2f((x+PX0)*16, y*16);
  }  
}

void Trace2()
{
  for (float x = 0; x < (PXMAX-10); x += 20) {
    float y = PYBase - ((0.5*x));
    GD.Vertex2f((x+PX0)*16, y*16);
  }  
}

void Trace3()
{
  for (float x = 0; x < (PXMAX-10); x += 20) {
    float y = PYBase - ((0.5*x)-0.0008*x*x);
    GD.Vertex2f((x+PX0)*16, y*16);
  }  
}

void SIN(){
  for ( long  i = 0 ; i<=359 ; i++)
    {  
       GD.Vertex2f((i+PX0)*16, (PYBase-(A*sin((B*i*PI)/180)))*16);
     }
}

void COS(){
  for ( long  j = 0 ; j<=359 ; j++)
    {  
       GD.Vertex2f((j+PX0)*16, (PYBase-(AC*cos((BC*j*PI)/180)))*16);
     }
}

void TAN(){
  for ( long  j = 0 ; j<=359 ; j++)
    {  
       GD.Vertex2f((j+PX0)*16, (PYBase-(AT*tan((BT*j*PI)/180)))*16);
     }
}

void MarcoG()
{
  GD.Begin(LINES);
  GD.Vertex2f(PX0*16, PY0*16);    GD.Vertex2f(PX0*16, (PYMAX+170)*16);
  GD.Vertex2f(PX0*16, (PYMAX-4)*16);  GD.Vertex2f((PXMAX+PX0)*16, (PYMAX-4)*16);
}


void GArray(){
  for ( long  j = 0 ; j<=119 ; j++)
    {  
       GD.Vertex2f((j+PX0+esp*j)*16, (PYBase-(y_data[j]/10))*16);
     }
}
