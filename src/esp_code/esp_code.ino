#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <FS.h>
#include <SoftwareSerial.h>

#include <ArduinoJson.h>

#define TIMEOUT_ 10000
int lastCheck = 0;

String server_ip;

String lastRequest = "";
String lastAcceptedRequest = "";

const char* CONFIG_FILE = "/config.json";

bool readConfigFile();
bool writeConfigFile();

SoftwareSerial MySerial(D1, D0);

void setup () {
  
  Serial.begin(9600);
  MySerial.begin(9600);
  SPIFFS.begin();
  
  if (!readConfigFile()) {
    Serial.println("M:Failed to open config file for writing");
  } else {
    WiFi.printDiag(Serial);
    WiFi.mode(WIFI_STA); 
    unsigned long startedAt = millis();
    Serial.print("M:After waiting ");
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis()- startedAt);
    Serial.print(waited/1000);
    Serial.print(" secs in setup() connection result is ");
    Serial.println(connRes);
  }

  if (WiFi.status()!= WL_CONNECTED){
    Serial.println("M:Failed to connect, finishing setup anyway");
    configurePortal();
  } else{
    Serial.print("M:Local ip: ");
    Serial.println(WiFi.localIP());
  }
 
}
 
void loop() {
 if (WiFi.status() == WL_CONNECTED && (millis() - lastCheck > TIMEOUT_)) { //Check WiFi connection status
 
    HTTPClient http;

    http.begin(String(server_ip) + "/users/12345.json");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();                    //Send the request

    if (httpCode > 0) { //Check the returning code
      if(httpCode == 200){
        String payload = http.getString();
        if (lastAcceptedRequest != payload){
           Serial.println(payload);
          lastRequest = payload;
        }
      } else {
        Serial.println("M: Something went wrong: " + httpCode);
      }    
    }
 
    http.end();   //Close connection

    lastCheck = millis();
  }

  serialEventRun();
}

void configurePortal(){
    Serial.println("M:Configuration portal requested");
    
    char convertedValue[4];
    
    WiFiManagerParameter p_server_ip("server_ip", "Server IP", "", 40);

    WiFiManager wifiManager;
    
    
    wifiManager.addParameter(&p_server_ip);

    if (!wifiManager.startConfigPortal()) {
      Serial.println("M:Not connected to WiFi but continuing anyway.");
    } else {
      Serial.println("M:Connected...yeey  :)");
    }
    
    server_ip = String(p_server_ip.getValue());
    
    writeConfigFile();

    ESP.reset();
    
    delay(5000);
}

void serialEventRun(void) {
  if (MySerial.available()) serialEvent();
}

void serialEvent() {
  while (MySerial.available()) {
    String data = MySerial.readString();

    if(data == "Reset"){
      Serial.println("M:Reseting ESP");
      ESP.reset();
      delay(5000);
    } else if(data == "conf" || data == "conf\n"){
      Serial.println("M:Configurating portal");
      configurePortal();
    } else if(data == "OK" || data == "OK\n"){
//      Serial.println("M:Confirmed");
      lastAcceptedRequest = lastRequest;
    } else if(data[0] == '{'){
      //Send json to server
      HTTPClient http;
      http.begin(String(server_ip) + "/users/12345.json");
      http.addHeader("Content-Type", "application/json");
      
      int httpCode = http.PUT(data);
      if (httpCode != 200) { //Check the returning code
        Serial.println("M: Something went wrong: " + httpCode);
      } else {
        Serial.print("OK");
      }
    } else {
      Serial.print("M: No match for: " + data);
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
