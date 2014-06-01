#include <Servo.h>
Servo ESC;

const int vol_slices = 104;
const int vol_layers = 15;
const int onboard_num_layers = 5;

typedef struct{
  uint16_t data;
  int slice;
  int layer;
} 
Spoke;

Spoke SpokeArr[vol_slices];

//Define pins;
int ESC_Out = 11;

//Define variables;
int ESC_Arm = 10;
int ESC_speed = 90;

uint8_t bitcount, tierport;

void setup()
{
  pinMode(A0, OUTPUT);      //Outbound CLOCK pin;
  pinMode(A1, OUTPUT);      //Outbound Tier 1 pin;
  pinMode(A2, OUTPUT);      //Outbound Tier 2 pin;
  pinMode(A3, OUTPUT);      //Outbound Tier 3 pin;
  //Motor start and ramp up;
  ESC.attach(ESC_Out);
  ESC.write(ESC_Arm);
  delay(5000);              //Wait for ESC to arm;
  ESC.write(ESC_speed);     //Send speed to ESC;
  Serial.begin(9600);
}

void loop()
{
  //Check for incoming data;
}

void update_onboard(uint8_t timeslice, uint8_t layer, uint16_t newval) //Update coordinate of onboard arduino
{
  tierport = layer/onboard_num_layers;         //Tier 0 is PC0, Tier 1 is PC1, Tier 2 is PC2, Clock is PC3; (Analog Pins);
  Ctransfer(16, newval);      //Transfer new value;
  Ctransfer(7, (timeslice + 25*layer%4)%vol_slices);    //Transfer timeslice coordinate with offsets;
  Ctransfer(3, layer%onboard_num_layers);        //Transfer layer coordinate;
}

void Ctransfer(uint8_t bitlength, uint16_t val)
{
  for(bitcount = 0; bitcount < bitlength; bitcount++)  //Shift out new value stored value;
  {
    if(bitRead(val, bitcount))
      PORTC |= (1 << 0);
    else
      PORTC &= ~(1 << 0);
    PORTC &= ~(1 << tierport);
    PORTC |= (1 << tierport);
    delayMicroseconds(8);
  }
}

void serialEvent() 
{
  int packageSize = 0;
  int numSpokes = 0;
  int currentLayer = 0;
  byte serialBuffer[4];

  Serial.print("serialEvent triggered!\n");

  while(true) 
  {
    packageSize = Serial.readBytes((char *)serialBuffer, 4);
    if(packageSize == 0) break;

    if(serialBuffer[3] != currentLayer) 
    {
      printLayer(numSpokes);
      sendLayer(currentLayer, &numSpokes);
      currentLayer = serialBuffer[3]; 
    }
    Serial.print(numSpokes);
    SpokeArr[numSpokes].data = (uint16_t) serialBuffer[0] | ((uint16_t) serialBuffer[1] << 8);
    SpokeArr[numSpokes].slice = serialBuffer[2];
    SpokeArr[numSpokes].layer = serialBuffer[3];
    numSpokes++;
  }
  printLayer(numSpokes);
  sendLayer(currentLayer, &numSpokes);

  Serial.print("Reached end of data scan\n");
}

void printLayer(int numSpokes) 
{
  Serial.print("Number of spokes stored: ");
  Serial.print(numSpokes);
  Serial.print("\n");

  for(int i=0; i<numSpokes; i++) {
    Serial.print("Data: ");
    Serial.print(SpokeArr[i].data, BIN);
    Serial.print("\nSlice: ");
    Serial.print(SpokeArr[i].slice);
    Serial.print("\nLayer: ");
    Serial.print(SpokeArr[i].layer);
    Serial.print("\n");
  }
}

void sendLayer(int currentLayer, int *numSpokes) 
{
  Serial.print("Sending layer: ");
  Serial.print(currentLayer);
  Serial.print("\n");
  //for loop sending Spokes to Bareduinos
  Serial.print("Wiping SpokeArr\n");
  for(int i; i<vol_slices; i++) 
  {
    if(SpokeArr[i].data != 0)
      update_onboard(SpokeArr[i].slice, SpokeArr[i].layer, SpokeArr[i].data);
    SpokeArr[i].data = 0;
    SpokeArr[i].slice = 0;
    SpokeArr[i].layer = 0;
  }
  *numSpokes = 0;
}

