#include "Settings.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

WiFiServer server(80);

#include <SPI.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;
int stepCount = 0;
int var, val, blynkSpeed, motorSpeed, blynkPower;

Stepper myStepper(stepsPerRevolution, 27, 26, 25, 33);

void setup()
{
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V1)
{
  int val = param.asInt();
  blynkSpeed = abs(val);
  Serial.print("Speed Value: ");
  Serial.println(blynkSpeed);
}

BLYNK_WRITE(V2)
{
  int var = param.asInt();
  blynkPower = abs(var);
  Serial.print("Power Value: ");
  Serial.println(blynkPower);
}

void loop()
{
  Blynk.run();
  // set the motor speed:
  if (blynkPower == 1)
  {
    if (blynkSpeed > 0)
    {
      myStepper.setSpeed(blynkSpeed);
      myStepper.step(stepsPerRevolution / 100);
    }
  }
}