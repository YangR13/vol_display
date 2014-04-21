#include <Servo.h>

Servo esc;

//Define pins;
int led_Data = A0;     //A# (0-5) = Analog pins
int led_Latch = A1;
int led_Clock = A2;
int _RPM = A5;
int esc_Out = 9;

//Define variables;
int esc_Arm = 10;

void setup()
{
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  pinMode(_RPM, INPUT);
  esc.attach(esc_Out);
  esc.write(esc_Arm);
  
  
  
  int maxTicks = 1000;
  int tick = 0;
  
  int *snpstArr;
  snpstArr = (int *)malloc(maxTicks*sizeof(int));
  memset(snpstArr, -1, maxTicks);
  
  //fill array with proper values
}

void loop()
{
  
  
  
  tick++;
  if(tick > maxTicks) tick = 0;
  
  //delay
}
