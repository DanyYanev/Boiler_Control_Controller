int RELAY1 = 31;
int RELAY2 = 33;
int RELAY3 = 35;
int RELAY4 = 37;
int RELAY5 = 39;
int RELAY6 = 41;
int RELAY7 = 43;
int RELAY8 = 45;
int RELAY9 = 47;
int RELAY10 = 49;
int RELAY11 = 51;
int RELAY12 = 53;
int delayValue = 1000;

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY6, OUTPUT);
  pinMode(RELAY7, OUTPUT);
  pinMode(RELAY8, OUTPUT);
  pinMode(RELAY9, OUTPUT);
  pinMode(RELAY10, OUTPUT);
  pinMode(RELAY11, OUTPUT);
  pinMode(RELAY12, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  digitalWrite(RELAY5, HIGH);
  digitalWrite(RELAY6, HIGH);
  digitalWrite(RELAY7, HIGH);
  digitalWrite(RELAY8, HIGH);
  digitalWrite(RELAY9, HIGH);
  digitalWrite(RELAY10, HIGH);
  digitalWrite(RELAY11, HIGH);
  digitalWrite(RELAY12, HIGH);

  Serial.begin(9600);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(RELAY1, LOW);
  Serial.println("RELAY1 ON");
  delay(delayValue);
  digitalWrite(RELAY1, HIGH);
  Serial.println("RELAY1 OFF");
  delay(delayValue);

  digitalWrite(RELAY2, LOW);
  Serial.println("RELAY2 ON");
  delay(delayValue);
  digitalWrite(RELAY2, HIGH);
  Serial.println("RELAY2 OFF");
  delay(delayValue);

  digitalWrite(RELAY3, LOW);
  Serial.println("RELAY3 ON");
  delay(delayValue);
  digitalWrite(RELAY3, HIGH);
  Serial.println("RELAY3 OFF");
  delay(delayValue);

  digitalWrite(RELAY4, LOW);
  Serial.println("RELAY4 ON");
  delay(delayValue);
  digitalWrite(RELAY4, HIGH);
  Serial.println("RELAY4 OFF");
  delay(delayValue);

  digitalWrite(RELAY5, LOW);
  Serial.println("RELAY5 ON");
  delay(delayValue);
  digitalWrite(RELAY5, HIGH);
  Serial.println("RELAY5 OFF");
  delay(delayValue);

  digitalWrite(RELAY6, LOW);
  Serial.println("RELAY6 ON");
  delay(delayValue);
  digitalWrite(RELAY6, HIGH);
  Serial.println("RELAY6 OFF");
  delay(delayValue);

  digitalWrite(RELAY7, LOW);
  Serial.println("RELAY7 ON");
  delay(delayValue);
  digitalWrite(RELAY7, HIGH);
  Serial.println("RELAY7 OFF");
  delay(delayValue);

  digitalWrite(RELAY8, LOW);
  Serial.println("RELAY8 ON");
  delay(delayValue);
  digitalWrite(RELAY8, HIGH);
  Serial.println("RELAY8 OFF");
  delay(delayValue);

  digitalWrite(RELAY9, LOW);
  Serial.println("RELAY9 ON");
  delay(delayValue);
  digitalWrite(RELAY9, HIGH);
  Serial.println("RELAY9 OFF");
  delay(delayValue);

  digitalWrite(RELAY10, LOW);
  Serial.println("RELAY10 ON");
  delay(delayValue);
  digitalWrite(RELAY10, HIGH);
  Serial.println("RELAY10 OFF");
  delay(delayValue);

  digitalWrite(RELAY11, LOW);
  Serial.println("RELAY11 ON");
  delay(delayValue);
  digitalWrite(RELAY11, HIGH);
  Serial.println("RELAY11 OFF");
  delay(delayValue);

  digitalWrite(RELAY12, LOW);
  Serial.println("RELAY12 ON");
  delay(delayValue);
  digitalWrite(RELAY12, HIGH);
  Serial.println("RELAY12 OFF");
  delay(delayValue);
}
 
  

