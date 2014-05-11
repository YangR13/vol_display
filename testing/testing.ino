int led_Data = A1;
int led_Latch = A2;
int led_Clock = A3;

int vol_slices = 360;
int vol_layers = 1;

int i, j;
char *serialData[360];

void setup()
{
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  
  for(i=0; i<360; i++) {
    serialData[i] = (char *) malloc(vol_layers*2*sizeof(char));
  }
  
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available() > 0) {
    for(i=0; i<360; i++) {
      for(j=0; j<vol_layers*2; i++) {
        serialData[i][j] = Serial.read();
      } 
    }
  }

  for(i=0; i<360; i++) {
    digitalWrite(led_Latch, LOW);
    for(j=0; j<vol_layers*2; i++) {
      shiftOut(led_Data, led_Clock, MSBFIRST, serialData[i][j]);
    }
    digitalWrite(led_Latch, HIGH);
    delay(1000);
  }
}
