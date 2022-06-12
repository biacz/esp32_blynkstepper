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

const int stepsPerRevolution = 238;
int stepCount = 0;
int val, absl, powerValue;

Stepper myStepper(stepsPerRevolution, 27, 26, 25, 33);

BLYNK_WRITE(V1)
{
  int speedValue = param.asInt();
  absl = abs(speedValue);
  Serial.print("absl: ");
  Serial.println(absl);
}

BLYNK_WRITE(V2)
{
  int powerValue = param.asInt();
  Serial.print("power: ");
  Serial.println(powerValue);
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Initialize Blynk.");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  if (powerValue == 1)
  {
    int motorSpeed = map(absl, 0, 1023, 0, 100);
    if (motorSpeed > 0)
    {
      myStepper.setSpeed(motorSpeed);
      myStepper.step(((stepsPerRevolution * val) / absl) / 100);
    }
  }
  else
  {
    myStepper.setSpeed(0);
  }
}