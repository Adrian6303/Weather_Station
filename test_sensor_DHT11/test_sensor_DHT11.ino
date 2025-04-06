#include <Arduino.h>
#include <DHT.h>

// Definire pin senzor DHT11
#define DHTPIN D4  // Pinul conectat la DATA (GPIO2)
#define DHTTYPE DHT11  // Tipul senzorului

// Inițializare senzor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200); // Inițializare Serial Monitor
    Serial.println("Pornire senzor DHT11...");
    dht.begin();
}

void loop() {
    delay(2000); // Citire la fiecare 2 secunde

    float humidity = dht.readHumidity();    // Citire umiditate
    float temperature = dht.readTemperature(); // Citire temperatura (Celsius)

    // Verificare dacă citirea a reușit
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Eroare la citirea senzorului DHT11!");
        return;
    }

    // Afișare date în Serial Monitor
    Serial.print("Umiditate: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" °C");
}
