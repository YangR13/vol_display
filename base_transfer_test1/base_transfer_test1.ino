int vol_slices = 100;       //# of separate radial slices that will be plexed through;
int vol_layers = 16;        //# of vertical layers;
int vol_shiftsperlayer = 4; //# of 8-bit shift registers per layer;
uint8_t tierport = 1;

uint8_t time = 100;
uint8_t lay = 3;
uint16_t input = 500;

void setup()
{
  pinMode(A0, OUTPUT);      //Outbound CLOCK pin;
  pinMode(A1, OUTPUT);      //Outbound Tier 1 pin;
  pinMode(A2, OUTPUT);      //Outbound Tier 2 pin;
  pinMode(A3, OUTPUT);      //Outbound Tier 3 pin;
  delay(10000);
}

void loop()
{
  //PORTC &= ~(1 << tierport);
  //PORTC |= (1 << tierport);     //Write the port HIGH to let onboard know that data is incoming;
  update_onboard(time, lay, input);
  delay(2000);
}


uint8_t bitcount;
void update_onboard(uint8_t timeslice, uint8_t layer, uint16_t newval) //Update coordinate of onboard arduino
{
  //tierport = layer/5;           //Tier 0 is PC0, Tier 1 is PC1, Tier 2 is PC2, Clock is PC3; (Analog Pins);
  PORTC &= ~(1 << tierport);
  PORTC |= (1 << tierport);
  Ctransfer(2, 1);              //Transfer 2-bit identifier code;
  delayMicroseconds(100);
  Ctransfer(7, timeslice);      //Transfer timeslice coordinate;
  delayMicroseconds(100);
  Ctransfer(3, layer);          //Transfer layer coordinate;
  delayMicroseconds(100);
  Ctransfer(16, newval);
}

void Ctransfer(uint8_t bitlength, uint16_t val)
{
  for(bitcount = 0; bitcount < bitlength; bitcount++)  //Shift out new value stored value;
  {
    if(bitRead(val, bitcount) == 1)
      PORTC |= (1 << tierport);
    else
      PORTC &= ~(1 << tierport);
    PORTC |= (1 << 0);
    PORTC &= ~(1 << 0);
    delayMicroseconds(10);
  }
}


