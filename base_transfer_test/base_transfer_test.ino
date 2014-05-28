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
  pinMode(A0, OUTPUT);      //Outbound CLOCK pin;
  pinMode(A1, OUTPUT);      //Outbound Tier 1 pin;
  pinMode(A2, OUTPUT);      //Outbound Tier 2 pin;
  pinMode(A3, OUTPUT);      //Outbound Tier 3 pin;
  //Motor start and ramp up;
  ESC.attach(ESC_Out);
  ESC.write(ESC_Arm);
  delay(5000);
  //Slowly ramp up speed;
  ESC.write(ESC_speed);
  //Read overhead delay time;

  //Read rpm and calculate rotation period;


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


  //tick++;
  //if(tick > maxTicks) tick = 0;
  //delay
}


uint8_t tierport, bitcount;

void update_onboard(uint8_t timeslice, uint8_t layer, uint8_t val) //Update coordinate of onboard arduino
{
  tierport = layer/5;           //Tier 0 is PC0, Tier 1 is PC1, Tier 2 is PC2, Clock is PC3; (Analog Pins);
  PORTC |= (1 << tierport);     //Write the port HIGH to let onboard know that data is incoming;
  
  PORTC |= (1 << tierport);     //Pulse Clock pin to begin data transfer;
  PORTC &= ~(1 << tierport);
  
  Ctransfer(2, 1);              //Transfer 2-bit identifier code;
  //delayMicroseconds();
  Ctransfer(7, timeslice);      //Transfer timeslice coordinate;
  //delayMicroseconds();
  Ctransfer(3, layer);          //Transfer layer coordinate;
}

void Ctransfer(uint8_t bitlength, void val)
{
  for(bitcount = 0; bitcount < bitlength; bitcount++)  //Shift out new value stored value;
  {
    if(bitRead(val, bitcount) == 1)
      PORTC |= (1 << tierport);
    else
      PORTC &= ~(1 << tierport);
    PORTC |= (1 << tierport);
    PORTC &= ~(1 << tierport);
  }
}


