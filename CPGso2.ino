// MQTT
#include <WiFiS3.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
const char ssid[] = SECRET_SSID;  // your network SSID (name)
const char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)

JsonDocument doc;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// const char broker[]    = "test.mosquitto.org";
const char broker[] = "mqtt-dashboard.com";
const int port = 1883;
const char willTopic[] = "arduino/will";
const char inTopic[] = "arduino/in";
const char outTopic[] = "arduino/out";
//^^^^^^^^^^^^ MQTT ^^^^^^^^^^^^^^^^^^

#include <SCServo.h>
SCSCL sc;

float alpha = 1.6;
float* ptr_alpha = &alpha;
float phi = PI / 12;
float* ptr_phi = &phi;
float mi = 0.05;
float* ptr_mi = &mi;

// Hormone
float beta = 0.01;
float* ptr_beta = &beta;
float alphah = 0.01;
float* ptr_alphah = &alphah;
float gramma = 0.01;
float* ptr_gramma = &gramma;

float hc = 1;
float* ptr_hc = &hc;
float hg = 0;
float* ptr_hg = &hg;
float hr = 0;
float* ptr_hr = &hr;
//^^^^^^^^^^^^ Hormone ^^^^^^^^^^^^^^^^^^

// SO2
// weight
float wso0so1 = (*ptr_alpha * sin(*ptr_phi + *ptr_mi));
float* ptr_wso0so1 = &wso0so1;
float wso0so0 = *ptr_alpha * cos(*ptr_phi);
float* ptr_wso0so0 = &wso0so0;
float wso1so1 = *ptr_alpha * cos(*ptr_phi);
float* ptr_wso1so1 = &wso1so1;
float wso1so0 = (*ptr_alpha * -sin(*ptr_phi - *ptr_mi));
float* ptr_wso1so0 = &wso1so0;
// float wso0so1 = 0.4;
// float* ptr_wso0so1 = &wso0so1;
// float wso0so0 = 1.5;
// float* ptr_wso0so0 = &wso0so0;
// float wso1so1 = 1.5;
// float* ptr_wso1so1 = &wso1so1;
// float wso1so0 = -0.4;
// float* ptr_wso1so0 = &wso1so0;

// so0 so1
float so0c = 0.01;
float* ptr_so0c = &so0c;
float so0o = tanh(*ptr_so0c);
float* ptr_so0o = &so0o;
float so1c = 0.01;
float* ptr_so1c = &so1c;
float so1o = tanh(*ptr_so1c);
float* ptr_so1o = &so1o;
//^^^^^^^^^^^^ SO2 ^^^^^^^^^^^^^^^^^^
// MRC IR1 IR2
float wmrc0mrc1 = -3.55;
float* ptr_wmrc0mrc1 = &wmrc0mrc1;
float wmrc0mrc0 = 5.4;
float* ptr_wmrc0mrc0 = &wmrc0mrc0;
float wmrc1mrc1 = 5.4;
float* ptr_wmrc1mrc1 = &wmrc1mrc1;
float wmrc1mrc0 = -3.55;
float* ptr_wmrc1mrc0 = &wmrc1mrc0;

float wir1mrc0 = 7;
float* ptr_wir1mrc0 = &wir1mrc0;
float wir2mrc1 = 7;
float* ptr_wir2mrc1 = &wir2mrc1;

float ir1 = 1;
float* ptr_ir1 = &ir1;
float ir2 = 1;
float* ptr_ir2 = &ir2;

// mrc0 mrc1
float mrc0c = 0;
float* ptr_mrc0c = &mrc0c;
float mrc0o = tanh(*ptr_mrc0c);
float* ptr_mrc0o = &mrc0o;
float mrc1c = 0;
float* ptr_mrc1c = &mrc1c;
float mrc1o = tanh(*ptr_mrc1c);
float* ptr_mrc1o = &mrc1o;
//^^^^^^^^^^^^ MRC ^^^^^^^^^^^^^^^^^^
float bias = -2.48285;
float* ptr_bias = &bias;
// VRN 0
float wso0vrn00 = 1.7246;
float* ptr_wso0vrn00 = &wso0vrn00;
float wso0vrn01 = -1.7246;
float* ptr_wso0vrn01 = &wso0vrn01;
float wso0vrn02 = 1.7246;
float* ptr_wso0vrn02 = &wso0vrn02;
float wso0vrn03 = -1.7246;
float* ptr_wso0vrn03 = &wso0vrn03;

float wmrc0vrn00 = 1.7246;
float* ptr_wmrc0vrn00 = &wmrc0vrn00;
float wmrc0vrn01 = -1.7246;
float* ptr_wmrc0vrn01 = &wmrc0vrn01;
float wmrc0vrn02 = -1.7246;
float* ptr_wmrc0vrn02 = &wmrc0vrn02;
float wmrc0vrn03 = 1.7246;
float* ptr_wmrc0vrn03 = &wmrc0vrn03;

float wvrn00vrn04 = 0.5;
float* ptr_wvrn00vrn04 = &wvrn00vrn04;
float wvrn01vrn04 = 0.5;
float* ptr_wvrn01vrn04 = &wvrn01vrn04;
float wvrn02vrn04 = -0.5;
float* ptr_wvrn02vrn04 = &wvrn02vrn04;
float wvrn03vrn04 = -0.5;
float* ptr_wvrn03vrn04 = &wvrn03vrn04;

// VRN00 VRN01 VRN02 VRN03 VRN04
float vrn00c = 0;
float* ptr_vrn00c = &vrn00c;
float vrn00o = tanh(*ptr_vrn00c);
float* ptr_vrn00o = &vrn00o;

float vrn01c = 0;
float* ptr_vrn01c = &vrn01c;
float vrn01o = tanh(*ptr_vrn01c);
float* ptr_vrn01o = &vrn01o;

float vrn02c = 0;
float* ptr_vrn02c = &vrn02c;
float vrn02o = tanh(*ptr_vrn02c);
float* ptr_vrn02o = &vrn02o;

float vrn03c = 0;
float* ptr_vrn03c = &vrn03c;
float vrn03o = tanh(*ptr_vrn03c);
float* ptr_vrn03o = &vrn03o;

float vrn04c = 0;
float* ptr_vrn04c = &vrn04c;
float vrn04o = tanh(*ptr_vrn04c);
float* ptr_vrn04o = &vrn04o;
//^^^^^^^^^^^^ VRN 0 ^^^^^^^^^^^^^^^^^^

// VRN 1
float wso0vrn10 = 1.7246;
float* ptr_wso0vrn10 = &wso0vrn10;
float wso0vrn11 = -1.7246;
float* ptr_wso0vrn11 = &wso0vrn11;
float wso0vrn12 = 1.7246;
float* ptr_wso0vrn12 = &wso0vrn12;
float wso0vrn13 = -1.7246;
float* ptr_wso0vrn13 = &wso0vrn13;

float wmrc1vrn10 = 1.7246;
float* ptr_wmrc1vrn10 = &wmrc1vrn10;
float wmrc1vrn11 = -1.7246;
float* ptr_wmrc1vrn11 = &wmrc1vrn11;
float wmrc1vrn12 = -1.7246;
float* ptr_wmrc1vrn12 = &wmrc1vrn12;
float wmrc1vrn13 = 1.7246;
float* ptr_wmrc1vrn13 = &wmrc1vrn13;

float wvrn10vrn14 = 0.5;
float* ptr_wvrn10vrn14 = &wvrn10vrn14;
float wvrn11vrn14 = 0.5;
float* ptr_wvrn11vrn14 = &wvrn11vrn14;
float wvrn12vrn14 = -0.5;
float* ptr_wvrn12vrn14 = &wvrn12vrn14;
float wvrn13vrn14 = -0.5;
float* ptr_wvrn13vrn14 = &wvrn13vrn14;

// VRN10 VRN11 VRN12 VRN13 VRN14
float vrn10c = 0;
float* ptr_vrn10c = &vrn10c;
float vrn10o = tanh(*ptr_vrn10c);
float* ptr_vrn10o = &vrn10o;

float vrn11c = 0;
float* ptr_vrn11c = &vrn11c;
float vrn11o = tanh(*ptr_vrn11c);
float* ptr_vrn11o = &vrn11o;

float vrn12c = 0;
float* ptr_vrn12c = &vrn12c;
float vrn12o = tanh(*ptr_vrn12c);
float* ptr_vrn12o = &vrn12o;

float vrn13c = 0;
float* ptr_vrn13c = &vrn13c;
float vrn13o = tanh(*ptr_vrn13c);
float* ptr_vrn13o = &vrn13o;

float vrn14c = 0;
float* ptr_vrn14c = &vrn14c;
float vrn14o = tanh(*ptr_vrn14c);
float* ptr_vrn14o = &vrn14o;
//^^^^^^^^^^^^ VRN 1 ^^^^^^^^^^^^^^^^^^

// recurent
float wso1re0 = 0.5;
float* ptr_wso1re0 = &wso1re0;
float wre0re0 = 1 - *ptr_wso1re0;
float* ptr_wre0re0 = &wre0re0;
float wre0re1 = 0.5;
float* ptr_wre0re1 = &wre0re1;
float wre1re1 = 1 - *ptr_wre0re1;
float* ptr_wre1re1 = &wre1re1;
float wre1re2 = 0.5;
float* ptr_wre1re2 = &wre1re2;
float wre2re2 = 1 - *ptr_wre1re2;
float* ptr_wre2re2 = &wre2re2;
float wre2re3 = 0.5;
float* ptr_wre2re3 = &wre2re3;
float wre3re3 = 1 - *ptr_wre2re3;
float* ptr_wre3re3 = &wre3re3;
float wre3re4 = 0.5;
float* ptr_wre3re4 = &wre3re4;
float wre4re4 = 1 - *ptr_wre3re4;
float* ptr_wre4re4 = &wre4re4;
// re0 re1 re2 re3 re4
float re0c = 0;
float* ptr_re0c = &re0c;
float re0o = tanh(*ptr_re0c);
float* ptr_re0o = &re0o;

float re1c = 0;
float* ptr_re1c = &re1c;
float re1o = tanh(*ptr_re1c);
float* ptr_re1o = &re1o;

float re2c = 0;
float* ptr_re2c = &re2c;
float re2o = tanh(*ptr_re2c);
float* ptr_re2o = &re2o;

float re3c = 0;
float* ptr_re3c = &re3c;
float re3o = tanh(*ptr_re3c);
float* ptr_re3o = &re3o;

float re4c = 0;
float* ptr_re4c = &re4c;
float re4o = tanh(*ptr_re4c);
float* ptr_re4o = &re4o;
//^^^^^^^^^^^^ recurent ^^^^^^^^^^^^^^^^^^

//motor
float m4c = 0;
float* ptr_m4c = &m4c;
float m4o = tanh(*ptr_m4c);
float* ptr_m4o = &m4o;

float m5c = 0;
float* ptr_m5c = &m5c;
float m5o = tanh(*ptr_m5c);
float* ptr_m5o = &m5o;

float m9c = 0;
float* ptr_m9c = &m9c;
float m9o = tanh(*ptr_m9c);
float* ptr_m9o = &m9o;

float m3c = 0;
float* ptr_m3c = &m3c;
float m3o = tanh(*ptr_m3c);
float* ptr_m3o = &m3o;

float m11c = 0;
float* ptr_m11c = &m11c;
float m11o = tanh(*ptr_m11c);
float* ptr_m11o = &m11o;

float m2c = 0;
float* ptr_m2c = &m2c;
float m2o = tanh(*ptr_m2c);
float* ptr_m2o = &m2o;

float m1c = 0;
float* ptr_m1c = &m1c;
float m1o = tanh(*ptr_m1c);
float* ptr_m1o = &m1o;

float m8c = 0;
float* ptr_m8c = &m8c;
float m8o = tanh(*ptr_m8c);
float* ptr_m8o = &m8o;

float m7c = 0;
float* ptr_m7c = &m7c;
float m7o = tanh(*ptr_m7c);
float* ptr_m7o = &m7o;

float m6c = 0;
float* ptr_m6c = &m6c;
float m6o = tanh(*ptr_m6c);
float* ptr_m6o = &m6o;

float m12c = 0;
float* ptr_m12c = &m12c;
float m12o = tanh(*ptr_m12c);
float* ptr_m12o = &m12o;
//^^^^^^^^^^^^ motor ^^^^^^^^^^^^^^^^^^

// servo
float stiffness = 1.5;
float damping = 0.0;
float torque[4];
float range[] = { 500, 700, 300, 500 };
//^^^^^^^^^^^^ servo ^^^^^^^^^^^^^^^^^^

const long interval = 10;
const long* ptr_interval = &interval;
unsigned long previousMillis = 0;
unsigned long* ptr_previousMillis = &previousMillis;
unsigned long currentMillis = millis();
unsigned long* ptr_currentMillis = &currentMillis;

float my_map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void hormone() {
  *ptr_hg = 0.01 * ((abs(sc.ReadLoad(1)) + abs(sc.ReadLoad(2)) + abs(sc.ReadLoad(3)) + abs(sc.ReadLoad(4)) + abs(sc.ReadLoad(5)) + abs(sc.ReadLoad(6)) + abs(sc.ReadLoad(7)) + abs(sc.ReadLoad(8)) + abs(sc.ReadLoad(9)) + abs(sc.ReadLoad(11)) + abs(sc.ReadLoad(12))) / 11);
  *ptr_hr = *ptr_gramma * 1;
  *ptr_hc = *ptr_beta * *ptr_hc + *ptr_alphah * *ptr_hg - *ptr_hr;
}

void leg_cpg() {
  // so2
  *ptr_so0o = tanh(*ptr_so0c);
  *ptr_so1o = tanh(*ptr_so1c);
  // mrc
  *ptr_mrc0o = tanh(*ptr_mrc0c);
  *ptr_mrc1o = tanh(*ptr_mrc1c);
  // recurent
  *ptr_re0o = tanh(*ptr_re0c);
  *ptr_re1o = tanh(*ptr_re1c);
  *ptr_re2o = tanh(*ptr_re2c);
  *ptr_re3o = tanh(*ptr_re3c);
  *ptr_re4o = tanh(*ptr_re4c);
  // motor
  *ptr_m4o = tanh(*ptr_m4c);
  *ptr_m5o = tanh(*ptr_m5c);
  *ptr_m9o = tanh(*ptr_m9c);
  *ptr_m3o = tanh(*ptr_m3c);
  *ptr_m11o = tanh(*ptr_m11c);
  *ptr_m2o = tanh(*ptr_m2c);
  *ptr_m1o = tanh(*ptr_m1c);
  *ptr_m8o = tanh(*ptr_m8c);
  *ptr_m7o = tanh(*ptr_m7c);
  *ptr_m6o = tanh(*ptr_m6c);
  *ptr_m12o = tanh(*ptr_m12c);
  // vrn 0
  *ptr_vrn00o = tanh(*ptr_vrn00c);
  *ptr_vrn01o = tanh(*ptr_vrn01c);
  *ptr_vrn02o = tanh(*ptr_vrn02c);
  *ptr_vrn03o = tanh(*ptr_vrn03c);
  *ptr_vrn04o = tanh(*ptr_vrn04c);
  // vrn 1
  *ptr_vrn10o = tanh(*ptr_vrn10c);
  *ptr_vrn11o = tanh(*ptr_vrn11c);
  *ptr_vrn12o = tanh(*ptr_vrn12c);
  *ptr_vrn13o = tanh(*ptr_vrn13c);
  *ptr_vrn14o = tanh(*ptr_vrn14c);
  // vrn 0
  *ptr_vrn00c = (*ptr_wmrc0vrn00 * *ptr_mrc0o) + (*ptr_wso0vrn00 * *ptr_so0o) + *ptr_bias;
  *ptr_vrn01c = (*ptr_wmrc0vrn01 * *ptr_mrc0o) + (*ptr_wso0vrn01 * *ptr_so0o) + *ptr_bias;
  *ptr_vrn02c = (*ptr_wmrc0vrn02 * *ptr_mrc0o) + (*ptr_wso0vrn02 * *ptr_so0o) + *ptr_bias;
  *ptr_vrn03c = (*ptr_wmrc0vrn03 * *ptr_mrc0o) + (*ptr_wso0vrn03 * *ptr_so0o) + *ptr_bias;
  *ptr_vrn04c = (*ptr_wvrn00vrn04 * *ptr_vrn00o) + (*ptr_wvrn01vrn04 * *ptr_vrn01o) + (*ptr_wvrn02vrn04 * *ptr_vrn02o) + (*ptr_wvrn03vrn04 * *ptr_vrn03o);
  // vrn 1
  *ptr_vrn10c = (*ptr_wmrc1vrn10 * *ptr_mrc1o) + (*ptr_wso0vrn10 * *ptr_so1o) + *ptr_bias;
  *ptr_vrn11c = (*ptr_wmrc1vrn11 * *ptr_mrc1o) + (*ptr_wso0vrn11 * *ptr_so1o) + *ptr_bias;
  *ptr_vrn12c = (*ptr_wmrc1vrn12 * *ptr_mrc1o) + (*ptr_wso0vrn12 * *ptr_so1o) + *ptr_bias;
  *ptr_vrn13c = (*ptr_wmrc1vrn13 * *ptr_mrc1o) + (*ptr_wso0vrn13 * *ptr_so1o) + *ptr_bias;
  *ptr_vrn14c = (*ptr_wvrn10vrn14 * *ptr_vrn10o) + (*ptr_wvrn11vrn14 * *ptr_vrn11o) + (*ptr_wvrn12vrn14 * *ptr_vrn12o) + (*ptr_wvrn13vrn14 * *ptr_vrn13o);
  // so2
  *ptr_wso0so1 = (*ptr_alpha * sin(*ptr_phi + *ptr_mi));
  *ptr_wso1so0 = (*ptr_alpha * -sin(*ptr_phi - *ptr_mi));
  *ptr_so0c = (*ptr_wso1so0 * *ptr_so1o) + (*ptr_wso0so0 * *ptr_so0o);
  *ptr_so1c = (*ptr_wso0so1 * *ptr_so0o) + (*ptr_wso1so1 * *ptr_so1o);
  // mrc
  *ptr_mrc0c = (*ptr_wmrc1mrc0 * *ptr_mrc1o) + (*ptr_wmrc0mrc0 * *ptr_mrc0o) + (*ptr_wir1mrc0 * *ptr_ir1);
  *ptr_mrc1c = (*ptr_wmrc0mrc1 * *ptr_mrc0o) + (*ptr_wso1so1 * *ptr_mrc1o) + (*ptr_wir2mrc1 * *ptr_ir2);
  // recurent
  *ptr_re0c = (*ptr_wso1re0 * *ptr_so1o);
  *ptr_re1c = (*ptr_wre0re1 * *ptr_re0o) + (*ptr_wre1re1 * *ptr_re1o);
  *ptr_re2c = (*ptr_wre1re2 * *ptr_re1o) + (*ptr_wre2re2 * *ptr_re2o);
  *ptr_re3c = (*ptr_wre2re3 * *ptr_re2o) + (*ptr_wre3re3 * *ptr_re3o);
  *ptr_re4c = (*ptr_wre3re4 * *ptr_re3o) + (*ptr_wre4re4 * *ptr_re4o);
  // motor
  *ptr_m4c = (1 * *ptr_so1o);
  *ptr_m5c = (1 * *ptr_so0o);
  *ptr_m9c = (1 * *ptr_so1o);
  *ptr_m1c = (1 * *ptr_so0o);

  *ptr_m11c = (1 * *ptr_re0o);
  *ptr_m2c = (1 * *ptr_re1o);
  *ptr_m3c = (1 * *ptr_re2o);
  *ptr_m8c = (1 * *ptr_re3o);
  *ptr_m7c = (1 * *ptr_re4o);

  *ptr_m6c = (1 * *ptr_so0o);
  *ptr_m12c = (1 * *ptr_so0o);
}

char mode = 't';

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    // Serial.print(mqttClient.readStringUntil('\n'));//(char)mqttClient.read()
    // Parse JSON object
    DeserializationError error = deserializeJson(doc, mqttClient.readStringUntil('\n'));
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    } else {
      if (doc["mode"]) {
        mode = String(doc["mode"]).charAt(0);
        Serial.println("mode: " + String(mode));
      } else {
        Serial.println("Not recived mode");
      }
      if (doc["stiffness"]) {
        stiffness = doc["stiffness"].as<float>();
        Serial.println("stiffness: " + String(stiffness));
      } else {
        Serial.println("Not recived stiffness");
      }
      if (doc["damping"]) {
        damping = doc["damping"].as<float>();
        Serial.println("damping: " + String(damping));
      } else {
        Serial.println("Not recived damping");
      }
      if (doc["range"]) {
        auto range = doc["range"];
        Serial.println("range: [" + String(range[0]) +" "+ String(range[1]) +" "+ String(range[2]) +" "+ String(range[3]) +"]");
      } else {
        Serial.println("Not recived range");
      }
      if (doc["mi"]) {
        *ptr_mi = doc["mi"].as<float>();
        Serial.println("mi: " + String(mi));
      } else {
        Serial.println("Not recived mi");
      }
    }
  }
  // Serial.println();

  // Serial.println();
}

void reconnectWiFi() {
  // Check if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi reconnected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(1000000);
  sc.pSerial = &Serial1;
  // sc.WritePos(0xfe, 500, 1000);
  sc.WritePos(4, 500, 1000);
  sc.WritePos(5, 500, 1000);
  sc.WritePos(9, 500, 1000);
  sc.WritePos(3, 500, 1000);

  sc.WritePos(11, 500, 1000);
  sc.WritePos(2, 600, 1000);
  sc.WritePos(1, 500, 1000);
  sc.WritePos(8, 500, 1000);
  sc.WritePos(7, 650, 1000);

  sc.WritePos(6, 0, 1000);
  sc.WritePos(12, 1000, 1000);
  sc.EnableTorque(0xfe, 0);
  delay(1000);

  // MQTT
  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // set a will message, used by the broker when the connection dies unexpectedly
  // you must know the size of the message beforehand, and it must be set before connecting
  String willPayload = "oh no!";
  bool willRetain = true;
  int willQos = 1;

  mqttClient.beginWill(willTopic, willPayload.length(), willRetain, willQos);
  mqttClient.print(willPayload);
  mqttClient.endWill();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1) {
      delay(1000);
    }
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(inTopic);
  Serial.println();

  // subscribe to a topic
  // the second parameter sets the QoS of the subscription,
  // the the library supports subscribing at QoS 0, 1, or 2
  int subscribeQos = 1;

  mqttClient.subscribe(inTopic, subscribeQos);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(inTopic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(inTopic);
  Serial.println();
  //^^^^^^^^^^^^ MQTT ^^^^^^^^^^^^^^^^^^
}

#define MAX_CID 4

void loop() {
  mqttClient.poll();
  reconnectWiFi();
  *ptr_currentMillis = millis();
  if (*ptr_currentMillis - *ptr_previousMillis >= *ptr_interval) {
    // Serial.println(String(previousMillis)+ " " + String(currentMillis));
    *ptr_previousMillis = *ptr_currentMillis;
    hormone();
    leg_cpg();
    // sc.EnableTorque(0xfe, 1);
    if (mode == 'g') {
      // leg
      sc.WritePos(4, my_map(*ptr_m4o, -0.94, 0.94, 380, 480), *ptr_interval);
      sc.WritePos(5, my_map(*ptr_m5o, -0.94, 0.94, 300, 500), *ptr_interval);
      sc.WritePos(9, my_map(*ptr_m9o, -0.94, 0.94, 490, 590), *ptr_interval);
      sc.WritePos(1, my_map(*ptr_m1o, -0.94, 0.94, 500, 700), *ptr_interval);
      // sc.WritePos(4, 500, *ptr_interval);
      // sc.WritePos(5, 500, *ptr_interval);
      // sc.WritePos(9, 500, *ptr_interval);
      // sc.WritePos(3, 500, *ptr_interval);
      // hand
      sc.WritePos(6, my_map(*ptr_m6o, -0.94, 0.94, 0, 200), *ptr_interval);
      sc.WritePos(12, my_map(*ptr_m12o, -0.94, 0.94, 800, 1000), *ptr_interval);
      // body & tail
      sc.WritePos(11, my_map(*ptr_m11o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(2, my_map(*ptr_m2o, -0.94, 0.94, 525, 675), *ptr_interval);
      sc.WritePos(3, my_map(*ptr_m3o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(8, my_map(*ptr_m8o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(7, my_map(*ptr_m7o, -0.94, 0.94, 575, 725), *ptr_interval);
    } else if (mode == 's') {
      // swim mode
      sc.WritePos(4, 800, *ptr_interval);
      sc.WritePos(5, 900, *ptr_interval);
      sc.WritePos(9, 200, *ptr_interval);
      sc.WritePos(1, 125, *ptr_interval);
      sc.WritePos(6, 20, *ptr_interval);
      sc.WritePos(12, 1000, *ptr_interval);
      sc.WritePos(11, my_map(*ptr_m11o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(2, my_map(*ptr_m2o, -0.94, 0.94, 525, 675), *ptr_interval);
      sc.WritePos(3, my_map(*ptr_m3o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(8, my_map(*ptr_m8o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(7, my_map(*ptr_m7o, -0.94, 0.94, 575, 725), *ptr_interval);
    } else if (mode == '0') {
      sc.WritePos(4, 500, *ptr_interval);
      sc.WritePos(5, 500, *ptr_interval);
      sc.WritePos(9, 500, *ptr_interval);
      sc.WritePos(3, 500, *ptr_interval);

      sc.WritePos(11, 500, *ptr_interval);
      sc.WritePos(2, 600, *ptr_interval);
      sc.WritePos(1, 500, *ptr_interval);
      sc.WritePos(8, 500, *ptr_interval);
      sc.WritePos(7, 650, *ptr_interval);

      sc.WritePos(6, 0, *ptr_interval);
      sc.WritePos(12, 1000, *ptr_interval);
      mode = 't';
    } else if (mode == 'd') {
      // leg
      for (unsigned char cID = 0; cID <= 3; cID++) {
        static int idnum[] = { 9, 1, 4, 5 };
        float value[] = { my_map(*ptr_m9o, -0.94, 0.94, range[0], range[1]), my_map(*ptr_m1o, -0.94, 0.94, 400, 600), my_map(*ptr_m4o, -0.94, 0.94, range[2], range[3]), my_map(*ptr_m5o, -0.94, 0.94, 400, 600) };
        static int tPos[MAX_CID];
        int Pos[MAX_CID];
        int Speed[MAX_CID];
        int Load[MAX_CID];
        static unsigned char sInc[MAX_CID];
        // static unsigned char cID = 0;

        // if (cID < MAX_CID) {
        //   cID++;
        // } else {
        //   cID = 0;
        // }

        if (sc.FeedBack(idnum[cID]) != -1) {
          Pos[cID] = sc.ReadPos(-1);
          Speed[cID] = sc.ReadSpeed(-1);
          Load[cID] = sc.ReadLoad(-1);

          torque[cID] = (stiffness * (value[cID] - Pos[cID])) + (damping * (0.0 - Speed[cID]));

          // Serial.print(" Impedance:");
          // Serial.print(torque[cID]);
          // Serial.print(" Position:");
          // Serial.print(Pos[cID]);
          // Serial.print(" Speed:");
          // Serial.print(Speed[cID]);
          // Serial.print(" Load:");
          // Serial.println(Load[cID]);
          // Serial.println(value[cID]);

          sc.WritePos(idnum[cID], Pos[cID] + torque[cID], *ptr_interval);
          // Serial.println(String(Pos[0] + torque[0]) + " " + String(Pos[1] + torque[1]) + " " + String(Pos[2] + torque[2]) + " " + String(Pos[3] + torque[3]));
        }
      }
      // hand
      sc.WritePos(6, my_map(*ptr_m6o, -0.94, 0.94, 0, 200), *ptr_interval);
      sc.WritePos(12, my_map(*ptr_m12o, -0.94, 0.94, 800, 1000), *ptr_interval);
      // body & tail
      sc.WritePos(11, my_map(*ptr_m11o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(2, my_map(*ptr_m2o, -0.94, 0.94, 525, 675), *ptr_interval);
      sc.WritePos(3, my_map(*ptr_m3o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(8, my_map(*ptr_m8o, -0.94, 0.94, 425, 575), *ptr_interval);
      sc.WritePos(7, my_map(*ptr_m7o, -0.94, 0.94, 575, 725), *ptr_interval);
    } else if (mode == 't') {
      sc.EnableTorque(0xfe, 1);
    } else {
      sc.EnableTorque(0xfe, 0);
    }

    Serial.println(String(*ptr_so0o) + " " + String(*ptr_so1o) + " " + String(*ptr_m9o) + " " + String(*ptr_m1o) + " " + String(*ptr_m4o) + " " + String(*ptr_m5o));
    // Serial.println(String(*ptr_so0o) + " " + String(*ptr_so1o) + " " + String(*ptr_m9o) + " " + String(*ptr_m1o) + " " + String(*ptr_m4o) + " " + String(*ptr_m5o));
    // Serial.println(String(*ptr_so1o) + " " + String(*ptr_so0o) + " " + String(*ptr_vrn14o) + " " + String(*ptr_vrn04o));
    // Serial.println(String(*ptr_m11o) + " " + String(*ptr_m2o) + " " + String(*ptr_m1o) + " " + String(*ptr_m8o) + " " + String(*ptr_m7o));
    // Serial.println(String(sc.ReadLoad(1)) + " " + String(sc.ReadLoad(2)) + " " + String(sc.ReadLoad(3)) + " " + String(sc.ReadLoad(4)) + " " + String(sc.ReadLoad(5)) + " " + String(sc.ReadLoad(6)) + " " + String(sc.ReadLoad(7)) + " " + String(sc.ReadLoad(8)) + " " + String(sc.ReadLoad(9)) + " " + String(sc.ReadLoad(11)) + " " + String(sc.ReadLoad(12)));
    // Serial.println(String(abs(sc.ReadLoad(1)) + abs(sc.ReadLoad(2)) + abs(sc.ReadLoad(3)) + abs(sc.ReadLoad(4)) + abs(sc.ReadLoad(5)) + abs(sc.ReadLoad(6)) + abs(sc.ReadLoad(7)) + abs(sc.ReadLoad(8)) + abs(sc.ReadLoad(9)) + abs(sc.ReadLoad(11)) + abs(sc.ReadLoad(12))));
    // Serial.println(String(*ptr_hg) + " " + String(*ptr_hr) + " " + String((0.17 * *ptr_hc) + 0.02));
  }

  if (Serial.available() > 0) {
    // *ptr_ir1 = Serial.parseFloat();
    // *ptr_ir2 = Serial.parseFloat();
    mode = Serial.readStringUntil('\n').charAt(0);
    Serial.println("mode: " + String(mode));
  }

  // Serial.println(mode);
}
