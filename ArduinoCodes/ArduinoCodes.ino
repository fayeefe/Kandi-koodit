#include <deque>
#include <cmath>
#include <iostream>

const int buttonPin = 25;
hw_timer_t *timer = NULL;
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
volatile SemaphoreHandle_t timerSemaphore ;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void ARDUINO_ISR_ATTR onTimer(){ 
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter = isrCounter + 1; // increments the counter
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  Serial.write("timer tripped ");
  Serial.write(isrCounter);
}

// takes the measurement when the semaphone is flipped
std::deque<int> measurements;
void measurement(){
  int dig = digitalRead(34);
  measurements.push_back(dig);
  Serial.println("mea");
}

// calculating the average value and displaying it in serial monitor
void averaging(){
  int addition = 0;
  for(int i : measurements){
    addition = addition + i;
    Serial.println("ave");

  }
  float average = addition/3;
  measurements.pop_front();
  measurements.pop_front();
  Serial.println(average);
  //Serial.println(millis());
}

void setup() {
  pinMode(34, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Connected");
  timerSemaphore = xSemaphoreCreateBinary(); //creates the semaphore
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0);
  timerStop(timer);
  timerWrite(timer,0);
  //timerRestart(timer);
}
// voltage = 3.3/4096*dig;
      // float thickness1 = 12.7/(V_max-V_min)*3.3/4096*dig+v_offset) !!!!!! needs the voltage values from the actual device !!!!!!
      //int dig = analogRead(34);
void loop() {
  if(digitalRead(buttonPin) == HIGH && timerRead(timer) == 0){
    // turn on the timer here
      timerStart(timer);
    } /*else if(digitalRead(buttonPin) == HIGH && timerRead(timer) != 0){
      timerStop(timer);
      timerWrite(timer,0);
    }*/
  

  if(xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
      if(float i = remainder(isrCounter,2) == 0){ //checks the remainder if it's an odd number or 0 it goes into 
      measurement();
    }else if (float i = remainder(isrCounter,2) != 0){
      measurement();
      averaging();
    }
  }
}

