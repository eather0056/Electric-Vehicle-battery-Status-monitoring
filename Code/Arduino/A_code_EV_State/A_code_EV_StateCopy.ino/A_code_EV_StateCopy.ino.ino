include <SimpleTimer.h>
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
 


  //-----------------------------percentage----------------------------------------------
  percn = (V/12)*100;
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
              
   
      float Dis = (acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon2-lon1))*637.1)-2; 

            
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
              


////////////////////////////////////Display/////////////////////////////////
//
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("EV-State");
//                      delay(1000);
//                      
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  Battery Perc  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(percn);
//                      lcd.setCursor(9,1);
//                      lcd.print("%");
//                      delay(1000);
//
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  KM Can Left  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(C_mil);
//                      lcd.setCursor(9,1);
//                      lcd.print("Km");
//                      delay(1000);
//
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  Voltage  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(V);
//                      lcd.setCursor(9,1);
//                      lcd.print("V");
//                      delay(1000);
//
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  Current  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(Amps_RMS);
//                      lcd.setCursor(9,1);
//                      lcd.print("A");
//                      delay(1000);
//
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  Temperature  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(t);
//                      lcd.setCursor(9,1);
//                      lcd.print("`C");
//                      delay(1000);
//
//                      lcd.clear();
//                      lcd.setCursor(0,0);
//                      lcd.print("  Nearest C. S.  ");
//                      lcd.setCursor(5,1);
//                      lcd.print(Dis);
//                      lcd.setCursor(9,1);
//                      lcd.print("km");
//                      delay(1000);

           //  /////////////////////////////Data Transfer////////////////////////////
            StaticJsonBuffer<2000> jsonBuffer;
            JsonObject& data = jsonBuffer.createObject();

//            Assign collected data to JSON Object
              data["percn_N"] =percn;
              data["C_mil_N"] =C_mil;
              data["V_N"] =V;
              data["Amps_RMS_N"] =Amps_RMS;
              data["t_N"] =t;
              data["Dis_N"] =Dis;
              
            //Send data to NodeMCU
            Serial.print(" JSON"); 
            data.printTo(nodemcu);
            jsonBuffer.clear();
          
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
