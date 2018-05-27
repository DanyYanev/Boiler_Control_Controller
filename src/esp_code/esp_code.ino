#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <FS.h>

#include <ArduinoJson.h>

#define TIMEOUT_ 10000
int lastCheck = 0;

String server_ip;

String lastRequest = "";

//const int TRIGGER_PIN = D3;

const char* CONFIG_FILE = "/config.json";

bool readConfigFile();
bool writeConfigFile();
 
void setup () {
  Serial.begin(115200);
  Serial.println("IM AWAKE");
  SPIFFS.begin();
  
  if (!readConfigFile()) {
    Serial.println("Failed to read configuration file, using default values");
    if(SPIFFS.format()){
      Serial.println("Format passed");
      File f = SPIFFS.open(CONFIG_FILE, "w");
      if (!f) {
        Serial.println("Failed to open config file for writing");
        configurePortal();
      } else f.close();
    } else {
      Serial.println("Format failed");
    }
  } else {
    WiFi.printDiag(Serial);
    WiFi.mode(WIFI_STA); 
    unsigned long startedAt = millis();
    Serial.print("After waiting ");
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis()- startedAt);
    Serial.print(waited/1000);
    Serial.print(" secs in setup() connection result is ");
    Serial.println(connRes);
  }

  if (WiFi.status()!= WL_CONNECTED){
    Serial.println("Failed to connect, finishing setup anyway");
    configurePortal();
  } else{
    Serial.print("Local ip: ");
    Serial.println(WiFi.localIP());
  }
 
}
 
void loop() {
 if (WiFi.status() == WL_CONNECTED && (millis() - lastCheck > TIMEOUT_)) { //Check WiFi connection status
 
    HTTPClient http;
 
//    http.begin("http://192.168.1.127:3000/users/12345.json");  //Specify request destination
    http.begin(String(server_ip) + "/users/12345.json");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();                    //Send the request

    Serial.println("Sending to: "+ String(server_ip) + "/users/12345.json");
    
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
//      if (lastRequest != payload){
//        Serial.println(payload);                     //Print the response payload  
//        lastRequest = payload;
//      }
      Serial.println(payload);
    }
 
    http.end();   //Close connection

    lastCheck = millis();
  }

  serialEventRun();
}

void configurePortal(){
    Serial.println("Configuration portal requested");
    
    char convertedValue[4];
    
    WiFiManagerParameter p_server_ip("server_ip", "Server IP", "", 40);

    WiFiManager wifiManager;
    
    
    wifiManager.addParameter(&p_server_ip);

    if (!wifiManager.startConfigPortal()) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      Serial.println("Connected...yeey  :)");
    }
    
    server_ip = String(p_server_ip.getValue());
    
    writeConfigFile();

    ESP.reset();
    
    delay(5000);
}

void serialEventRun(void) {
  if (Serial.available()) serialEvent();
}

void serialEvent() {
  Serial.println("IM SERIAL EVENT");
  while (Serial.available()) {
    String data = Serial.readString();
    Serial.println("GOT THIS: " + data);
    if(data == "Reset"){
      Serial.println("Reseting ESP");
      ESP.reset();
      delay(5000);
    } else if(data == "conf" || data == "conf\n"){
      Serial.println("Configurating portal");
      configurePortal();
    } else {
      //Send json to server
      HTTPClient http;
      http.begin(String(server_ip) + "/users/12345");
      http.addHeader("Content-Type", "application/json");
      
      int httpCode = http.PUT(data);
      if (httpCode == 200) { //Check the returning code
        Serial.println("Data update successfull");
        delay(1000);
      }
      Serial.println(data);
    }
  }
}

bool readConfigFile() {
  File f = SPIFFS.open(CONFIG_FILE, "r");
  
  if (!f) {
    Serial.println("Configuration file not found");
    return false; 
  } else {
    
    size_t size = f.size();

    std::unique_ptr<char[]> buf(new char[size]);

   
    f.readBytes(buf.get(), size);

    f.close();

    DynamicJsonBuffer jsonBuffer;

    JsonObject& json = jsonBuffer.parseObject(buf.get());

    if (!json.success()) {
      Serial.println("JSON parseObject() failed");
      return false;
    }
    json.printTo(Serial);

    if (json.containsKey("server_ip")) {
      server_ip = (const char*)json["server_ip"];
    }
  }
  Serial.println("\nConfig file was successfully parsed");
  return true;
}

bool writeConfigFile(){
  Serial.println("Saving config file");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject(); 

  File f = SPIFFS.open(CONFIG_FILE, "w");
  if (!f) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.prettyPrintTo(Serial);
  json["server_ip"] = server_ip;
  
  json.printTo(f);
  f.close();

  Serial.println("\nConfig file was successfully saved");
  return true;
}

//    StaticJsonBuffer<200> jsonBuffer;
//    JsonObject& root = jsonBuffer.createObject();
//
//    root["uid"] = uid;
//    root["room"] = room;
//
//    char jsonBody[100];
//    root.printTo(jsonBody, root.measureLength() + 1);
//    Serial.println(jsonBody);
//    
//    HTTPClient http;
// 
//    http.begin(server_ip);
//    http.addHeader("Content-Type", "application/json");
//    
//    int httpCode = http.POST(String(jsonBody));
//    Serial.print("HTTP request send to: ");
//    Serial.println(server_ip);
//    if (httpCode == 200) { //Check the returning code
//      Serial.println("JSON Parsed succccsessfully");
//      digitalWrite(GREEN_LED_PIN, HIGH);
//      delay(1000);
//      digitalWrite(GREEN_LED_PIN, LOW);
//    } else if(httpCode == 401){
//      Serial.println("No such database entry");
//      tone(BUZZER_PIN, 800);
//      delay(1000);
//      noTone(BUZZER_PIN);
//    }else{
//      tone(BUZZER_PIN, 800);
//      delay(500);
//      noTone(BUZZER_PIN);
//      delay(200);
//      tone(BUZZER_PIN, 800);
//      delay(500);
//      noTone(BUZZER_PIN);
//      delay(200);
//      tone(BUZZER_PIN, 800);
//      delay(500);
//      noTone(BUZZER_PIN);
//    }
// 
//    http.end();   //Close connection
//    rfid.PICC_HaltA();
//
//    // Stop encryption on PCD
//    rfid.PCD_StopCrypto1(); 
//  }
  
