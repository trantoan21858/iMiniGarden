#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <Firebase_ESP_Client.h>

//smartconfig
#define FLASH_BUTTON 0

//led
#define R 2
#define G 4
#define B 5

//relay
#define RELAY_PIN 14

//độ ẩm đất A0

//cảm biến nhiệt độ, độ ẩm
#define DHTPIN 13
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

/**
 * firebase
 */
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyDcOIg3YGbrYlKM2DOeZsXUgx60w1jWx2U"
#define FIREBASE_PROJECT_ID "i-mini-garden"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-6vf27@i-mini-garden.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCzvIhjKFJRb8sy\n9M7qVR9RR5HpW1l/drCk9PTo7po3UxnW+/taLKVDj7QykzQnHiJgA5vdMf93WUAv\nAlMXJT2xXMEndTZ/nNRe0atWJNrzZaU6GI3LG4zbXldAIxwl/ENoW0Hy3YposZU2\niFFCs8K4X8vzeK/6bB4ThUKZlCSLFndNioO8II/ciYP8Lc9mSBwBX+gMBJF7JPZZ\nKTZ+ZrqxweJRulQZ841KkiqifCnFbf3/yxwlKfGrP884JxJMKUXnSOdRHDyOUzqZ\no4OGUD/MME6VzPZ+WJjskqwML4UTjiQkUStpYNXzV5DqLWLrlCI4gpOKjn2/SJMP\nEha9FqhBAgMBAAECggEAAjY1P7bbPhErx2zyUUpsmaJRfyQa1YDo3hl0xyPzuCKz\ndDwApAXD9Lm/HMoUJAQoLmZr5uyWYd1Df6oxMQfsOKAu14C6NwYQkEpoWwN8sKo1\nE/ebHgN1Plg5dBqpb0eQA/0zfwVkn390vAD9kJn25DAzusN4CIIhBfPODUTT4tOW\nzera4o/56DyIU5sVNEbscp3SBt3yDBBbCEmwtuhgyYXHZry8GahRZWji1VVDR1q0\neS9C32CVT97O/j++JGGpaSMWyG2XqMmF+5g4FBhHbUhQT1RdttHGxisrnTdYLFF1\nV6SJsrhk5/TQl7NyJ57YfkBPt4NNhLHZKPi3tI+8bQKBgQD6ggIVJCYqO8sGWF/A\nqkDupKE3txPQ20h5bC/1O+8Uj3BDe4c15r+gF1Ei2K4t7RoLrENmjxaTC2Xo84Rl\nSXTZpXCKkWfDIoJiYSngv1IDDtyZjCdNpkqvD9ipatDbaaHNceWR9Oc4aEY2HVI2\nOJPdDS2nbTjeFA6svNIL7boL0wKBgQC3rVDGQ2/DE5RDOvw1lHo9O1mwTda1MaBL\nCEwq/1/wMPPlUAJDeB9h08k9AV4NgpVkiCOdYrJ13V+7DsFoANddQOdOnhtsEwDU\nOc7KcQui/JmQ9qPz49WKDo9qhSxytOV76RQBo7jnRGPG3EAELG4WZBvQw+V+/r47\nLPrcCZZTGwKBgQCfKgt4s0csfmE43t235HTtK9zjMYd9pFYgEAVIo5P8VEbYGZyH\nOt12Mlpy8Yl51LQ/xmvste158DwIDvGICWwc/WjMIibzbPG64FFjYywQlVjElrBC\nPe4l3MOkoSGixzLu/fcHDbv7swUpTOf3W2tXgBAJ2PPQ5p2MU0yCF8Gl0QKBgQCp\npOf/1nU5JbJAThB3pEUqnNB/p8Tw4X79DhLbe7aub2g0masdiarifetV7xdhPwSg\nL6VFTc0qU2aBrGUxm+VN+g4pbJpDPWH8oc++5p4q0Uc8vrA7KGkn876+UXX2E+II\nigOyZ/b1kxLcrrxpnmenQpr2PJiQrNQib67a3/f5lQKBgDXAiRePmNfnKU7vWbg1\n0xhUC78v0BtmVLKS8+12xTBpOWBCJBU71IICExFo3pO+dCOMVVkdGPjUx/SCPcR6\n1RZCrSeclTsqpY4dpjqRO55eBxAXDyepEF1PTkz/2r3poLnn4gxRffND8IJ0a9YD\nID+hNi4HqLafheVPh+nyfhx7\n-----END PRIVATE KEY-----\n";

#define DATABASE_URL "https://i-mini-garden-default-rtdb.firebaseio.com/"

#define DATABASE_SECRET "RoqIsh1UZJJDkHhQG761toKYGJalVfSAGQZOUNVa"

FirebaseData fbdo;
FirebaseData stream;

FirebaseAuth auth;

FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

/**
 * 
 */

ESP8266WebServer webServer(2812);
/**
 * Smartconfig
 */
bool longPress() {
  static int lastPress = 0;
  if (millis() - lastPress > 3000 && digitalRead(FLASH_BUTTON) == 0) {
    return true;
  } else if (digitalRead(FLASH_BUTTON) == 1) {
    lastPress = millis();
  }
  return false;
}

bool in_smartconfig = false;

void enter_smartconfig()
{
  if (in_smartconfig == false) {
    in_smartconfig = true;
    WiFi.beginSmartConfig();
  }
}

void exit_smart()
{
  in_smartconfig = false;
}

/**
 * smartconfig_end
 */

/**
 * led
 */

void ledRed() {
  analogWrite(R,1024);
  analogWrite(G,0);
  analogWrite(B,0);
}

void ledGreen() {
  analogWrite(R,0);
  analogWrite(G,1024);
  analogWrite(B,0);
}

void ledBlue() {
  analogWrite(R,0);
  analogWrite(G,0);
  analogWrite(B,1024);
}

/**
 * led_end
 */

//firebase
void streamCallback(FirebaseStream data) {
  if(data.boolData()) {
    digitalWrite(RELAY_PIN,LOW);
    delay(10000);
    digitalWrite(RELAY_PIN,HIGH);
    String path = "/UsersData/";
    path += auth.token.uid.c_str(); //<- user uid or "Node1"
    path += "/devices/" + WiFi.macAddress();
    path += "/pump/timestamp";
    Firebase.RTDB.setTimestamp(&fbdo,path.c_str());
    Firebase.RTDB.setBool(&fbdo,data.streamPath().c_str(),false);
  } else {
    digitalWrite(RELAY_PIN,HIGH);
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timeout, resuming...\n");
}

void customTokeStatusCallback(TokenInfo info) {
  if (info.status == token_status_error)
    {
        Serial.printf("Token info: type = %s, status = %s\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
        Serial.printf("Token error: %s\n", getTokenError(info).c_str());
        Firebase.begin(&config, &auth);
    }
    else
    {
        Serial.printf("Token info: type = %s, status = %s\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
    }
}

void setup() {
  
  Serial.begin(9600);

  //led
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  //smart_config
  pinMode(FLASH_BUTTON, INPUT);

  //relay mặc định tắt
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  pinMode(DHTPIN, INPUT);

  //web_server
  webServer.on("/echo",echo);
  webServer.on("/check_register",checkRegister);
  webServer.begin();

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  //firebase
  config.api_key = API_KEY;
  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = PRIVATE_KEY;
  auth.token.uid = "Xwk9UsLtYEd2nINM5Rdp6oFbmS22";
  auth.token.claims.add("premium_account", true);
  auth.token.claims.add("admin", true);
  config.database_url = DATABASE_URL;
  fbdo.setResponseSize(4096);
  String base_path = "/UsersData/";
  config.token_status_callback = customTokeStatusCallback; //see addons/TokenHelper.h
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  String var = "$userId";
  String val = "($userId === auth.uid && auth.token.premium_account === true && auth.token.admin === true)";
  Firebase.RTDB.setReadWriteRules(&fbdo, base_path.c_str(), var.c_str(), val.c_str(), val.c_str(), DATABASE_SECRET);
  Firebase.RTDB.setReadWriteRules(&stream, base_path.c_str(), var.c_str(), val.c_str(), val.c_str(), DATABASE_SECRET);
  String path = "/UsersData/";
  path += auth.token.uid.c_str(); //<- user uid or "Node1"
  path += "/devices/" + WiFi.macAddress();
  path += "/pump/on";
  if (!Firebase.RTDB.beginStream(&stream, path.c_str()))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}

bool registed = false;

void echo() {
  DynamicJsonDocument doc(64);
  String bodyRequest = webServer.arg("plain");
  deserializeJson(doc, bodyRequest);
  Serial.println("Recive:");
  Serial.println(bodyRequest);
  webServer.send(200,"text/html","");
  registed = true;
}

void checkRegister() {
  DynamicJsonDocument responeJson(64);
  responeJson["registed"] = registed;
  String respone;
  serializeJson(responeJson, respone);
  webServer.send(200,"text/html",respone);
}

void loop() {

  if (millis() - dataMillis > 60000 && Firebase.ready()) {
        dataMillis = millis();
        String path = "/UsersData/";
        path += auth.token.uid.c_str(); //<- user uid or "Node1"
        path += "/devices/" + WiFi.macAddress();
        readDhtSensor(path);
        readSoilMoistureSensor(path);
  }


  //smart_config
  if (longPress()) {
    enter_smartconfig();
  }
  if (WiFi.status() == WL_CONNECTED && in_smartconfig && WiFi.smartConfigDone()) {
    exit_smart();
  }
  if (WiFi.status() == WL_CONNECTED) {
    ledGreen();
  } else if (in_smartconfig) {
    ledBlue();
  } else {
    ledRed();
  }
//  webServer.handleClient();
}

/**
 * cảm biến nhiệt độ, độ ẩm
 */
//Hàm đọc nhiệt độ từ cảm biến
void readDhtSensor(String path) {
  float humidity = 0;
  float temperature = 0;
  //chỉ số nhiệt
  float heatIndex = 0;
  // Wait a few seconds between measurements.

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
//    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  FirebaseJson json;
  json.add("humidity",humidity);
  json.add("temperature",temperature);
  json.add("heatIndex",heatIndex);
  Firebase.RTDB.updateNode(&fbdo, (path + "/DHTSensor").c_str(), &json);
  Firebase.RTDB.setTimestamp(&fbdo, (path + "/DHTSensor/timestamp").c_str());
}

/**
 * cảm biến độ ẩm đất
 */
void readSoilMoistureSensor(String path) {
  if(!Firebase.RTDB.setInt(&fbdo, (path + "/SoilMoistureSensor/value").c_str(), analogRead(A0)))
  {
  }
  if(!Firebase.RTDB.setTimestamp(&fbdo, (path + "/SoilMoistureSensor/timestamp").c_str()))
  {
  }
}
