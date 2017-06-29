#include <TinyGPS++.h>
#include <String.h>
#include <SoftwareSerial.h>

float oncekienlem=0.0;
float oncekiboylam=0.0;

  static const int RXPin = 4, TXPin = 3;
  SoftwareSerial Serial2(RXPin,TXPin);  //gps
  
  static const uint32_t GPSBaud = 9600;
  SoftwareSerial Serial1(8, 7);  //gsm
  
 
 TinyGPSPlus shield;
 float latitude, longitude;
  void setup()
  {



  Serial1.begin(19200);///GSM
  Serial2.begin(9600);///GPS
  Serial.begin(19200);

 
  }

void SendSQL()
{

 //Serial.println("sendSQL");
Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
delay(500);
 Serial1.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
delay(500);
Serial1.println("AT+SAPBR=1,1");
delay(500);
Serial1.println("AT+SAPBR=2,1");
delay(3000);
Serial1.println("AT+HTTPINIT");
delay(500);
Serial1.println("AT+HTTPPARA=\"CID\",1");
delay(500);
Serial1.print("AT+HTTPPARA=\"URL\",\"http://kodakademi.tv/odev01/add_data.php?gelenevent=tarih&gelenboylam=shield.location.lat()&gelenenlem=shield.location.lng()");
Serial1.print("&gelenenlem=");
Serial1.print(shield.location.lat(),5); 
Serial1.print("&gelenboylam=");
Serial1.print(shield.location.lng(),5);
Serial1.print("&gelenevent=");
char tarih[32];

sprintf(tarih,"%02d-%02d-%02d %02d:%02d:%02d",shield.date.year(),shield.date.month(),shield.date.day(),shield.time.hour(),shield.time.minute(),shield.time.second());
Serial1.print(tarih);
Serial1.println("\"");
delay(1000);

delay(1000);
Serial1.println("AT+HTTPACTION=0");
delay(15000);
Serial1.println("AT+HTTPTERM");
delay(500);
Serial1.println("AT+SAPBR=0,1");
delay(500);
Serial.println("Databaseye Gonderildi");
}

void SendTextMessage() // mesaj atar
{
  Serial.println("Sending Text...");
  Serial1.print("AT+CMGF=1\r"); 
  delay(100);
 
  Serial1.println("AT+CMGS = \"+90507*******\"");
  delay(100);
  Serial1.println("CIHAZ  KONUM BILGISI");
  delay(50);
  Serial1.print("ENLEM: "); 
  delay(50);
  Serial1.print(shield.location.lat(),5); 
  Serial1.print(" BOYLAM: "); 
  Serial1.println(shield.location.lng(),5);
  delay(100);
  Serial1.print((char)26);
  delay(100);
  Serial1.println();
  Serial.println("Text Sent.");
 
}
 
 
  
  void getgps(TinyGPSPlus &gps)
{

  
  
  
 
  Serial.println("--------------------------------------------------------------");
  Serial.print("ENLEM: "); 
  Serial.print(shield.location.lat(),6); 
  Serial.print(" BOYLAM: "); 
 Serial.println(shield.location.lng(),6);
 
 //if((latitude != oncekienlem)|| (longitude != oncekiboylam))
 //{
 //oncekienlem=latitude;
 //oncekiboylam=longitude;
 SendSQL();
 //}


}
  static void printDateTime()
{
  

    char tarih[32];
    sprintf(tarih,"%02d-%02d-%02d",shield.date.year(),shield.date.month(),shield.date.day());
    char zaman[32];
    sprintf(zaman,"%02d:%02d:%02d",shield.time.hour(),shield.time.minute(),shield.time.second());
    Serial.println(tarih);
    Serial.println(zaman);

  }

 

 
void loop()
{

  
delay(1000);
  
  byte a;
  if ( Serial2.available()>0 ) 
  {

    
    a = Serial2.read(); 
    if(shield.encode(a)) 
    {
      
      getgps(shield); 
      printDateTime();
      
    }
  }
 
//mesaj gonderme
 if (Serial.available()) 
   switch(Serial.read()) 
   {
     case 'P': 
       SendTextMessage(); 
       break;
      case 'dD': // 
       SendSQL(); // 
       break;
 
   } 

  
}
 


