int led_Data = A3;
int led_Latch = A2;
int led_Clock = A1;

const int vol_slices = 104;
const int vol_layers = 15;

typedef struct{
  uint16_t data;
  int slice;
  int layer;
} Spoke;

Spoke SpokeArr[vol_slices];

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
  int packageSize = 0;
  int numSpokes = 0;
  int currentLayer = 0;
  byte serialBuffer[4];
  
  Serial.print("serialEvent triggered!\n");
  
  while(true) {
     packageSize = Serial.readBytes((char *)serialBuffer, 4);
     if(packageSize == 0) break;
     
     if(serialBuffer[3] != currentLayer) {
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

void printLayer(int numSpokes) {
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

void sendLayer(int currentLayer, int *numSpokes) {
  Serial.print("Sending layer: ");
  Serial.print(currentLayer);
  Serial.print("\n");
  //for loop sending Spokes to Bareduinos
  
  Serial.print("Wiping SpokeArr\n");
  for(int i; i<vol_slices; i++) {
    SpokeArr[i].data = 0;
    SpokeArr[i].slice = 0;
    SpokeArr[i].layer = 0;
  }
  *numSpokes = 0;
}
