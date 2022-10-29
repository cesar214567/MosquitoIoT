#include <ESP8266WiFi.h>// WiFi
#include <PubSubClient.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 0 // PIN connected to the dht11 (eventhough it says 0, is actually D3)

const char *ssid = "Galaxy A121A09"; // Enter your WiFi name
const char *password = "yynm5492"; // Enter WiFi password// MQTT Broker
const char *mqtt_broker = "192.168.120.238"; // Enter your WiFi or Ethernet IP
const char *topic = "test/topic";   //Enter topic 
const int mqtt_port = 10000;      //Enter MQTT port
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);
  
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to mosquitto mqttbroker\n", client_id.c_str());
    if (client.connect(client_id.c_str())) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // publish and subscribe
  client.publish(topic, "Hello From ESP8266!");
  client.subscribe(topic);
}

//Setting callback when someone publish anything to the topic
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println(" - - - - - - - - - - - -");
}

void loop() {
  client.loop();
  //Check if the client is connected
  if (client.connected()){

    //DHT reads
    DHT.read11(DHT11_PIN);
    delay(1000);
    //
    char temp[100];
    
    //publish temperature
    String(DHT.temperature,4).toCharArray(temp,100);
    client.publish(topic, "Temperature: ");
    client.publish(topic, temp);
    
    //publish humidity
    String(DHT.humidity,4).toCharArray(temp,100);
    client.publish(topic, "Humidity:" );
    client.publish(topic, temp);
  }
}
