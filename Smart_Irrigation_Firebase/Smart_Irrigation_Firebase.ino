#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
/* 1. Define the WiFi credentials */
#define WIFI_SSID "23iot23"
#define WIFI_PASSWORD "12345678"
//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino
/* 2. Define the API Key */
#define API_KEY "AIzaSyDY3tF0QMZyR6Lf41PS7X8G4QwBkfmChvQ"
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://smart-irrigation-system-92a81-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "shriharifirebase@gmail.com"
#define USER_PASSWORD "Shrihari@1234"
//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
String Irri;

int gasthres = 1000;
int tempthres = 40;
int rainthres = 200;

int gas;
int Humidity;
int Temperature;
int flame;
int rain;
int moisture;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print(" to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

}
void loop()
{
  if (Serial.available()) {
    Irri = Serial.readStringUntil('\n');

    int fristCommaIndex = Irri.indexOf(',');
    int secondCommaIndex = Irri.indexOf(',', fristCommaIndex + 1);
    int thirdCommaIndex = Irri.indexOf(',', secondCommaIndex + 1);
    int fourthCommaIndex = Irri.indexOf(',', thirdCommaIndex + 1);
    int fifthCommaIndex = Irri.indexOf(',', fourthCommaIndex + 1);
    int sixthCommaIndex = Irri.indexOf(',', fifthCommaIndex + 1);


    String Temp = Irri.substring(0, fristCommaIndex);
    String humid = Irri.substring(fristCommaIndex + 1, secondCommaIndex);
    String Flame = Irri.substring(secondCommaIndex + 1, thirdCommaIndex);
    String Soil = Irri.substring(thirdCommaIndex + 1, fourthCommaIndex);
    String Rain = Irri.substring(fourthCommaIndex + 1, fifthCommaIndex);
    String Gas = Irri.substring(fifthCommaIndex + 1, sixthCommaIndex);



    gas = Gas.toInt();
    Humidity = humid.toInt();
    Temperature = Temp.toInt();
    flame = Flame.toInt();
    moisture = Soil.toInt();
    rain = Rain.toInt();

    Firebase.setInt(fbdo, "Smart_Irrigation/Data/Humidity", Humidity);
    Firebase.setInt(fbdo, "Smart_Irrigation/Data/Temperature", Temperature);
    Firebase.setInt(fbdo, "Smart_Irrigation/Data/Gas", gas);
    Firebase.setInt(fbdo, "Smart_Irrigation/Data/Flame", flame);
    Firebase.setInt(fbdo, "Smart_Irrigation/Data/Rain_Level", rain);
    Firebase.setInt(fbdo, "Smart_Irrigation/Data/Soil_Moisture_Level", moisture);
    if (moisture == 0 ) {
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Soil_Moisture", "Dry State");
    }
    else if (flame == 1) {
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Flame_Detection", "Flame is Detected");
    }
    else if (Temperature > tempthres) {
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Temperature", "Temperature is High");
    }
    else if (gas > gasthres) {
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Gas", "Harm gas is detected");
    }
    else if (rain > rainthres) {
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Rain", "Raining");
    }
    else {
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Soil_Moisture", "Wet State");
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Flame_Detection", "There is no flame detected");
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Temperature", "Temperature is Normal");
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Gas", "No Harm gas detected");
      Firebase.setString(fbdo, "Smart_Irrigation/Status/Rain", "No Rain Detected");
    }
