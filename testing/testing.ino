int led_Data = A3;
int led_Latch = A2;
int led_Clock = A1;

const int vol_slices = 360;
const int vol_layers = 1;

uint16_t serialData[vol_slices][vol_layers];

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
    for(int j=0; j<vol_layers; j++) {
     byte serialBuffer[2];
     Serial.readBytes((char *)serialBuffer, 2);
     serialData[i][j] = (uint16_t) serialBuffer[0] | ((uint16_t) serialBuffer[1] << 8);
    } 
  }
  
  for(int i=0; i<=vol_slices; i++) {
    for(int j=0; j<vol_layers; j++) {
      Serial.print(serialData[i][j], BIN);
      Serial.print("\n");
    }
  }
}
