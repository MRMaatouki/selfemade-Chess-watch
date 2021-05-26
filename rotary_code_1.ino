

// Rotary Encoder Inputs
#include <Arduino.h>
#include <TM1637Display.h>

// rotary encoder definition
#define inputCLK 4
#define inputDT 5

// delay definition
#define TEST_DELAY   1000
#define secondsCounterDt 1000
#define PBdt 100

// Toggle switch definition
#define buttonPin 12
#define LEDPin 7

// LEDs definition
#define ledCW 8
#define ledCCW 9

// display definition
#define CLK 2
#define DIO 3

// rotary encoder vars'
int counter = 0;
int currentStateCLK;
int previousStateCLK;
String encdir = "";

//push button vars'
int LEDState = 0;
int buttonNew;
int buttonOld = 1;

//set boolean
boolean breakness = true;

// display setups
TM1637Display display(CLK, DIO);

void setup() {

  // Set encoder pins as inputs
  pinMode (inputCLK, INPUT);
  pinMode (inputDT, INPUT);

  // Set LED pins as outputs
  pinMode (ledCW, OUTPUT);
  pinMode (ledCCW, OUTPUT);

  // Setup Serial Monitor
  Serial.begin (9600);

  pinMode (LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // Read the initial state of inputCLK
  // Assign to previousStateCLK variable
  previousStateCLK = digitalRead(inputCLK);

}

void loop() {
  while (breakness == true) {
    buttonNew = digitalRead(buttonPin);
    if (buttonOld == 0 && buttonNew == 1) {
      if (LEDState == 0) {
        digitalWrite(LEDPin, HIGH);
        LEDState = 1;

      }
      else {
        digitalWrite(LEDPin, LOW);
        LEDState = 0;

      }
    }
    buttonOld = buttonNew;
    delay(PBdt);
  }
  breakness = false;
  
  while ( breakness == false) {
    // Read the current state of inputCLK
    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
    currentStateCLK = digitalRead(inputCLK);
    display.setBrightness(0x0f);
    //  display.setSegments(data);
    //  delay(TEST_DELAY);

    //  display.setSegments(data);
    //  delay(TEST_DELAY);

    // If the previous and the current state of the inputCLK are different then a pulse has occured
    if (currentStateCLK != previousStateCLK) {

      // If the inputDT state is different than the inputCLK state then
      // the encoder is rotating counterclockwise
      if (digitalRead(inputDT) != currentStateCLK) {
        counter --;
        encdir = "CCW";
        digitalWrite(ledCW, LOW);
        digitalWrite(ledCCW, HIGH);

      } else {
        // Encoder is rotating clockwise
        counter ++;
        encdir = "CW";
        digitalWrite(ledCW, HIGH);
        digitalWrite(ledCCW, LOW);

      }
      Serial.print("Direction: ");
      Serial.print(encdir);
      Serial.print(" -- Value: ");
      Serial.println(counter);
      display.showNumberDec(counter, false);

    }
    counter --;
    delay(secondsCounterDt);
    display.showNumberDec(counter, false);
    // Update previousStateCLK with the current state
    previousStateCLK = currentStateCLK;

  }
  breakness = true;
}
