#include <ld2410.h>
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define RADAR_SERIAL Serial1
#define RADAR_RX_PIN 16
#define RADAR_TX_PIN 17

#define WIFI_SSID    "sua_rede"
#define WIFI_PASS    "senha_rede"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  "port"
#define AIO_USERNAME    "seu_user"
#define AIO_KEY         "sua_key"

// WiFi e MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish peopleFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/People Count");

ld2410 radar;

int buffer[100];
int count_buffer = 0;
uint16_t LastTime = 0;
uint16_t Interval = 0;
bool first_reading = true;
uint16_t currentDistance = 0;
int peopleCount = 0;
unsigned long lastMovementTime = 0;
unsigned long LastSendTime = 0;

void MQTT_connect() 
{
  int8_t ret;
  if (mqtt.connected())
  {
     return;
  }
  Serial.print("Conectando ao MQTT...");
  while ((ret = mqtt.connect()) != 0) 
  {
    Serial.println(mqtt.connectErrorString(ret));
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("MQTT conectado!");
}

void setup() 
{
  Serial.begin(115200);
  RADAR_SERIAL.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
  delay(500);

  Serial.print("\nInicializando sensor LD2410: ");
  if (radar.begin(RADAR_SERIAL))
  { 
    Serial.println("OK");
  }
  else 
  {
    Serial.println("Não conectado");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
}

void ResetBuffer() 
{
  for (int i = 0; i <= 100; i++) buffer[i] = 0;
}

int FindLast() 
{
  for (int i = 100; i > 0; i--) 
  {
    if (buffer[i] != 0)
    {
      return i;
    }
  }
  return 0;
}

void ProcessDistance() 
{
  if (FindLast() < 5) 
  {
    return;
  }

  int count = 0;
  int movement = buffer[FindLast()] - buffer[0];

  for (int i = 1; i < FindLast(); i++) 
  {
    if (buffer[i] < buffer[i - 1]) 
    {
      count++;
    }
    else
    {
      count--;
    }
  }

  if (count > 4) 
  {
    peopleCount++;
  }
  else if (count < -4) 
  {
    peopleCount = max(0, peopleCount - 1);
  }
  else 
  {
    return;
  }
}

void loop() {
  MQTT_connect();
  radar.read();

  if (radar.presenceDetected() && radar.movingTargetDetected() && count_buffer <= 100 && radar.movingTargetDistance() < 300 && radar.movingTargetDistance() > 80)
  {
    if (first_reading) 
    {
      LastTime = millis();
      first_reading = false;
    } 
    else if ((millis() - LastTime > 500)) 
    {
      if (count_buffer == 0) 
      {
        ResetBuffer();
      measure:
        currentDistance = radar.movingTargetDistance();
        Serial.print("Distance: "); Serial.print(currentDistance); Serial.println(" cm");
        buffer[count_buffer] = currentDistance;
        count_buffer++;
        first_reading = true;
        lastMovementTime = millis();
        goto fim;
      } 
      else if (radar.movingTargetDistance() != buffer[count_buffer - 1]) 
      {
        goto measure;
      }
    }
  } 
  else 
  {
    Interval = millis() - lastMovementTime;
    if (count_buffer > 100 && radar.presenceDetected() && radar.movingTargetDetected()) 
    {
      first_reading = true;
      count_buffer = 0;
      goto measure;
    } 
    else if (Interval > 1000 && count_buffer > 0) 
    {
      ProcessDistance();
      Serial.print("PeopleCount: ");
      Serial.println(peopleCount);
      count_buffer = 0;
      first_reading = true;
    }
  }

fim:
  if (millis() - LastSendTime > 5000) 
  {
    LastSendTime = millis();
    if (!peopleFeed.publish((int32_t)peopleCount))
    {
      Serial.println("Falha ao enviar MQTT.");
    }
    else 
    {
      Serial.println("MQTT enviado com sucesso.");
    }
  }
}
