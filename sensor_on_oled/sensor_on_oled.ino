#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Definire pin senzor DHT11
#define DHTPIN D4  // Pinul conectat la DATA (GPIO2)
#define DHTTYPE DHT11  // Tipul senzorului

// Inițializare senzor
DHT dht(DHTPIN, DHTTYPE);

// Definire dimensiune ecran OLED
#define SCREEN_WIDTH 128  // Lățimea ecranului OLED
#define SCREEN_HEIGHT 64  // Înălțimea ecranului OLED

// Crearea obiectului ecran OLED I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);  // Inițializare Serial Monitor
    Serial.println("Pornire senzor DHT11...");

    // Inițializare ecran OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // 0x3C este adresa I2C
        Serial.println(F("Ecran OLED nu a fost găsit!"));
        for(;;);  // Blochează dacă ecranul nu este găsit
    }
    display.display();  // Afișează ecranul inițializat

    dht.begin();  // Pornește senzorul DHT11
    delay(2000);  // Așteaptă 2 secunde
}

void loop() {
    delay(2000);  // Citire la fiecare 2 secunde

    // Citire valori de la senzor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Verificare dacă citirea a reușit
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Eroare la citirea senzorului DHT11!");
        return;
    }

    // Curățăm ecranul OLED înainte de a afisa noile valori
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);  // Setăm culoarea textului (alb)  
    
    // Afișăm valorile pe ecranul OLED

    // Afișăm textul pentru temperatura
    display.setTextSize(1);  // Dimensiune text standard
    display.setCursor(0, 5);  // Setăm poziția pe linia 3
    display.print("Temperatura:");
    display.setTextSize(2);  // Mărimea textului pentru valoare
    display.setCursor(0, 20);  // Afișăm valoarea temperaturii
    display.print(temperature);
    display.print(" C");

    // Afișăm textul pentru umiditate
    display.setTextSize(1);      // Setăm mărimea textului
    display.setCursor(0, 40);  // Setăm poziția pe prima linie
    display.print("Umiditate:");
    display.setTextSize(2);  // Mărimea textului pentru valoare
    display.setCursor(0, 50);  // Afișăm valoarea umidității
    display.print(humidity);
    display.print(" %");

    // Afișăm totul pe ecran
    display.display();

}
