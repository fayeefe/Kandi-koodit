void setup() {
  pinMode(13, INPUT);
  pinMode(5, INPUT);
}

void loop() {

  if(5 == HIGH){
    float startTime = millis();
    for(int i = 0; i <= 300; i++){
      float currentTime = millis();
      //skips ahead one second
      //while (currentTime <= pollTime){currentTime = millis();}
      int dig = digitalRead(13);
      // outputs 
      Serial.println(dig);
      Serial.println((currentTime-startTime)/1000);
      delay(1000);
    }
  }
}

