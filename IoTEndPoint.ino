#include <Arduino.h>

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const char* WIFI_SSID = "";
const char* WIFI_PASSWORD= "";


const char* API_KEY= "";

const char* DATABASE_URL= ""; //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app;

const char* USER_EMAIL= "";
const char* USER_PASSWORD= "";


FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

const int BUTTON =15;
int buttonState = 0;


void setup_WIFI(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  config.signer.tokens.legacy_token = "<database secret>";
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}

void setup() {
    Serial.begin(115200);
    setup_WIFI();
    setupFirebase();
    pinMode(BUTTON,INPUT);
}

void loop() {

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
      buttonState = digitalRead(BUTTON);
      
    //Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, F("/sensor/a")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/sensor/a"), buttonState) ? "ok" : fbdo.errorReason().c_str());
    
  }

}
