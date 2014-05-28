int led_Data = A3;
int led_Latch = A2;
int led_Clock = A1;

int vol_slices = 360;
int vol_layers = 1;

byte serialData[360][2];

void setup()
{
  //Configure IO Pins
  pinMode(led_Data, OUTPUT);
  pinMode(led_Latch, OUTPUT);
  pinMode(led_Clock, OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  
}

void serialEvent() {
  for(int i=0; i<vol_slices; i++) {
   Serial.readBytes((char *)serialData[i], 2);
  } 
  
  for(int i=0; i<=vol_slices; i++) {
    Serial.print(serialData[i][0], BIN);
    Serial.print(" ");
    Serial.print(serialData[i][1], BIN);
    Serial.print("\n");
  }
}
