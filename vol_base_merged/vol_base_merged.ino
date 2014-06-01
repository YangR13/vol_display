#include <Servo.h>
Servo ESC;

const int vol_slices = 104;
const int vol_layers = 15;
const int onboard_num_layers = 5;
int LED_sync = 3;

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
int ESC_speed = 87;

uint8_t bitcount, tierport;

void setup()
{
  pinMode(A0, OUTPUT);      //Outbound CLOCK pin;
  pinMode(A1, OUTPUT);      //Outbound Tier 1 pin;
  pinMode(A2, OUTPUT);      //Outbound Tier 2 pin;
  pinMode(A3, OUTPUT);      //Outbound Tier 3 pin;
  pinMode(3, OUTPUT);
  //Motor start and ramp up;
  ESC.attach(ESC_Out);
  ESC.write(ESC_Arm);
  delay(5000);              //Wait for ESC to arm;
  ESC.write(ESC_speed);     //Send speed to ESC;
  delay(20000);
  digitalWrite(LED_sync, HIGH);
  delay(15000);
  
  uint16_t SQUARE1[] = {0, 2, 5, 10, 20, 25, 27};
  uint16_t SQUARE2[] = {16,32,64,128,128,64,32};
  
  for(int i = 0; i < 4; i++)
  {
    for(int k = 0; k <7 ; k++)
    {
      update_onboard(SQUARE1[k] + 30*i, 2, SQUARE2[k]);
      update_onboard(SQUARE1[k] + 30*i, 10, SQUARE2[k]);
    }
    update_onboard(30*i, 3, 16);
    update_onboard(30*i, 4, 16);
    update_onboard(30*i, 5, 16);
    update_onboard(30*i, 6, 16);
    update_onboard(30*i, 7, 16);
    update_onboard(30*i, 8, 16);
    update_onboard(30*i, 9, 16);
  }
  display_realign();
  delay(10000);
  for(int i = 0; i < 4; i++)
  {
    for(int k = 0; k <7 ; k++)
    {
      update_onboard(SQUARE1[k] + 30*i, 2, 0);
      update_onboard(SQUARE1[k] + 30*i, 10, 0);
    }
    update_onboard(30*i, 3, 0);
    update_onboard(30*i, 4, 0);
    update_onboard(30*i, 5, 0);
    update_onboard(30*i, 6, 0);
    update_onboard(30*i, 7, 0);
    update_onboard(30*i, 8, 0);
    update_onboard(30*i, 9, 0);
  }
  Serial.begin(9600);
}

void loop()
{
  //Periodically realign the display; causes jumping but the Arduinos will unsync.
  display_realign();
}

void display_realign()
{
  digitalWrite(LED_sync, HIGH);
  delay(500);
  digitalWrite(LED_sync, LOW);
}

void update_onboard(uint8_t timeslice, uint8_t layer, uint16_t newval) //Update coordinate of onboard arduino
{
  tierport = layer/onboard_num_layers + 1;         //Tier 0 is PC0, Tier 1 is PC1, Tier 2 is PC2, Clock is PC3; (Analog Pins);
  Ctransfer(16, newval);      //Transfer new value;
  Ctransfer(7, (vol_slices - vol_slices/8*(layer%8) + timeslice)%vol_slices);//Transfer timeslice coordinate with offsets;
  Ctransfer(3, layer%onboard_num_layers);        //Transfer layer coordinate;
  delay(30);
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
  digitalWrite(LED_sync, LOW);
  Serial.print("Sending layer: ");
  Serial.print(currentLayer);
  Serial.print("\n");
  //for loop sending Spokes to Bareduinos
  Serial.print("Wiping SpokeArr\n");
  delay(2000);
  for(int i; i<vol_slices; i++) 
  {
    if(SpokeArr[i].data != 0)
      update_onboard(SpokeArr[i].slice, SpokeArr[i].layer, SpokeArr[i].data);
    SpokeArr[i].data = 0;
    SpokeArr[i].slice = 0;
    SpokeArr[i].layer = 0;
  }
  *numSpokes = 0;
  digitalWrite(LED_sync, HIGH);
  delay(2000);
  digitalWrite(LED_sync, LOW);
}


