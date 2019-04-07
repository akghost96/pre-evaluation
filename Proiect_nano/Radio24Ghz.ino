#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

//RF24 radio(2, 13);

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";

void setupRadio24Ghz()
{  
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr); 
  radio.stopListening();
  radio.printDetails();
}
void loopRadio24Ghz()
{
  const char text[] = "Hello World_Ok213";
  radio.write(&text, sizeof(text));
  Serial.println(text);
  delay(1000);
}
void SendRadio24Ghz(String str)
{  
  const char text[str.length() + 1] ;
  strcpy(text, str.c_str());
  radio.write(&text, strlen(text));        
}
