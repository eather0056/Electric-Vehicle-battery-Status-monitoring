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
LiquidCrystal_I2C lcd(0x27, 16, 2); 
SoftwareSerial nodemcu(5, 6);
DHT dht(2, DHT11);

///----------------------------GPS----------------------------
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
//SoftwareSerial ss(RXPin, TXPin);

              float lat1 = 23.827911677596937;//random(22, 24);
              float lon1 = 90.41956186294557;//random(88, 91);
              float lat2 = 23.822116179236765;//random(22, 24);
              float lon2 = 90.42760848999025;//random(88, 91);

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
int Vib = 11;

////////////Data Transfer///////////////////////////////

          float percn_N,C_mil_N,V_N,Amps_RMS_N,t_N,Dis_N;

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
    
    pinMode(green,OUTPUT);
    pinMode(red,OUTPUT);
    pinMode(relay,OUTPUT);
    pinMode(Vib,INPUT);
    
}

void loop() {
/////////////////////Electrical Parameter ///////////////////////////

//----------------------------Current---------------------------------------------
 read_Amps();               
  Amps_RMS = Amps_Peak_Peak*0.3536*0.06;     //Now we have the peak to peak value normally the formula requires only multiplying times 0.3536                    
                                           
      if(Amps_RMS < 0.2)
         {Amps_RMS =0;}                                       
 // Serial.println(Amps_RMS);                   
  delay(200);
//-------------------------------Voltage-------------------------------------------------
int volt = analogRead(A0);
 V = map(volt,0,1023,0,2200);

 V /=100;
 V = V+1;


  //-----------------------------percentage----------------------------------------------
  percn = (V/13)*100;
  C_mil = percn * 1.7; 

///////////////////////// Temperature///////////////////////////////////

      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(h) || isnan(t)) 
         {
             //Serial.println("Failed to read from DHT sensor!");
              return;
         }

///----------------------------Distance----------------------------


//              while (ss.available() > 0){
//                gps.encode(ss.read());
//                if (gps.location.isUpdated()){
//                  Serial.print("Latitude= "); 
//                  Serial.print(gps.location.lat(), 6);
//                  //String myString = gps.location.lat();
//                 
//                 lat2 = gps.location.lat();
//                 lon2 = gps.location.lng();
//              
//                  Serial.print(" Longitude= "); 
//                  Serial.println(gps.location.lng(), 6);
//                }
//              }
              float D = (random(0,50)/100);
   
      float Dis = (acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon2-lon1))*637.1)-2-D; 
//  float Dis = (random(200,500)/100)-0.45;
//////////////////////////////Vibration///////////////////////////////////
            int val;
            val=digitalRead(Vib);
            Serial.println("v");
Serial.print(val);

            
///////////////////////////////Alarm///////////////////////////////////////
            if(t>31)
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
                      lcd.print("   !!EV-State!!");
                      delay(2000);
                      
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("  Battery Perc  ");
                      lcd.setCursor(5,1);
                      lcd.print(percn);
                      lcd.setCursor(9,1);
                      lcd.print("%");
                      delay(2000);

                      if (percn>25)
                      {
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Cell is Balanced");
                      delay(2000);
                      } 
                      if (percn<25)
                      {
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Cell is Unbalanced");
                      delay(2000);
                      }
                      
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  KM Can Left  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(C_mil);
//                      lcd.setCursor(9,1);
//                      lcd.print("Km");
//                      delay(2000);

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
                      lcd.print(Dis);
                      lcd.setCursor(9,1);
                      lcd.print("km");
                      delay(2000);

                       if(val == 0)
                        {
                          lcd.clear();
                          lcd.setCursor(0,0);
                          lcd.print("Collusion happened");
                          digitalWrite(red,HIGH);
                          delay(1000);
                          digitalWrite(red,LOW);
                          delay(1000);
                         }
   

       nodemcu.print(percn);  nodemcu.print("A");
       nodemcu.print(C_mil);  nodemcu.print("B");
       nodemcu.print(V);  nodemcu.print("C");
       nodemcu.print(Amps_RMS);  nodemcu.print("D");
       nodemcu.print(t);  nodemcu.print("E");
       nodemcu.print(Dis);  nodemcu.print("F");
       nodemcu.print("\n");
       delay(500);
}


void read_Amps()           
{                           
  int cnt;            
  High_peak = 0;      
  Low_peak = 1024;
  
      for(cnt=0 ; cnt<SAMPLES ; cnt++)       
      {
        float ACS_Value = analogRead(ACS_Pin); 

        
        if(ACS_Value > High_peak)              
            {
              High_peak = ACS_Value;           
            }
        
        if(ACS_Value < Low_peak)               
            {
              Low_peak = ACS_Value;            
            }
      }                                       
      
  Amps_Peak_Peak = High_peak - Low_peak;      
}
