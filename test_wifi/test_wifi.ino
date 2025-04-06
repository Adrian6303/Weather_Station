#include <ESP8266WiFi.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Scanare Wi-Fi...");

    WiFi.mode(WIFI_STA);  // Modul Station (client)
    WiFi.disconnect();     // Deconectare înainte de scanare
    delay(1000);

    int n = WiFi.scanNetworks(); // Scanare rețele disponibile
    Serial.println("Scanare finalizată.");
    
    if (n == 0) {
        Serial.println("Nicio rețea Wi-Fi găsită.");
    } else {
        Serial.print(n);
        Serial.println(" rețele Wi-Fi găsite:");
        for (int i = 0; i < n; ++i) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (Semnal: ");
            Serial.print(WiFi.RSSI(i));
            Serial.println(" dBm)");
            delay(10);
        }
    }
}

void loop() {
}
