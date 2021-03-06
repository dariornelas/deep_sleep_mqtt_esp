#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "NOS_WIFI_Fon";
const char* password = "";

//nos login captive
const String NOSUSR  = "*****";
const String NOSPASS  = "******";
//gerada aqui -> https://www.grc.com/fingerprints.htm
const String fingerprint  = "E6:E9:08:D6:6A:7E:E7:5D:34:5D:F0:DB:28:14:DA:2B:8D:87:3A:02";


#define mqtt_server "****"
#define mqtt_user "*****"
#define mqtt_password "****"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

if (!client.connected()) {
    reconnect();
  }
  client.loop();

 
      Serial.println("correio");
      client.publish("predio/correio", "1", true);
      Serial.println("bye");
      ESP.deepSleep(0);
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
//nos wifi
  String uri = "";

  static const char* LOCATION = "Location";
  static const char* SET_COOKIE = "Set-Cookie";
  static const char* HEADER_NAMES[] = {LOCATION, SET_COOKIE};

  HTTPClient http;
  // saca o captive portal da nos ja com os querystrings
  http.begin("http://captive.apple.com/");
  http.collectHeaders(HEADER_NAMES, 2);
  int httpCode = http.GET();
  uri = http.header(LOCATION);

  // print do link do captive portal
  Serial.print("portal=");
  Serial.println(uri);
  delay(2000);

  http.begin(uri,fingerprint);     //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
  http.addHeader("Cookie", http.header(SET_COOKIE));
  httpCode = http.POST("userFake=" + NOSUSR + "&UserName=NOS/" + NOSUSR + "&Password=" + NOSPASS); //Send the request
  Serial.println(httpCode);   //Print 302 movido temp -2*/
  delay(2000);


  uri = http.header(LOCATION);
  http.collectHeaders(HEADER_NAMES, 2);
  httpCode = http.GET();
  http.begin(uri);

  Serial.print("1-redirect=");
  Serial.println(uri);
  Serial.println(httpCode);   //Print 302 movido temp -2*/
  delay(2000);


  uri = http.header(LOCATION);
  http.collectHeaders(HEADER_NAMES, 2);
  httpCode = http.GET();
  http.begin(uri);

  Serial.print("2-redirect=");
  Serial.println(uri);
  Serial.println(httpCode);   //Print 302 movido temp -2*/
  delay(2000);

  
  http.end();  //Close connection
  
}

void reconnect() {
  // Loop until we're reconnected
  int count =0;
  while (!client.connected() && count<5) {
    Serial.print("Attempting MQTT connection...");
    Serial.print(count);
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266-correio",mqtt_user,mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);//tempo
    }
    count++;
  }
}



void loop() {
  
  }

