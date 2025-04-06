#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <MQ135.h> // Biblioteca pentru senzorul MQ-135

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
    Serial.begin(115200);  // Inițializare Serial Monitor
    Serial.println("Pornire senzori...");

    // Inițializare ecran OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  
        Serial.println(F("Ecran OLED nu a fost găsit!"));
        for(;;);
    }
    display.display();
    
    dht.begin();  // Pornește senzorul DHT11
    delay(2000);  // Așteaptă 2 secunde
}

void loop() {
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
