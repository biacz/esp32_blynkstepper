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

#define BUTTON1 35
#define BUTTON2 0

#define STEPPER_IN1 27
#define STEPPER_IN2 26
#define STEPPER_IN3 25
#define STEPPER_IN4 33

#define REVOLUTION_STEP 2048 // 1 revolution

boolean stepperDirection = false;
int stepperStep = 0;
int stepperStepCount = 0;
boolean stepperMove = false;
long prevMillisStepper = 0;
int intervalStepper = 4; // Minimum is 2

boolean button1Pressed = false;
boolean button2Pressed = false;

BLYNK_WRITE(V0) // Button Widget is writing to pin V0
{
  stepperDirection = param.asInt();
}

BLYNK_WRITE(V1) // Button Widget is writing to pin V1
{
  int stepperSpeed = param.asInt();
}

BLYNK_WRITE(V2) // Button Widget is writing to pin V2
{
  stepperMove = true;
  stepperStepCount = 0;
  stepperStep = 1;
}

void setup()
{
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  pinMode(STEPPER_IN1, OUTPUT);
  pinMode(STEPPER_IN2, OUTPUT);
  pinMode(STEPPER_IN3, OUTPUT);
  pinMode(STEPPER_IN4, OUTPUT);

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

  Blynk.virtualWrite(0, 0);
  Blynk.virtualWrite(1, 1);
  Blynk.virtualWrite(2, 0);
}

void loop()
{
  Blynk.run();

  if (digitalRead(BUTTON1) == LOW && button1Pressed == false) {
    button1Pressed = true;
    stepperDirection = false;
    stepperMove = true;
    stepperStepCount = 0;
    stepperStep = 1;
  }
  else if (digitalRead(BUTTON1) == HIGH && button1Pressed == true) {
    button1Pressed = false;
  }

  if (digitalRead(BUTTON2) == LOW) {
    stepperDirection = true;
    stepperMove = true;
    stepperStepCount = 0;
    stepperStep = 1;
  }
  else if (digitalRead(BUTTON2) == HIGH && button2Pressed == true) {
    button2Pressed = false;
  }

  if (millis() - prevMillisStepper > intervalStepper) {
    if (stepperMove == true) {
      if (stepperDirection) {
        if (stepperStep++ >= 3) {
          stepperStep = 0;
        }
      }
      else {
        if (stepperStep-- == 0) {
          stepperStep = 3;
        }
      }

      if (stepperStepCount++ == REVOLUTION_STEP) {
        stepperMove = false;
        stepperStep = 4;

        Blynk.virtualWrite(2, 0);
      }

      switch (stepperStep) {
      case 0:
        digitalWrite(STEPPER_IN1, HIGH);
        digitalWrite(STEPPER_IN2, LOW);
        digitalWrite(STEPPER_IN3, LOW);
        digitalWrite(STEPPER_IN4, LOW);
        break;

      case 1:
        digitalWrite(STEPPER_IN1, LOW);
        digitalWrite(STEPPER_IN2, HIGH);
        digitalWrite(STEPPER_IN3, LOW);
        digitalWrite(STEPPER_IN4, LOW);
        break;

      case 2:
        digitalWrite(STEPPER_IN1, LOW);
        digitalWrite(STEPPER_IN2, LOW);
        digitalWrite(STEPPER_IN3, HIGH);
        digitalWrite(STEPPER_IN4, LOW);
        break;

      case 3:
        digitalWrite(STEPPER_IN1, LOW);
        digitalWrite(STEPPER_IN2, LOW);
        digitalWrite(STEPPER_IN3, LOW);
        digitalWrite(STEPPER_IN4, HIGH);
        break;

      default:
        digitalWrite(STEPPER_IN1, LOW);
        digitalWrite(STEPPER_IN2, LOW);
        digitalWrite(STEPPER_IN3, LOW);
        digitalWrite(STEPPER_IN4, LOW);
        break;
      }
    }
    prevMillisStepper = millis();
  }
}