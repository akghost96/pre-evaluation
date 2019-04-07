//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
static DateTime now;

//RF24 radio(7, 8);
RF24 radio(3, 2);

const byte rxAddr[6] = "00001";


/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNONE;  // when all others fail, return this...
}
String old_amprenta="";
String amprenta="";
    String Atribut="";
    String Valoare="";
    int pas=10;

void setup()
{
  
  printf_begin();
 // while (!Serial);
   Serial.begin(115200);
  
  radio.begin();  
  //radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, rxAddr);  
  radio.startListening();
  radio.printDetails();
  radio.getDataRate();
  ////
  Wire.begin();
    RTC.begin();
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) {
  //  Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }    
  ////
   lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Pornit"); // print a simple message
 if (radio.available())
  {
    lcd.print("+RF");
  }
  else
  {
    lcd.print(":(");
  }
  
}

void loop()
{
  
  if (radio.available())
  {
    //lcd.clear();
    char text[255] = {0};
    radio.read(&text, sizeof(text));
    //Serial.println(strlen(text));
    //Serial.println(text);
    //
    
    String msg=String(text);
    
    if(msg.indexOf('{')!=-1){
    amprenta=msg.substring(msg.indexOf('{')+1,msg.indexOf('}'));    
    msg=msg.substring(msg.indexOf('}')+2);    
    }
    if(msg.indexOf('[')!=-1){
    Atribut=msg.substring(msg.indexOf('[')+1,msg.indexOf(']'));    
    }
    else
    {
      Valoare=msg;      
      }
      
    if(old_amprenta!=amprenta)
    {
      old_amprenta=amprenta;
      }
      else
      {
        lcd.setCursor(0,0);
    lcd.print(Atribut); // print a simple message
    lcd.setCursor(0,1);
 lcd.print(Valoare); // print a simple message
 Serial.println(Valoare);
 delay(50);
        }    
        lcd.setCursor(0,1);            // move to the begining of the second line
lcd.print(msg); // print a simple message

  }
  


 
 lcd_key = read_LCD_buttons();  // read the buttons
 lcd.setCursor(0,1);            // move to the begining of the second line
//lcd.print(msg); // print a simple message
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     pas+=10;
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     pas-=10;
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     //lcd.print("NONE  ");
     break;
     }
 }
 delay(pas);
}
