#include <SFE_BMP180.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>

/*
   Global variables
*/
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int numRows = 4;
const int numCols = 20;

SFE_BMP180 bmp;
double P;

DHT dht(A1, DHT22);
float T, H;

void setup() {
  // put your setup code here, to run once:
  /*
     LCD Setup
  */
  lcd.begin(numCols, numRows);
  lcd.setCursor(0, 0);

  /*
     BMP180 Setup
  */
  if (bmp.begin()) {
    lcd.print("BMP Started.");
  } else {
    lcd.print("BMP Failed.");
    while(1);
  }

  /*
     DHT22
  */
  dht.begin();
}

void loop() {
  readPressure();
  readTemperatureAndHumidity();
  readDistance();
}

void readDistance() {
  int x = analogRead(A0);
  int d = (6787 / (x - 3)) - 4;

  lcd.setCursor(10, 1);
  lcd.print(d);
  lcd.print(" cm");
}

void readTemperatureAndHumidity() {
  H = dht.readHumidity();
  T = dht.readTemperature();

  lcd.setCursor(0, 3);
  if (isnan(T) || isnan(H)) {  
    lcd.print("Error read DHT22");
    while(1);
  }

  lcd.print(T, 2);
  lcd.print(" C");
  lcd.setCursor(10, 3);
  lcd.print(H, 2);
  lcd.print(" %");
}

void readPressure() {
  char status;
  double t;

  if ((status = bmp.startTemperature()) != 0) {
    delay(status);

    if (bmp.getTemperature(t) != 0) {
      if ((status = bmp.startPressure(3)) != 0) {
        delay(status);

        if (bmp.getPressure(P, t) != 0) {
          lcd.setCursor(0, 1);
          lcd.print(t, 2);
          lcd.print(" C");
          lcd.setCursor(0, 2);
          lcd.print(P, 2);
          lcd.print(" hPa");
        } else {
          // error get pressure
          lcd.print("Error Get P");
        }
      } else {
        // error start pressure
        lcd.print("Error Start P");
      }
    } else {
      // error get temperature
      lcd.print("Error Get T BMP");
    }
  } else {
    // erro start temperature
    lcd.print("Error Start T BMP");
  }
}
