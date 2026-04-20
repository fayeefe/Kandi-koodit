// Bachelors thesis Appendix 1
// Made by Freija Juntunen
// Code written for the ESP32-S thickness measurements

#include <deque>
#include <cmath>
#include <iostream>


using namespace std;
const int buttonPin = 25;
const int readingPin = 34; 
hw_timer_t *timer = NULL;
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
volatile SemaphoreHandle_t timerSemaphore ;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
deque<int> measurements;

// takes the measurement when the semaphone is flipped

void measurement(){
  int dig = analogRead(readingPin);
  measurements.push_front(dig);
}

int startTime;
float average;
// calculating the average value 
void averaging(){
  int addition = 0;
  for(int i : measurements){
    addition = addition + i;   
  }
  average = addition/3;
  measurements.clear();
  isrCounter = 1;
  output();
}

void ARDUINO_ISR_ATTR onTimer(){ 
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter = isrCounter + 1; // increments the counter
  portEXIT_CRITICAL_ISR(&timerMux);

  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void setup() {
  pinMode(readingPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  while(!Serial);
  timerSemaphore = xSemaphoreCreateBinary(); //creates the semaphore
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 500000, true, 0);
  timerStop(timer);
  timerWrite(timer,0);
}

// displaying values in serial monitor
float measureTime;
void output(){
  float diameter = (3.97/1.5376)*(3.3/4096*average)-2.389;
  int measurementTime = millis();
  double takenTime = (measurementTime-startTime) /1000;
  measureTime = measureTime + takenTime;
  Serial.print(diameter); Serial.print(",");
  Serial.println(measureTime);
}

bool lastState = HIGH;
bool latchedState = LOW;
void loop() {
  bool currentState = digitalRead(buttonPin);
  // latches the switch
  if(currentState == LOW && lastState == HIGH){
      delay(500);
    // turn on the timer here
      latchedState = !latchedState;
      Serial.println("thickness,time");
    switch(latchedState){
    case HIGH:
      measurements.clear();
      isrCounter = 0;
      measureTime = 0;
      startTime = 0;
      timerStart(timer);
      break;
    case LOW:
      timerStop(timer);
      timerWrite(timer,0);
      
      break;
    }
  } 
  lastState = currentState;
  // changes task according to the latched switch
  
  if(xSemaphoreTake(timerSemaphore, 0) == pdTRUE){ 
      switch(isrCounter){//checks the value of the isrcounter to see where in the cycle the system is
        case 1:
          measurement();
          break;
        case 2:
          measurement();         
          break;
        case 3:
          measurement();
          averaging();
          startTime = millis();
          measurement();
          break;
        default:
          Serial.println("Error in isrCounter");
          break;
      }
  }
}

