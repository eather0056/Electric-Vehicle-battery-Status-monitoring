#include <SimpleTimer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include <DHT.h> 
#include <TinyGPS++.h>
#define SAMPLES 300   
#define ACS_Pin A1
#define DHTPIN 2    

SimpleTimer timer;
LiquidCrystal_I2C lcd(0x20, 16, 2); 
DHT dht(2, DHT11);

///----------------------------GPS----------------------------
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
//SoftwareSerial ss(RXPin, TXPin);

              float lat1 = 23.827911677596937;
              float lon1 = 90.41956186294557;
              float lat2 = 23.822116179236765;
              float lon2 = 90.42760848999025;

///-----------------------Voltage-------------------
  double V=0;
  int offset = 18.7;
  float percn=0;
  float C_mil=0;

//-----------------------------Current-------------------------------------
        float High_peak,Low_peak;        
        float Amps_Peak_Peak, Amps_RMS;

int green = 7;
int red = 8;
int relay = 9;

////////////Data Transfer///////////////////////////////

          float percn_N,C_mil_N,V_N,Amps_RMS_N,t_N,Dis_N;

void setup() {

     Wire.begin();
     Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    dht.begin();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  !!EV-State!!");
    pinMode(green,OUTPUT);
    pinMode(red,OUTPUT);
    pinMode(relay,OUTPUT);
    
}

void loop() {

 
    Amps_RMS =0.78;
    V = 10.77;
      //-----------------------------percentage----------------------------------------------
      percn = (V/12)*100;
      C_mil = percn * 1.7; 
  lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("   !!EV-State!!");
                      delay(2000);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Temperature  ");
                      lcd.setCursor(5,1);
                      lcd.print("27.5");
                      lcd.setCursor(9,1);
                      lcd.print("`C");
                      delay(2000);

                      lcd.setCursor(0,0);
                      lcd.print("  Nearest C. S.  ");
                      lcd.setCursor(5,1);
                      lcd.print("1.92");
                      lcd.setCursor(9,1);
                      lcd.print("km");
                      delay(2000);
   
                      
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Battery Perc  ");
                      lcd.setCursor(5,1);
                      lcd.print(percn);
                      lcd.setCursor(9,1);
                      lcd.print("%");
                      delay(2000);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  KM Can Go  ");
                      lcd.setCursor(5,1);
                      lcd.print(C_mil);
                      lcd.setCursor(9,1);
                      lcd.print("Km");
                      delay(2000);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("    Voltage  ");
                      lcd.setCursor(5,1);
                      lcd.print(V);
                      lcd.setCursor(9,1);
                      lcd.print("V");
                      delay(2000);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("    Current  ");
                      lcd.setCursor(5,1);
                      lcd.print(Amps_RMS);
                      lcd.setCursor(9,1);
                      lcd.print("A");
                      delay(2000);
///////////////////////// Temperature///////////////////////////////////

      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(h) || isnan(t)) 
         {
             //Serial.println("Failed to read from DHT sensor!");
              return;
         }
      float D = (random(0-50)/100);
      float Dis = (acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon2-lon1))*637.1)-2-D; 
            
///////////////////////////////Alarm///////////////////////////////////////
            if(t>35)
            {
              digitalWrite(green,LOW);
              digitalWrite(relay,LOW);
              digitalWrite(red,HIGH);
              }
              else
              {
              digitalWrite(green,HIGH);
              digitalWrite(relay,HIGH);
              digitalWrite(red,LOW);
              }
              


//////////////////////////////////Display/////////////////////////////////

                    

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Temperature  ");
                      lcd.setCursor(5,1);
                      lcd.print(t);
                      lcd.setCursor(9,1);
                      lcd.print("`C");
                      delay(2000);

                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Nearest C. S.  ");
                      lcd.setCursor(5,1);
                      lcd.print("1.92");
                      lcd.setCursor(9,1);
                      lcd.print("km");
                      delay(2000);
   

////////////////////////////////////////Serial/////////////////////////////////////////
                       Serial.println("   !!EV-State!!");
                       
                       Serial.println("  Battery Perc  ");
                       Serial.print(percn);
                       Serial.print("%");
                       
//                       Serial.println("  KM Can Left  ");
//                       Serial.print(C_mil);
//                       Serial.print("Km");
                       
                       Serial.println("    Voltage  ");
                       Serial.print(V);
                       Serial.print("V");
                                              
                       Serial.println("    Current  ");
                       Serial.print(Amps_RMS);
                       Serial.print("A");
                       
                       Serial.println("  Temperature  ");
                       Serial.print(t);
                       Serial.print("`C");
                       
                       Serial.println("  Nearest C. S.  ");
                       Serial.print("1.92");
                       Serial.print("km");
                       
}
