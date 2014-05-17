#include <Servo.h>

Servo ESC;

//Define pins;
int ESC_Out = 11;


//Define variables;
int ESC_Arm = 10;
int ESC_lowbound = 80;
int ESC_highbound = 165;
int ESC_speed = 90;

int vol_slices = 360;       //# of separate radial slices that will be plexed through;
int vol_layers = 16;        //# of vertical layers;
int vol_shiftsperlayer = 4; //# of 8-bit shift registers per layer;

void setup()
{

  
  //Motor start and ramp up;
  esc.attach(ESC_Out);
  esc.write(ESC_Arm);
  delay(5000);
  //Slowly ramp up speed;
  for(i = ESC_lowbound; i <= ESC_highbound && i <= ESC_speed; i++)
  {
    esc.write(i);
    delay(delay_iniESC);
  }
  //Read overhead delay time;
  
  //Read rpm and calculate rotation period;  
  while(notconstant());
  
  
  int maxTicks = 1000;
  int tick = 0;
  
  int *snpstArr;
  snpstArr = (int *)malloc(maxTicks*sizeof(int));
  memset(snpstArr, -1, maxTicks);
  
  //fill array with proper values
}

void loop()
{
  //Check for incoming data;
  
  //Cycle through arrays and shiftout data;
  for()
  {
    //Cycle through radial slices;
    for()
    {
      //Cycle and shift out all registers
    }
    
    //Check for and break if at rotation completion; Calculate period;
  }
  
  
  tick++;
  if(tick > maxTicks) tick = 0;
  
  //delay
}


