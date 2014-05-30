int ESC_tol = 1000;  //Maximum tolerated variation in rotational period in micros();

uint16_t delay_overhead;  //Time it takes to run output all data;
uint16_t delay_degree;    //Time needed to wait between each shiftout;
uint16_t delay_slice;     //Time needed for each slice; (angular speed)

uint8_t bitcount = 0;                //Ticker for CshiftOut function;
volatile uint8_t bit_in_count = 0;   //Ticker for data receiving;
volatile uint8_t current_slice;      //Ticker for shifting out data;
volatile uint8_t current_layer;      //Ticker for shifting out data;

volatile uint8_t timeslice;          //Global storage variable for new data coordinate;
volatile uint8_t layer;              //Global Storage variable for new data coordinate;
volatile uint16_t newval;            //Global Storage variable for new data value;

volatile uint16_t data_array[100][5] = {
  0};    //Initialize data array with all zeros.
//Format:    data_array[timeslice][layer] = {data_value @ location};


//Function Prototypes
void slice_reset();        //Resets slice count after each rotation. Interrupt Routine;
void Creceive();           //WARNING: HARDCODED PINS; Receives coordinate updates. Interrupt Routine;
void CShiftOut(uint16_t);  //WARNING: HARDCODED PINS;
int ESC_getPeriod();       //Returns the time required for a certain number of rotations;
boolean RPM_notconstant();     //Checks if angular speed has stabilized;

void setup()
{
  int led_Data = A2;   //Data pin: ANALOG2;    PORTC 2
  int led_Latch = A1;  //Latch pin: ANALOG1;   PORTC 1
  int led_Clock = A0;  //Clock pin: ANALOG0;   PORTC 0
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);

  pinMode(A3, INPUT);  //Transfer Data Input;
  pinMode(2, INPUT);   //Interrupt for data receive;
  pinMode(3, INPUT);   //Interrupt for rotation reset;

  //Time the loop() function to determine overhead;
  uint32_t k, period;
  uint32_t current = micros();
  for(k = 0; k < 10000; k++)
    loop();
  period = micros() - current;
  delay_overhead = period/10000;

  //RPM Sensing and delay calculations;
  while(RPM_notconstant());    //Wait until RPM is ~constant;
  delay_slice = ESC_getPeriod()/1000;
  
  delay_degree = (delay_overhead + delay_slice)/100; //# Microseconds;

  attachInterrupt(0, Creceive, RISING);    //Interrupt to pulse clock and input data;
  attachInterrupt(1, reset, RISING); //Interrupt to reset at each rotation;
}

void loop()
{
  for(current_slice = 0; current_slice < 100; current_slice++)
  {
    PORTC &= ~(1 << 1);   //Begin sending in new data;
    if(current_slice < 50)
    {
      for(current_layer = 0; current_layer <= 4; current_layer++)
        CShiftOut(data_array[current_slice][current_layer]);      //Shiftout far side; Bottom to top;
      for(current_layer = 4; current_layer >= 0; current_layer--)
        CShiftOut(data_array[current_slice + 50][current_layer]);      //Shiftout near side; Top to bottom;
    }
    else
    {
      for(current_layer = 0; current_layer <= 4; current_layer++)
        CShiftOut(data_array[current_slice][current_layer]);      //Shiftout far side; Bottom to top;
      for(current_layer = 4; current_layer >= 0; current_layer--)
        CShiftOut(data_array[current_slice - 50][current_layer]);      //Shiftout near side; Top to bottom;
    }
    delayMicroseconds(delay_degree);
    PORTC |= (1 << 1);    //Display all new data;
  }
}

void reset()
{
  current_slice = 0;
}

void Creceive()
{
  if(bit_in_count < 16)           //Write to updated value;
  {
    bitWrite(newval, bit_in_count, (PINC & (1 << 3)));
    bit_in_count++;
    return;
  }
  else if(bit_in_count < 23)     //Write to slice coordinate;
  {
    bitWrite(timeslice, (bit_in_count - 7), (PINC & (1 << 3)));
    bit_in_count++;
    return;
  }
  else if(bit_in_count < 25)     //Write to layer coordinate;
  {
    bitWrite(layer, (bit_in_count - 10), (PINC & (1 << 3)));
    bit_in_count++;
    return;
  }
  else
  {
    bitWrite(layer, (bit_in_count - 10), (PINC & (1 << 3))); //Finish writing to layer;
    bit_in_count = 0;

    //Update data_array;
    data_array[timeslice][layer] = newval;

    //timeslice = 0;  These "clear variables" theoretically shouldn't be needed.
    //layer = 0;
    //newval = 0;
  }
}

void CShiftOut(uint16_t val)
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

int ESC_getPeriod()
{
  uint32_t count, current, previous = micros();
  boolean same = false;
  for(count = 0; count < 1000;)
  {
    if((PIND & (1 << 3)) && same){
      count++; 
      same = true;
    }
    else if(!(PIND & (1 << 3)) && same)
      same = false;
  }
  current = micros();
  return current - previous;
}

boolean RPM_notconstant()
{
  int delta;
  int samp1 = ESC_getPeriod();
  delta = ESC_getPeriod() - samp1;
  return (delta >= ESC_tol);
}
