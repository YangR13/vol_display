void setup() {
  int maxTicks = 1000;
  int tick = 0;
  
  int *snpstArr;
  snpstArr = (int *)malloc(maxTicks*sizeof(int));
  memset(snpstArr, -1, maxTicks);
  
  //fill array with proper values
}

void loop() {
  
  
  
  tick++;
  if(tick > maxTicks) tick = 0;
  
  //delay
}
