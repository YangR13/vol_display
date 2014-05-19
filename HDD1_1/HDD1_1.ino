#include <Servo.h>

Servo esc;
int signal;

int gatePort = 7;
int gateThreshold = 0;
int previous, current, period, count, same = 0;
int testspeed = 95;
int num_samples = 100;

void setup()
{
  Serial.begin(9600);
  esc.attach(11);
  esc.write(10);
  Serial.println("Arming...");
  delay(5000);
  Serial.println("Initializing...");
  esc.write(testspeed);
  Serial.println("Begin rps sensing when speed constant.");
}
void loop()
{
  if(Serial.available() > 0)
  {
    Serial.println("Beginning rps sampling...");
    previous = millis();
    for(count = 1; count <= num_samples;)
    {
      if(digitalRead(gatePort) == gateThreshold && same == 0)
      {
        count++;
        same = 1;
      }
      if(digitalRead(gatePort) != gateThreshold && same == 1)
      {
        same = 0;
      }
    }
    current = millis();
    period = current - previous;
    Serial.print("Millis per 1000 rotations: ");
    Serial.println(period);
  }
  /*
  Serial.println(digitalRead(gatePort));
  /*
  if(analogRead(gatePort) < gateThreshold)
  {
    Serial.println("1");
  }
  else
  {
    Serial.println("0");
  }
  Serial.println(millis());
  if(analogRead(gatePort) == gateThreshold)
  {
    current = micros();
    period = current - previous;
    Serial.println(period);
    previous = current;
  }
  if(Serial.available() > 0)
  {
    signal = Serial.parseInt();
    if(signal >= 10 && signal <= 180)
    {
      Serial.print("Output: ");
      Serial.println(signal);
      esc.write(signal);
    }
    else
    {
      Serial.println("Invalid value.");
    }
  }
  */
}
