#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "XXX";
const char* password = "XXX";
const char* mqtt_server = "192.168.2.120";
const uint8_t bssid[6] = {XXX}; // anpassen
const int wifi_channel = 9;

const char* mqtt_topic = "///";
const char* mqtt_topic_state = "///";

const char* mqtt_client = "octoclick";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


const int led = 2;
const int button[] = {12, 13, 14};
const int latch = 16;

bool state[] = {false, false, false};


void setup() {
  pinMode(latch, OUTPUT);
  digitalWrite(latch, HIGH);

  for (int i = 0; i < 3; i++) {
    pinMode(button[i], INPUT);
    state[i] = digitalRead(button[i]);
  }

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop(); //never reaching this
}



void setup_wifi() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password, wifi_channel, bssid, true);

  while ((WiFi.status() != WL_CONNECTED)/* || (millis() < 1000)*/) {
    digitalWrite(led, LOW);
    delay(20);
    digitalWrite(led, HIGH);
    delay(100);

    if (millis() > 6000) {
      for (int i = 0; i < 5; i++) {
        digitalWrite(led, LOW);
        delay(200);
        digitalWrite(led, HIGH);
        delay(50);
      }
      digitalWrite(latch, LOW);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(mqtt_client)) {
      client.subscribe(mqtt_topic_state);
      delay(100);

      for (int i = 0; i < 3; i++) {
        if (state[i]) {
          char b[2];
          String x = String(i);
          x.toCharArray(b, 2);
          client.publish(mqtt_topic_state, b);
          for (int n = 0; n <= i; n++) {
            digitalWrite(led, LOW);
            delay(50);
            digitalWrite(led, HIGH);
            delay(20);
          }
        }
      }

      delay(100);
      digitalWrite(latch, LOW);
      delay(200);
    }
  }
}
