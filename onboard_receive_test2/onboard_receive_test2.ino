uint8_t bitcount = 0;    //Ticker for CshiftOut function;

volatile uint8_t bit_in_count = 0;
volatile uint16_t inputID;
volatile uint16_t timeslice;
volatile uint16_t layer;
volatile uint16_t newval;
//Function Prototypes
void CshiftOut(uint16_t);  //WARNING: HARDCODED PINS
void Creceive();           //WARNING: HARDCODED PINS AND VARIABLES

void setup()
{
  pinMode(A3, INPUT);   //Clock input, PC3
  Serial.begin(9600);
  attachInterrupt(0, Creceive, RISING); //Interrupt when data pin goes from low to high;
}

void loop()
{

}

void Creceive()
{
  if(bit_in_count < 7)
  {
    bitWrite(timeslice, bit_in_count, (PINC & (1 << 3)));
    bit_in_count++;
    return;
  }
  else if(bit_in_count < 10)
  {
    bitWrite(layer, (bit_in_count - 7), (PINC & (1 << 3)));
    bit_in_count++;
    return;
  }
  else if(bit_in_count < 25)
  {
    bitWrite(newval, (bit_in_count - 10), (PINC & (1 << 3)));
    bit_in_count++;
    return;
  }
  else
  {
    bitWrite(newval, (bit_in_count - 10), (PINC & (1 << 3)));
    bit_in_count = 0;
    Serial.print(timeslice);
    Serial.print(" ");
    Serial.print(layer);
    Serial.print(" ");
    Serial.println(newval);
    timeslice = 0;
    layer = 0;
    newval = 0;
  }
}

void CshiftOut(uint16_t val)
{
  for(bitcount = 0; bitcount < 16; bitcount++)
  {
    if(bitRead(val, bitcount))
      PORTC |= (1 << 2);
    else
      PORTC &= ~(1 << 2);
    PORTC |= (1 << 0);
    PORTC &= ~(1 << 0);
  }
}








