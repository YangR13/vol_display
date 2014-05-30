#include <Servo.h>
Servo ESC;

//Define pins;
int ESC_Out = 11;

//Define variables;
int ESC_Arm = 10;
int ESC_speed = 90;

uint8_t bitcount, tierport;

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
  //Fill array with proper values;
}

void loop()
{
  //Check for incoming data;
}

void update_onboard(uint8_t timeslice, uint8_t layer, uint16_t newval) //Update coordinate of onboard arduino
{
  tierport = layer/5;         //Tier 0 is PC0, Tier 1 is PC1, Tier 2 is PC2, Clock is PC3; (Analog Pins);
  Ctransfer(16, newval);      //Transfer new value;
  Ctransfer(7, timeslice);    //Transfer timeslice coordinate;
  Ctransfer(3, layer);        //Transfer layer coordinate;
}

void Ctransfer(uint8_t bitlength, uint16_t val)
{
  for(bitcount = 0; bitcount < bitlength; bitcount++)  //Shift out new value stored value;
  {
    if(bitRead(val, bitcount))
      PORTC |= (1 << 0);
    else
      PORTC &= ~(1 << 0);
    PORTC &= ~(1 << tierport);
    PORTC |= (1 << tierport);
    delayMicroseconds(8);
  }
}
