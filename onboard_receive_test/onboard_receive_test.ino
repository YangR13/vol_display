uint8_t bitcount = 0;    //Ticker for CshiftOut function;

volatile uint8_t bit_in_count = 0;
volatile uint16_t inputID;
volatile uint16_t timeslice;
volatile uint16_t layer;
volatile uint16_t newval;
volatile uint16_t temp;
//Function Prototypes
void CshiftOut(uint16_t);  //WARNING: HARDCODED PINS

void setup()
{
  pinMode(2, INPUT);    //Data input, PD2
  pinMode(A3, INPUT);   //Clock input, PC3
  Serial.begin(9600);
  attachInterrupt(0, receive_update, RISING); //Interrupt when data pin goes from low to high;
}

void loop()
{

}

void lemmeknow()
{
  Serial.println("HAI THERE");
}

void receive_update()
{
  inputID = Creceive(2);
  timeslice = Creceive(7);
  layer = Creceive(3);
  newval = Creceive(16);
  Serial.print(inputID);
  Serial.print(', ');
  Serial.print(timeslice);
  Serial.print(', ');
  Serial.print(layer);
  Serial.print(', ');
  Serial.println(newval);
}

uint16_t Creceive(uint8_t bitlength)
{
  bit_in_count = 0;
  temp = 0;
  while(bit_in_count < bitlength)
  {
    if(PINC & (1 << 3)) //if CLOCK pin is HIGH;
    {
      bitWrite(temp, bit_in_count, (PIND & (1 << 2)));
      bit_in_count++;
    }
  }
  return temp;
}

void CshiftOut(uint16_t val)
{
  for(bitcount = 0; bitcount < 16; bitcount++)
  {
    if(bitRead(val, bitcount) == 1)
      PORTC |= (1 << 2);
    else
      PORTC &= ~(1 << 2);
    PORTC |= (1 << 0);
    PORTC &= ~(1 << 0);
  }
}




