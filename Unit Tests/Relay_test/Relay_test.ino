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
int relays[12] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6, RELAY7, RELAY8, RELAY9, RELAY10, RELAY11, RELAY12};
int i = 0;

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
  //pinMode(RELAY9, OUTPUT);
  //pinMode(RELAY10, OUTPUT);
  //pinMode(RELAY11, OUTPUT);
  //pinMode(RELAY12, OUTPUT);
  
  Serial.begin(9600);
  
  for(int i = 0; i < 8; i++){
    digitalWrite(relays[i], HIGH);
  }
}

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(relays[i], LOW);
  Serial.print("RELAY ");
  Serial.print(i);
  Serial.println(" ON");
  delay(delayValue);
 
  digitalWrite(relays[i], HIGH);
  Serial.print("RELAY ");
  Serial.print(i);
  Serial.println(" OFF");
  delay(delayValue);

  i++;
  
  if(i == 8) i = 0;
}
