const int buttonPin = 5;
void setup() {
  pinMode(A1, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println("Connected");
}

void loop() {
  if(digitalRead(buttonPin) == HIGH){
    Serial.println("button was pressed");
    float startTime = millis();
    for(int i = 0; i <= 300; i++){
      float currentTime = millis();
      float dig = analogRead(A1);
      // outputs 
      Serial.println(dig);
      Serial.println((currentTime-startTime)/1000);
      //skips ahead one second
      delay(1000);
    }
  }
}

