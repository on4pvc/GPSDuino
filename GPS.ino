#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// LCD I2C : adresse 0x27 ou 0x3F selon ton module
LiquidCrystal_I2C lcd(0x27, 20, 4);

// GPS sur SoftwareSerial (car Uno n'a qu'un seul Serial matériel)
static const int RXPin = 4, TXPin = 3;   // RX sur pin 4, TX sur pin 3
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initialisation...");

  gpsSerial.begin(GPSBaud);

  delay(2000);
  lcd.clear();
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      lcd.clear();

      // Ligne 0 : Latitude
      if (gps.location.isValid()) {
        lcd.setCursor(0, 0);
        lcd.print("Lat:");
        lcd.print(gps.location.lat(), 6);
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Lat: ---");
      }

      // Ligne 1 : Longitude
      if (gps.location.isValid()) {
        lcd.setCursor(0, 1);
        lcd.print("Lng:");
        lcd.print(gps.location.lng(), 6);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Lng: ---");
      }

      // Ligne 2 : Vitesse + Direction
      lcd.setCursor(0, 2);
      if (gps.speed.isValid()) {
        lcd.print("Vit:");
        lcd.print(gps.speed.kmph(), 1);
        lcd.print("km/h ");
      } else {
        lcd.print("Vit: --- ");
      }

      if (gps.course.isValid()) {
        lcd.print("Dir:");
        lcd.print(gps.course.deg(), 0);
        lcd.print((char)223); // symbole °
      } else {
        lcd.print("Dir: ---");
      }

      // Ligne 3 : Heure UTC + Satellites
      lcd.setCursor(0, 3);
      if (gps.time.isValid()) {
        if (gps.time.hour() < 10) lcd.print("0");
        lcd.print(gps.time.hour()); lcd.print(":");
        if (gps.time.minute() < 10) lcd.print("0");
        lcd.print(gps.time.minute()); lcd.print(":");
        if (gps.time.second() < 10) lcd.print("0");
        lcd.print(gps.time.second());
      } else {
        lcd.print("Time: --:--:--");
      }

      lcd.print(" Sat:");
      if (gps.satellites.isValid()) {
        lcd.print(gps.satellites.value());
      } else {
        lcd.print("---");
      }
    }
  }
}
