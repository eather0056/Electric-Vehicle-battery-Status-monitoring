#include <SimpleTimer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include <DHT.h> 
#include <TinyGPS++.h>
#include <ArduinoJson.h>
#define SAMPLES 300   
#define ACS_Pin A1
#define DHTPIN 2 

SimpleTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2); 
SoftwareSerial nodemcu(5, 6);
DHT dht(2, DHT11);


///----------------------------GPS----------------------------
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
//SoftwareSerial ss(RXPin, TXPin);

              float lat1 = 23.827911677596937;
              float lon1 = 90.41956186294557;
              float lat2=0;
              float lon2=0;


///-----------------------Voltage-------------------
float V=0;

  float percn=0;
  float C_mil=0;

//-----------------------------Current-------------------------------------
        float High_peak,Low_peak;        
        float Amps_Peak_Peak, Amps_RMS;
        
void setup() {
    Wire.begin();
    Serial.begin(9600);
    nodemcu.begin(9600);
    TinyGPSPlus gps;
    lcd.begin();
    lcd.backlight();
    dht.begin();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  !!EV-State!!");
}

void loop() {

  
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("EV-State");
                      delay(1500);
                      
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Battery Perc  ");
                      lcd.setCursor(5,1);
                      lcd.print(percn);
                      lcd.setCursor(9,1);
                      lcd.print("%");
                      delay(1500);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  KM Can Left  ");
                      lcd.setCursor(5,1);
                      lcd.print(C_mil);
                      lcd.setCursor(9,1);
                      lcd.print("Km");
                      delay(1500);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Voltage  ");
                      lcd.setCursor(5,1);
                      lcd.print(V);
                      lcd.setCursor(9,1);
                      lcd.print("V");
                      delay(1500);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Current  ");
                      lcd.setCursor(5,1);
                      lcd.print(Amps_RMS);
                      lcd.setCursor(9,1);
                      lcd.print("A");
                      delay(1500);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Temperature  ");
                      lcd.setCursor(5,1);
 //                     lcd.print(t);
                      lcd.setCursor(9,1);
                      lcd.print("`C");
                      delay(1500);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Nearest C. S.  ");
                      lcd.setCursor(5,1);
                      //lcd.print(Dis);
                      lcd.setCursor(9,1);
                      lcd.print("Dis");
                      delay(1500);

                      

}
