#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <MQ135.h> // Biblioteca pentru senzorul MQ-135
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";  //exemplu
const char* password = "YOUR_WIFI_PASSWORD"; 

ESP8266WebServer server(80); // Portul serverului web

// Definire pin senzor DHT11
#define DHTPIN D4  // Pinul conectat la DATA (GPIO2)
#define DHTTYPE DHT11  // Tipul senzorului

// Definire pin senzor MQ-135
#define MQ135_PIN A0  // Pin analogic pentru MQ-135

// Inițializare senzori
DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(MQ135_PIN);

// Definire dimensiune ecran OLED
#define SCREEN_WIDTH 128  // Lățimea ecranului OLED
#define SCREEN_HEIGHT 64  // Înălțimea ecranului OLED

// Crearea obiectului ecran OLED I2C
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Inițializare monitor serial
  Serial.begin(115200);
  delay(100);  // Mică întârziere pentru stabilizarea comunicației seriale

  // Inițializare ecran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Ecran OLED nu a fost găsit! Verifică conexiunile."));
    for (;;); // Oprește execuția dacă nu se găsește ecranul
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Pornire...");
  display.display();

  // Inițializare senzor DHT
  dht.begin();
  delay(2000);  // Așteaptă senzorul DHT să se stabilizeze

  // Conectare la rețea WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectare la WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectat la WiFi!");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  // Afișare IP pe OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi conectat!");
  display.setCursor(0, 10);
  display.println(WiFi.localIP());
  display.display();

  // Configurare server web
  server.on("/data", []() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int sensorValue = analogRead(MQ135_PIN);


    String json = "{";
    json += "\"temperature\":" + String(temperature, 2) + ",";
    json += "\"humidity\":" + String(humidity, 2) + ",";
    json += "\"mq135\":" + String(sensorValue);
    json += "}";

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
  });

  // Pornire server web
  server.begin();
  Serial.println("Server web pornit pe portul 80.");
}


void loop() {
    server.handleClient();
    delay(2000);  // Citire la fiecare 2 secunde

    // Citire valori de la senzor DHT11
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Verificare dacă citirea a reușit
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Eroare la citirea senzorului DHT11!");
        return;
    }

    Serial.print("Temp: "); Serial.print(temperature); Serial.print(" C | ");
    Serial.print("Umid: "); Serial.print(humidity); Serial.print(" % | ");

    // Citire MQ-135
    int sensorValue = analogRead(MQ135_PIN);
    Serial.print("Valoare brută MQ-135: ");
    Serial.println(sensorValue);

    float voltage = analogRead(A0) * (5.0 / 1023.0);
    Serial.print("Tensiune MQ-135: ");
    Serial.print(voltage);
    Serial.println(" V");


    // Afișăm datele pe ecranul OLED
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("Temp:");
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print(temperature);
    display.print(" C");

    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("Umiditate:");
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print(humidity);
    display.print(" %");

    display.setTextSize(1);
    display.setCursor(70, 5);
    display.print("CO2:");
    display.setTextSize(1);
    display.setCursor(70, 20);
    display.print(sensorValue);
    display.print(" ppm");

    display.display();
}
