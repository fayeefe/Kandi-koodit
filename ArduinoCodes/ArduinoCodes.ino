const int buttonPin = 5;
void setup() {
  pinMode(27, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println("Connected");
}

void loop() {
  if(digitalRead(buttonPin) == HIGH){
    Serial.println("button was pressed");
    float startTime = millis();
    while(buttonPin == LOW){
      float currentTime = millis();
      int dig = analogRead(27);
      // voltage = 3.3/4096*dig;
      // float thickness1 = 12.7/(V_max-V_min)*3.3/4096*dig+v_offset) !!!!!! needs the voltage values from the actual device !!!!!!
      // outputs 
      Serial.println(dig);
      // Serial.println(thickness)
      Serial.println((currentTime-startTime)/1000);
      //skips ahead one second
      delay(1000);
    }
  }
}

