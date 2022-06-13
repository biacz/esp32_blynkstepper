/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP32 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

#define BLYNK_TEMPLATE_ID "TMPLlDbEstbF"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "H5-K9DRtM5Bt_R4MVUSuQp3V-dsoTLiZ"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Drueck mich!";
char pass[] = "Key01@BM";

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