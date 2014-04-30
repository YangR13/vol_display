#include <Servo.h>

Servo ESC;

//Define pins;
int led_Data = A1;          //A# (0-5) = Analog pins
int led_Latch = A2;
int led_Clock = A3;
int RPM_check = A0;
int ESC_Out = 11;


//Define variables;
int ESC_Arm = 10;
int ESC_lowbound = 80;
//int ESC_highbound = 165;
//int RPM_target = 1000;      //Target RPM
int ESC_speed = 100;
int delay_iniESC = 2000;

//Helper setting variables;
int delay_overhead;
int delay_degree;

int vol_slices = 360;       //# of separate radial slices that will be plexed through;
int vol_layers = 16;        //# of vertical layers;
int vol_shiftsperlayer = 4; //# of 8-bit shift registers per layer;

void setup()
{
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  pinMode(RPM_check, INPUT);
  
  //Motor start and ramp up;
  esc.attach(ESC_Out);
  esc.write(ESC_Arm);
  delay(5000);
  //Slowly ramp up speed;
  for(i = ESC_lowbound; i <= ESC_highbound && i <= target; i++)
  {
    esc.write(i);
    delay(delay_iniESC);
  }
  
  //Read overhead delay time;
  
  
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
