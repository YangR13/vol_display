#include <Servo.h>
Servo ESC;

int num_slices = 120;
int vol_layers = 15;
int onboard_num_layers = 5;

//Define pins;
int ESC_Out = 11;
int LED_sync = 3;

//Define variables;
int ESC_Arm = 10;
int ESC_speed = 87;

uint8_t bitcount, tierport;

void setup()
{
  pinMode(A0, OUTPUT);      //Outbound CLOCK pin;
  pinMode(A1, OUTPUT);      //Outbound Tier 1 pin;
  pinMode(A2, OUTPUT);      //Outbound Tier 2 pin;
  pinMode(A3, OUTPUT);      //Outbound Tier 3 pin;
  pinMode(LED_sync, OUTPUT);
  //Motor start and ramp up;
  ESC.attach(ESC_Out);
  ESC.write(ESC_Arm);
  delay(6000);              //Wait for ESC to arm;
  ESC.write(ESC_speed);     //Send speed to ESC;
  delay(20000);
  digitalWrite(LED_sync, HIGH);
  delay(15000);
  
  uint16_t SQUARE1[] = {0, 2, 5, 10, 20, 25, 27};
  uint16_t SQUARE2[] = {16,32,64,128,128,64,32};
  
  for(int i = 0; i < 4; i++)
  {
    for(int k = 0; k <7 ; k++)
    {
      update_onboard(SQUARE1[k] + 30*i, 2, SQUARE2[k]);
      update_onboard(SQUARE1[k] + 30*i, 10, SQUARE2[k]);
    }
    update_onboard(30*i, 3, 16);
    update_onboard(30*i, 4, 16);
    update_onboard(30*i, 5, 16);
    update_onboard(30*i, 6, 16);
    update_onboard(30*i, 7, 16);
    update_onboard(30*i, 8, 16);
    update_onboard(30*i, 9, 16);
  }
  
  uint16_t NOPE[][29] = {
    {
      1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1}
    ,
    {
      1,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0}
    ,
    {
      1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1}
    ,
    {
      1,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0}
    ,
    {
      1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1}
  };
  uint16_t T_T[5][19] = {
    {1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0},
  };
  /*
  for(int i = 0; i < 5; i++)
  {
    for(int k = 0; k < 19; k++)
    {
      if(T_T[i][k] == 1)
        update_onboard(70-1*k, i, 1);
    }
  }
  */
  uint16_t ECE199[5][40] = {
    {1,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1},
    {1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
  };
  /*
  for(int i = 0; i < 5; i++)
  {
    for(int k = 0; k < 40; k++)
    {
      if(ECE199[i][k] == 1)
        update_onboard(100-1*k, i, 1);
    }
  }
  /*
  for (uint8_t i = 0; i < vol_layers; i++)
   {
   update_onboard(0, i, 3);
   }
   
  for(int i = 0; i < 5; i++)
  {
    for(int k = 0; k < 26; k++)
    {
      if(NOPE[i][k] == 1)
        update_onboard(70-1*k, i, 1);
    }
  }
  */
  digitalWrite(LED_sync, HIGH);
  delay(2000); 
  //digitalWrite(LED_sync, LOW);
  //digitalWrite(LED_sync, LOW);
}

void loop()
{
}

void update_onboard(uint8_t timeslice, uint8_t layer, uint16_t newval) //Update coordinate of onboard arduino
{
  tierport = layer/onboard_num_layers + 1;         //Tier 0 is PC0, Tier 1 is PC1, Tier 2 is PC2, Clock is PC3; (Analog Pins);
  Ctransfer(16, newval);      //Transfer new value;
  Ctransfer(7, (num_slices - num_slices/8*(layer%8) + timeslice)%num_slices);//num_slices - num_slices/8*(layer%8) + timeslice);    //Transfer timeslice coordinate with offsets;
  Ctransfer(3, layer%onboard_num_layers);        //Transfer layer coordinate;
  delay(40);
}

void Ctransfer(uint8_t bitlength, uint16_t val)
{
  for(bitcount = 0; bitcount < bitlength; bitcount++)  //Shift out new value stored value;
  {
    if(bitRead(val, bitcount))
      PORTC |= (1 << 0);
    else
      PORTC &= ~(1 << 0);
      delayMicroseconds(4);
    PORTC &= ~(1 << tierport);
    PORTC |= (1 << tierport);
    delayMicroseconds(16);
  }
}
