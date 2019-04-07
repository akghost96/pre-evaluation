/*
DC Voltmeter Using a Voltage Divider
Based on Code Created By
T.K.Hareendran
*/
int relay1 = 11;
int relay2 = 12;
int relay3 = 13;
int relay4 = 14;


int analogInput = A1;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; // 
int value = 0;

/////
const int analogIn = A0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;


void setupVolti(){
   pinMode(analogInput, INPUT);  
    pinMode(relay1, OUTPUT);
   

}
void loopVolti(){
  digitalWrite(relay1, HIGH);
   // read the value at analog input
   value = analogRead(analogInput);
   vout = (value * 5.0) / 1024.0; // see text
   vin = vout / (R2/(R1+R2)); 
   
Trimite("[INPUT V=]",String(vin));

RawValue = analogRead(analogIn);
 Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
 Amps = ((Voltage - ACSoffset) / mVperAmp);

Trimite("[INPUT AV=]",String(RawValue));
Trimite("[INPUT mV=]",String(Voltage));
Trimite("[INPUT A=]",String(Amps));
 /*
  *Serial.print("Raw Value = " ); // shows pre-scaled value 
 Serial.print(RawValue); 
 Serial.print("\t mV = "); // shows the voltage measured 
 Serial.print(Voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
 Serial.print("\t Amps = "); // shows the voltage measured 
 Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
  */

}
