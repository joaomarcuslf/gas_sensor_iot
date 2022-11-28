#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "device.h"
#include "credentials.h"

#define MICRO_PULSE 500
#define SHORT_PULSE 2500
#define MEDIUM_PULSE 7500
#define LONG_PULSE 15000

float last_Reading = -1.0;
int loop_count = 0;

void connect_wifi();
void disconnect_wifi();
bool should_turn_wifi_on(float, int);

void send_sensor_read_to_receiver(float);
void send_alive_to_receiver();

void setup()
{
  Serial.begin(115200);
  delay(MICRO_PULSE);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("");
  Serial.println("[Server] Gas Sensor starting");

  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  float sensor_read = read_gas_senson();
  int delay_time = LONG_PULSE;

  if (should_turn_wifi_on()) // Only activate if the values are different
  {
    WiFi.forceSleepWake();
    delay(1);
    connect_wifi();
    digitalWrite(LED_BUILTIN, HIGH);
  }


  if (sensor_read != last_reading)
  {
    send_sensor_read_to_receiver(sensor_read);

    if (sensor_read >= 25.0) // Dangerous
    {
      delay_time = SHORT_PULSE;
    }
    else if (read >= 10.0) // Leak
    {
      delay_time = MEDIUM_PULSE;
    }

    last_Reading = sensor_read;
  }

  if (loop_count >= 20) // Every 5 Mins notify Receiver
  {
    send_alive_to_receiver();
    loop_count = -1;
  }

  loop_count += 1;

  digitalWrite(LED_BUILTIN, LOW);
  disconnect_wifi();
  delay(delay_time);
}

void connect_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("[Server] Connecting to");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(MICRO_PULSE);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("[Server] Not connected!");
  }
  else
  {
    Serial.print("[Server] Connected! IP-Address: ");
    Serial.println(WiFi.localIP());
  }
}

void disconnect_wifi()
{
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  Serial.println("WiFi is down");
}

bool should_turn_wifi_on(float sensor_read, int loop_count) {
  return sensor_read != last_reading || loop_count >= 20;
}

void send_sensor_read_to_receiver(float reading) {}

void send_alive_to_receiver() {}

