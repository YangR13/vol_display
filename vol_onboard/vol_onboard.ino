int led_Data = A2;          //A# (0-5) = Analog pins
int led_Latch = A1;
int led_Clock = A0;
int RPM_check;

int delay_iniESC = 2000;
int ESC_tol = 1000;          //Maximum tolerated variation in rotational period in micros();
int RPM_trip = 1;

//Helper setting variables;
int delay_overhead;
int delay_degree;

void setup()
{
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  pinMode(RPM_check, INPUT);
}

void loop()
{
  
}

int ESC_getperiod()
{
  int count, current, start = micros();
  int same = 0;
  for(count = 1; count <= 1000;)
  {
    if(digitalRead(RPM_check) == RPM_trip && same == 0)
    {
      count++;
      same = 1;
    }
    else if(gatePort) != RPM_trip && same == 1)
    {
      same = 0;
    }
  }
  current = micros();
  return current - previous;
}

boolean notconstant()
{
  int delta = 100000;
  int samp1 = ESC_getperiod();
  delta = ESC_getperiod() - samp1;
  if(delta <= ESC_tol)
    return false;
  else
    return true;
}
