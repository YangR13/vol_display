#include <Servo.h>

Servo ESC;

//Define pins;
int led_Data = A1;          //A# (0-5) = Analog pins
int led_Latch = A2;
int led_Clock = A3;
int RPM_check = 7;
int ESC_Out = 11;


//Define variables;
int ESC_Arm = 10;
int ESC_lowbound = 80;
int ESC_highbound = 165;
int ESC_speed = 90;
int delay_iniESC = 2000;
int ESC_tol = 1000;          //Maximum tolerated variation in rotational period in micros();
int RPM_trip = 1;

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

void shiftout_degree(int **cur_degree) const
{
  
}

int ESC_getperiod()
{
  int count, current, start = micros();
  int same = 0;
  for(count = 1; count <= 1000;)
  {
    if(digitalRead(RPM_check) == RPM_trip && same == 0)
    {
      count++;
      same = 1;
    }
    else if(gatePort) != RPM_trip && same == 1)
    {
      same = 0;
    }
  }
  current = micros();
  return current - previous;
}

boolean notconstant()
{
  int delta = 100000;
  int samp1 = ESC_getperiod();
  delta = ESC_getperiod() - samp1;
  if(delta <= ESC_tol)
  {
    return false;
  }
  else
  {
    return true;
  }
}
