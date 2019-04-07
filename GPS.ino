#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
//tx rx static const int RXPin = 4, TXPin = 3;

static const int RXPin = 5, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

static String GPSDateTime="00/00/0000 00:00:00";


// The TinyGPS++ object
//TinyGPSPlus gps;

// The serial connection to the GPS device
//SoftwareSerial ss(RXPinGPS, TXPinGPS);
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t);

void setupGPS()
{
  //Serial.begin(9600);
  ss.begin(GPSBaud);
 // Serial.begin(GPSBaud);  
  Trimite("[INIT_GPS]","v. "+String(TinyGPSPlus::libraryVersion()));  
  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail"));
  Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
}
void loopGPS()
{
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  if(gps.satellites.isValid())
  {
    Trimite("[GPS-Satelites]",String(gps.satellites.value()));
    }
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  if(gps.hdop.isValid())
  {
    Trimite("[GPS-HDOP]",String(gps.hdop.hdop(),6));
    }
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  if(gps.location.isValid())
  {
    Trimite("[GPS-LAT]",String(gps.location.lat(),6));
    Trimite("[GPS-LNG]",String(gps.location.lng(),6));
    Trimite("[GPS-AGE]",String(gps.location.age()));
    LAT_LONG=String(gps.location.lat(),6)+"-"+String(gps.location.lng(),6)+"-"+String(gps.altitude.meters(),2);
    }
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);  
  if(gps.altitude.isValid())
  {
    Trimite("[GPS-ALT]",String(gps.altitude.meters(),2));
    }
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  if(gps.course.isValid())
  {
    Trimite("[GPS-Course]",String(gps.course.deg()));
    }
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  if(gps.speed.isValid())
  {
    Trimite("[GPS-Speed]",String(gps.speed.kmph()));
    }
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON) / 1000;
  printInt(distanceKmToLondon, gps.location.isValid(), 9);

  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON);

  printFloat(courseToLondon, gps.location.isValid(), 7, 2);

  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

  printInt(gps.charsProcessed(), true, 6);
  Trimite("[GPS-CharsRX]",String(gps.charsProcessed()));
  printInt(gps.sentencesWithFix(), true, 10);
  Trimite("[GPS-SentencesRX]",String(gps.sentencesWithFix()));
  printInt(gps.failedChecksum(), true, 9);
  Trimite("[GPS-ChkSumFail]",String(gps.failedChecksum()));
  Serial.println();

  //smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
    Trimite("[GPS-DateTime]",String(GPSDateTime));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
/////////////////////////////////////////////////////      while (Serial.available())
/////////////////////////////////////////////////////      gps.encode(Serial.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
  //  while (len-- > 1)
  //    Serial.print('*');
  //  Serial.print(' ');
  }
  else
  {
    //Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    //for (int i = flen; i < len; ++i)
     // Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  //Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{  
  char szD[32];
  char szT[32];
  if (!d.isValid())
  {
    //Serial.print(F("********** "));
  }
  else
  {
    
    sprintf(szD, "%02d/%02d/%02d ", d.day(),d.month(), d.year());
    //Serial.print(szD);
    
  }

  if (!t.isValid())
  {
    //Serial.print(F("******** "));
  }
  else
  {    
    sprintf(szT, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    //Serial.print(szT);
    
  }  
  if(d.isValid()){
  GPSDateTime=String(szD)+String(szT);
  }
//  sprintf(GPSDateTime,"%s-%s",szD,szT);
  printInt(d.age(), d.isValid(), 5);
  if(d.isValid())
  {
  Trimite("[GPS-DateAge]",String(d.age()));
  }
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    //Serial.print(i < slen ? str[i] : ' ');
  smartDelay(0);
}
