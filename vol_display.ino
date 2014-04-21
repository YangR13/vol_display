#include <Servo.h>

Servo esc;

//Define pins;
int led_Data = A0;          //A# (0-5) = Analog pins
int led_Latch = A1;
int led_Clock = A2;
int RPM_check = A5;
int esc_Out = 9;


//Define variables;
int esc_Arm = 10;
int HDD_lowbound = 75;
int HDD_highbound = 165;
int RPM_target = 1000;      //Target RPM

int vol_slices = 360;       //# of separate radial slices that will be plexed through;
int vol_layers = 16;        //# of vertical layers;
int vol_shiftsperlayer = 4; //# of 8-bit shift registers per layer;
int vol_totalbytes = vol_shiftsperlayer*vol_layers;


void setup()
{
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  pinMode(RPM_check, INPUT);
  
  //Motor start and ramp up;
  esc.attach(esc_Out);
  esc.write(esc_Arm);
  delay(2000);
  
  
  
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
