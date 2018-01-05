#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "MEO-WiFi";
const char* password = "";

//meo login captive
const String MEOUSR  = "******";
const String MEOPASS  = "*******";

//gerada aqui -> https://www.grc.com/fingerprints.htm
const String fingerprint  = "50:FE:E2:67:16:26:3C:D0:64:8F:23:43:5D:B2:82:56:A4:5F:03:B0";


#define mqtt_server "********"
#define mqtt_user "*******"
#define mqtt_password "******"

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
  
  HTTPClient http;    //Declare object of class HTTPClient

   http.begin("https://wifi.meo.pt/HttpHandlers/HotspotConnection.asmx/Login?usr="+MEOUSR,fingerprint);      //Specify request destination
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header

   int httpCode = http.POST("username="+MEOUSR+"&password="+MEOPASS);   //Send the request
   String payload = http.getString();                  //Get the response payload

   Serial.println(httpCode);   //Print HTTP return code // 200=sucesso
   Serial.println(payload);    //Print request response payload

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

