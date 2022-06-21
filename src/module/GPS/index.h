
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(4, 5) ;
float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str;
int pm;
void readGSP()
{
    while (ss.available() > 0)     // while data is available
        if (gps.encode(ss.read())) // read gps data
        {
            if (gps.location.isValid()) // check whether gps location is valid
            {
                latitude = gps.location.lat();
                lat_str = String(latitude, 6); // latitude location is stored in a string
                longitude = gps.location.lng();
                lng_str = String(longitude, 6); // longitude location is stored in a string
            }
            if (gps.date.isValid()) // check whether gps date is valid
            {
                date_str = "";
                date = gps.date.day();
                month = gps.date.month();
                year = gps.date.year();
                if (date < 10)
                    date_str = '0';
                date_str += String(date); // values of date,month and year are stored in a string
                date_str += " / ";

                if (month < 10)
                    date_str += '0';
                date_str += String(month); // values of date,month and year are stored in a string
                date_str += " / ";
                if (year < 10)
                    date_str += '0';
                date_str += String(year); // values of date,month and year are stored in a string
            }
            if (gps.time.isValid()) // check whether gps time is valid
            {
                time_str = "";
                hour = gps.time.hour();
                minute = gps.time.minute();
                second = gps.time.second();
                minute = (minute + 30); // converting to IST
                if (minute > 59)
                {
                    minute = minute - 60;
                    hour = hour + 1;
                }
                hour = (hour + 5);
                if (hour > 23)
                    hour = hour - 24; // converting to IST
                if (hour >= 12)       // checking whether AM or PM
                    pm = 1;
                else
                    pm = 0;
                hour = hour % 12;
                if (hour < 10)
                    time_str = '0';
                time_str += String(hour); // values of hour,minute and time are stored in a string
                time_str += " : ";
                if (minute < 10)
                    time_str += '0';
                time_str += String(minute); // values of hour,minute and time are stored in a string
                time_str += " : ";
                if (second < 10)
                    time_str += '0';
                time_str += String(second); // values of hour,minute and time are stored in a string
                if (pm == 1)
                    time_str += " PM ";
                else
                    time_str += " AM ";
            }
        }
    Serial.print("Niali : ");
    Serial.println(ss.available());
}