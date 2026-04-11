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

float average;
// calculating the average value 
void averaging(){
  float addition = 0;
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
double measureTime;
void output(){
  // voltage = 3.3/4096*dig;
      // float thickness1 = 12.7/(V_max-V_min)*3.3/4096*dig+v_offset) !!!!!! needs the voltage values from the actual device !!!!!!
      //int dig = analogRead(34);
  float diameter = (3.970/1.5376)*(3.30/4096*average)-1.86; // Tune the last parameter to get the correct thickness value when the sensor is at 0
  measureTime = measureTime + 1;
  //Serial.println(average); //analog value
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
      timerStart(timer);
      break;
    case LOW:
      timerStop(timer);
      timerWrite(timer,0);
      
      break;
    }
  } 
  lastState = currentState;
  // changes the task according to the latched switch
  
  
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

          measurement();
          break;
        default:
          Serial.println("Error in isrCounter");
          break;
      }
  }
}

