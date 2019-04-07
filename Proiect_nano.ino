
String LAT_LONG="";
#include <Wire.h>
#include "RTClib.h"

char sID[7] = "1";


//nou
const int analogPin0 = A0;
const int analogPin1 = A1;

int pasi=0;

RTC_DS1307 RTC;
DateTime now;
long old_unixtime = 0;
int pas_unixtime = 0;
int vibr_Pin = 3;
int mesaj_pin = 12;
char m1[25] = "";
char m2[25] = "";
char m3[25] = "";
char m4[25] = "";
char m5[25] = "";
char m6[25] = "";
char m7[25] = "";



void Trimite(String mesaj);
void setup() {
pinMode(LED_BUILTIN, OUTPUT);
  pinMode(mesaj_pin, OUTPUT);
   digitalWrite(mesaj_pin, HIGH);
 //  pinMode(9, OUTPUT);
 //  digitalWrite(9, HIGH);
  delay(100);
 // digitalWrite(mesaj_pin, LOW);
  //  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  //digitalWrite(LED_BUILTIN, LOW);
//  lcd.begin(); //
//  lcd.clear();
//change the contrast ratio to maximum
for( int darkness=0 ; darkness<32; darkness++)
{
  //lcd.contrast(darkness);
//  digitalWrite(mesaj_pin, HIGH);
//  delay(100);
//  digitalWrite(mesaj_pin, LOW);
}

//change the contrast ratio to minimum
for( int darkness=32 ; darkness>1; darkness--)
{
  //lcd.contrast(darkness);
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(100);
//  digitalWrite(LED_BUILTIN, LOW);
}

//lcd.clear();
//lcd.line(0);

  
  /*
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(2,INPUT);//set pin2 as INPUT tilt inclinare
    digitalWrite(2, HIGH);//set pin2 as HIGH tilt inclinare
    pinMode(vibr_Pin, INPUT); //set vibr_Pin input for measurment

    pinMode(LED_BUILTIN, OUTPUT);
    Wire.begin();
    RTC.begin();
    // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
    if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    }
  */
  ////
  Serial.begin(115200);
//  Serial3.begin(9600);
  //Trimite("[START]","SYSTEM");
  // put your setup code here, to run once:
Serial.println("STARTTTTTTT");
delay(1000);
  setupRadio24Ghz();
  Serial.println("RADIO ON");
  //loopRadio24Ghz();
  Trimite("[INIT]","Radio2.4Ghz");
  delay(1000);
 //   Trimite("[START]","GPS"); setupGPS();  Trimite("[INIT]","GPS");
  delay(1000);
  //  setupMAGNETIC();
  //setupGSM();
  setupGPS();
  //loopGSM();
//  lcd.clear();
//lcd.line(0);
  Trimite("Initializare", "... terminata");
  //setupVolti();
//  setupDHT();
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("RADIO ON 2");
 // lcd.displayNormal();
}

void loop() {
  //  loopVolti();
//    loopDHT();
  // put your main code here, to run repeatedly:
  delay(500);
 // if(pasi==1000)
  {
  loopGPS();
  pasi=0;
  }
//  else
  {
    //delay(100);
   int val0 = analogRead(analogPin0);
  int val1 = analogRead(analogPin1);
  Trimite("[RF]",String(val0+val1,6));
    }
  pasi++;
  //loopGSM();
  //loopGSM2();
  //loopTilt2();
  //loopVibratie3();
  //loopMAGNETIC();
  //Data si Ora
  //Trimite("[RTC DateTime] 2", String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
  
}

void Trimite(String titlu, String mesaj)
{
  char m1[25] = "";
char m2[25] = "";
char m3[25] = "";
char m4[25] = "";
char m5[25] = "";
char m6[25] = "";
char m7[25] = "";
  //igitalWrite(LED_BUILTIN, HIGH);
  //digitalWrite(mesaj_pin, HIGH);

  //now=RTC.now();
  if (now.unixtime() == old_unixtime)
  {
    pas_unixtime = pas_unixtime + 1;
  }
  else
  {
    pas_unixtime = 0;
    old_unixtime = now.unixtime();
    //digitalWrite(mesaj_pin, HIGH);

  }
  String s=String(pas_unixtime);
  titlu.toCharArray(m6, 25);
  mesaj.toCharArray(m7, 25);
  s.toCharArray(m5, 25);
  
  //lcd.line(5); lcd.print(m5);
  //lcd.line(6); lcd.print(m6);
  //lcd.line(7); lcd.print(m7);
  //titlu = "{" + String(now.unixtime()) + String(pas_unixtime) + "}-" + titlu;
  //mesaj = "{" + String(now.unixtime()) + String(pas_unixtime) + "}-" + mesaj;

  titlu = "{" + String(sID) + "}-" + titlu;
  mesaj = "{" + String(sID) + "}-" + mesaj;
  //mesaj = "{" + LAT_LONG + "}" + mesaj;
  // if (Serial.available())
  {
    Serial.println(titlu); Serial.println(mesaj);
  }

  SendRadio24Ghz(titlu);SendRadio24Ghz(mesaj);


  //strncpy(m7,buffer,25);
  /*
  lcd.line(1); lcd.print(m1);
  lcd.line(2); lcd.print(m2);
  lcd.line(3); lcd.print(m3);
  lcd.line(4); lcd.print(m4);
  lcd.line(5); lcd.print(m5);
  lcd.line(6); lcd.print(m6);
  lcd.line(7); lcd.print(m7);
  */
  
 
//  if (Serial3.available())
  {
//    Serial3.println(titlu); Serial3.println(mesaj);
  }
  //delay(500);
  //digitalWrite(LED_BUILTIN, LOW);
  //digitalWrite(mesaj_pin, LOW);
  //delay(500);
}

void loopTilt2()
{
  int digitalVal = digitalRead(2);//Read the value of pin2
  if (HIGH == digitalVal) //if tilt switch is not breakover
  {
    //digitalWrite(ledPin,LOW);//turn the led off
    Trimite("[Inclinat2]", String(digitalVal));
  }
  else ////if tilt switch breakover
  {
    //digitalWrite(ledPin,HIGH);//turn the led on
    Trimite("[Inclinat2]", String(digitalVal));
  }
}
void loopVibratie3()
{
  long measurement = TP_init();
  delay(50);
  // Serial.print("measurment = ");
  //Serial.println(measurement);
  Trimite("[Vibratie]", String(measurement));
  if (measurement > 1000) {
    //digitalWrite(LED_Pin, HIGH);
  }
  else {
    //digitalWrite(LED_Pin, LOW);
  }
}
long TP_init() {
  delay(10);
  long measurement = pulseIn (vibr_Pin, HIGH); //wait for the pin to get HIGH and returns measurement
  return measurement;
}
