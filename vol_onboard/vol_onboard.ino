int num_slices = 120;
int num_half = num_slices/2;
int num_layers = 5;
int ESC_tol = 10000;  //Maximum tolerated variation in rotational period in micros();

uint16_t delay_overhead;  //Time it takes to run output all data;
uint16_t delay_degree = 1;    //Time needed to wait between each shiftout;
uint16_t delay_slice;     //Time needed for each slice; (angular speed)

uint8_t bitcount = 0;                //Ticker for CshiftOut function;
volatile uint8_t bit_in_count = 0;   //Ticker for data receiving;
volatile uint8_t current_slice;      //Ticker for shifting out data;
volatile uint8_t opposite_slice;
int current_layer;

volatile uint8_t timeslice;          //Global storage variable for new data coordinate;
volatile uint8_t layer;              //Global Storage variable for new data coordinate;
volatile uint16_t newval;            //Global Storage variable for new data value;

volatile uint16_t data_array[120][5];    //Initialize data array with all zeros.
//Format:    data_array[timeslice][layer] = {data_value @ location};

boolean store;
volatile long ESC_period;

//Function Prototypes
void slice_reset();        //Resets slice count after each rotation. Interrupt Routine;
void Creceive();           //WARNING: HARDCODED PINS; Receives coordinate updates. Interrupt Routine;
void CShiftOut(uint16_t);  //WARNING: HARDCODED PINS;
void ESC_getPeriod();       //Returns the time required for a certain number of rotations;
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
  for(int k = 0; k < num_slices; k++)
  {
    for(int i = 0; i < num_layers; i++)
      data_array[k][i] = 1;
  }

  //Time the loop() function to determine overhead;
  volatile long period;
  volatile long current = micros();
  //Serial.begin(9600);
  loop();

  for(int k = 0; k < num_slices; k++)
  {
    for(int i = 0; i < num_layers; i++)
      data_array[k][i] = 0;
  }
  for(int k = 0; k < 100; k++)
    loop();
  period = micros() - current;
  delay_overhead = period/100;
  //Serial.println(delay_overhead);
  //RPM Sensing and delay calculations;
  int store = bitRead(PIND, 3);
  while(bitRead(PIND,3) == store);
  //while(RPM_notconstant());            //Wait until RPM is ~constant;
  //Serial.println(ESC_getPeriod());
  attachInterrupt(1, ESC_getPeriod, RISING);
  delay(10000);
  delay_slice = ESC_period/100;
  delay_degree = 8 + (delay_slice - delay_overhead)/num_slices; //# Microseconds;

  /*
  for(int i = 0; i < num_layers; i++)
   data_array[0][i] = 3;
   */
  if(delay_degree <= 1)     //Prevent negative or 0 values;
    delay_degree = 1;

  attachInterrupt(0, Creceive, RISING);    //Interrupt to pulse clock and input data;
  //attachInterrupt(1, reset, RISING); //Interrupt to reset at each rotation;
}

void loop()
{
  for(current_slice = 0, opposite_slice = num_half; current_slice < num_half; current_slice++, opposite_slice++)
  {
    store = bitRead(PIND, 3);
    PORTC &= ~(1 << 1);   //Begin sending in new data;
    CShiftOut(data_array[opposite_slice][4]);
    CShiftOut(data_array[opposite_slice][3]);
    CShiftOut(data_array[opposite_slice][2]);
    CShiftOut(data_array[opposite_slice][1]);
    CShiftOut(data_array[opposite_slice][0]);
    CShiftOut(data_array[current_slice][0]);
    CShiftOut(data_array[current_slice][1]);
    CShiftOut(data_array[current_slice][2]);
    CShiftOut(data_array[current_slice][3]);
    CShiftOut(data_array[current_slice][4]);
    delayMicroseconds(delay_degree);
    PORTC |= (1 << 1);    //Display all new data;
    if(bitRead(PIND, 3) && store == false)
      return;
  }
  for(opposite_slice = 0; current_slice < num_slices; current_slice++, opposite_slice++)
  {
    store = bitRead(PIND, 3);
    PORTC &= ~(1 << 1);   //Begin sending in new data;
    CShiftOut(data_array[opposite_slice][4]);
    CShiftOut(data_array[opposite_slice][3]);
    CShiftOut(data_array[opposite_slice][2]);
    CShiftOut(data_array[opposite_slice][1]);
    CShiftOut(data_array[opposite_slice][0]);
    CShiftOut(data_array[current_slice][0]);
    CShiftOut(data_array[current_slice][1]);
    CShiftOut(data_array[current_slice][2]);
    CShiftOut(data_array[current_slice][3]);
    CShiftOut(data_array[current_slice][4]);
    delayMicroseconds(delay_degree);
    PORTC |= (1 << 1);    //Display all new data;
    if(bitRead(PIND, 3) && store == false)
      return;
  }
}

void reset()
{
  current_slice = 104;    //Kicks current_slice to the end, breaking all display loops.
}

void Creceive()
{
  if(bit_in_count < 16)           //Write to updated value;
  {
    bitWrite(newval, bit_in_count, bitRead(PINC, 3));
    bit_in_count++;
    return;
  }
  else if(bit_in_count < 23)     //Write to slice coordinate;
  {
    bitWrite(timeslice, (bit_in_count - 16), bitRead(PINC, 3));
    bit_in_count++;
    return;
  }
  else if(bit_in_count < 25)     //Write to layer coordinate;
  {
    bitWrite(layer, (bit_in_count - 23), bitRead(PINC, 3));
    bit_in_count++;
    return;
  }
  else
  {
    bitWrite(layer, (bit_in_count - 23), bitRead(PINC, 3)); //Finish writing to layer;
    bit_in_count = 0;

    //Update data_array;
    data_array[timeslice][layer] = newval;
    /*
    Serial.print(timeslice);
     Serial.print(" ");
     Serial.print(layer);
     Serial.print(" ");
     Serial.println(newval);
     */
  }
}

void CShiftOut(uint16_t val)
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

volatile uint8_t count_RPM = 0;
volatile long current;

void ESC_getPeriod()
{
  if(count_RPM == 0)
    current = micros();
  if(count_RPM < 100)
    count_RPM++;
  else
  {
    ESC_period = micros() - current;
    detachInterrupt(1);
  }
}

/*
boolean RPM_notconstant()
 {
 uint32_t delta;
 uint32_t samp1 = ESC_getPeriod();
 delta = ESC_getPeriod() - samp1;
 return (delta >= ESC_tol);
 }
 */


