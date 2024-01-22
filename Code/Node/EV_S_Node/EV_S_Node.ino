#include "ThingSpeak.h"
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <Wire.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(4, 2);

    unsigned long ch_no = 1948702;
    const char * write_api = "B8E4CHR8GOCDSV6C";
    const char * read_api = "8N4J3GCB9CTRXJ7W";
    String apiKey = "B8E4CHR8GOCDSV6C"; 

       char auth[] = "mwa0000028407168";
       char ssid[] = "EV State"; 
       char pass[] = "borno1234";

//       char ssid[] = "POCO M2"; 
//       char pass[] = "0987654321";
       
const char* server = "api.thingspeak.com";

    unsigned long startMillis;
    unsigned long currentMillis;
    const unsigned long period = 10000;

    WiFiClient client;
    
 float CB;

char c;
String dataIn;
int8_t indexOfA,indexOfB,indexOfC,indexOfD,indexOfE,indexOfF;
String percn,C_mil,V,Amps_RMS,t,Dis;
 
void setup() {
 
  Serial.begin(9600);
  nodemcu.begin(9600);
  
  while (!Serial) continue;
   WiFi.begin(ssid, pass);
          while (WiFi.status() != WL_CONNECTED)
          {
            delay(500);
            Serial.print(".");
            delay(2000);
          }
              Serial.println("WiFi connected");
              Serial.println(WiFi.localIP());
              delay(2000);
              ThingSpeak.begin(client);
              startMillis = millis();
              
}

void loop() {

        while (nodemcu.available()>0)
        {
        c = nodemcu.read();
        if(c=='\n') {break;}
        else        {dataIn+=c;}
        }
        
        if(c=='\n') 
        {
          Parse_the_Data();
          c=0;
          dataIn="";
          }
          
      Serial.print(percn);
      Serial.print(C_mil);
      Serial.print(V);
      Serial.print(Amps_RMS);
      Serial.print(t);
      Serial.print(Dis);
      delay(500);
           
ThingSpeak.writeField(ch_no,1,percn,write_api);
ThingSpeak.writeField(ch_no,2,t,write_api);
ThingSpeak.writeField(ch_no,3,C_mil,write_api);
ThingSpeak.writeField(ch_no,4,Dis,write_api);

}

void Parse_the_Data()
{
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  indexOfD = dataIn.indexOf("D");
  indexOfE = dataIn.indexOf("E");
  indexOfF = dataIn.indexOf("F");

  percn    = dataIn.substring (0, indexOfA);
  C_mil    = dataIn.substring (indexOfA+1, indexOfB);
  V        = dataIn.substring (indexOfB+1, indexOfC);
  Amps_RMS = dataIn.substring (indexOfC+1, indexOfD);
  t        = dataIn.substring (indexOfD+1, indexOfE);
  Dis      = dataIn.substring (indexOfE+1, indexOfF);
  }
