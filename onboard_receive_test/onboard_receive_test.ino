
int RPM_check;       //Photoresistor pin: 

int ESC_tol = 1000;  //Maximum tolerated variation in rotational period in micros();
uint8_t bitcount = 0;    //Ticker for CshiftOut function;

int delay_overhead;  //Time it takes to run output all data;
int delay_degree;    //Time needed to wait between each slice;

//Function Prototypes
void CshiftOut(uint16_t);  //WARNING: HARDCODED PINS
int ESC_getPeriod();       //Returns the time required for a certain number of rotations.
boolean notconstant();     //Checks if angular speed has stabilized.

void setup()
{
  int led_Data = A2;   //Data pin: ANALOG2;    PORTC 2
  int led_Latch = A1;  //Latch pin: ANALOG1;   PORTC 1
  int led_Clock = A0;  //Clock pin: ANALOG0;   PORTC 0
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  pinMode(RPM_check, INPUT);
}

void loop()
{

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

int ESC_getPeriod()
{
  int count, current, start = micros();
  int same = 0;
  for(count = 1; count <= 1000;){
    if(digitalRead(RPM_check) && same == 0){
      count++; 
      same = 1;
    }
    else if(gatePort != RPM_trip && same == 1)
      same = 0;
  }
  current = micros();
  return current - previous;
}

boolean notconstant()
{
  int delta = 10000;
  int samp1 = ESC_getperiod();
  delta = ESC_getperiod() - samp1;
  if(delta <= ESC_tol)
    return false;
  else
    return true;
}



