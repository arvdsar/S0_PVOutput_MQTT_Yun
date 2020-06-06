/*
 * Written bij Alexander - www.vdsar.net
 * partially based on Solarmeter or Harold65 and S0 Uploader of Dylantje
 * This version is only to be used with the Arduino Yun and will not work with e.g. an Arduino Mega
 * Data is send every 15 seconds to MQTT Topics. Using MQTT you can easily further process data using i.e. Node-Red or Domoticz
 * Data is send every 5 minutes to PVOutput
 * 
 * This version does not contain any smart meter (Slimme Meter) data. You could you DSMRReader on a Raspberry Pi to get the data from your smart meter.
 * 
 * You have to use an microSD card with the Arduino Yun. This saves your EEPROM from being ruined. Some data variables are stored on the SD Card for use
 * by this sketch only (not for other reading).
 * 
 * Additional information will follow.
 * 
 * version 1.4 - 06-06-2020: Working version with 1 S0 Reader, saving data to SDcard instead of EEPROM and ability to change totals/todaycnt using API of Arduino
 *                           Code could be cleaned a little in a future version.
 */


#define VERSION "Arduino Yun - PVO & MQTT S0 Uploader V1.4 06-6-2020 "   /


#include <MsTimer2.h>
#include "S0Sensor.h"
#include "userdefs.h"
#include <FileIO.h>
#include "files.h"


#include <Console.h>
#include <Process.h>
#include <TimeLib.h>

// global variables
byte   lastDayReset;
byte   lastHour;
byte   lastMinute;
byte   iDay;
byte   iHour;
byte   iMinute;
int    upTime;               // the amount of hours the Arduino is running
long   lastMillis;
long   lastUpdateMillis;
char   webData[70];
char   topic[30];
char   mqttvar[30];
char   total[10];
char   todayCnt[10];
char   save[2];
  
#define EE_RESETDAY 4


long getLocalTime() { // This function return a long with the time stamp and depends on pythonscript at linux side
  String result; //creates a result string
  Process time; //names the process Time
  time.begin("/mnt/sda1/localtime.py"); //call for Python localtime script on linux side
  time.run();  // run the command
  while (time.available() > 0) {   // read the output of the command
    char c = time.read();
    if (c != '\n')
      result += c;
  }
  
  return result.toInt();
}


  
void setup()
{
    
  Bridge.begin();
  Console.begin();
  FileSystem.begin();
  delay(3000);
  //Wait for Console port to connect
  //while (!Console);
 // Console.println("Start");
       
  setTime(getLocalTime()); //set time (get it from linux)
  Console.print("LocalTime set to:");
  Console.println( now() );


    // initialize the sensors
    for(byte i = 0; i < NUMSENSORS; i++)
    {
        sensors[i]->Begin(i);
    }
        
    // set a random seed
    randomSeed(analogRead(0));

    // restore the last day on which the counters were reset
    lastDayReset = ReadSD(EE_RESETDAY);
    // if the eeprom contains illegal data, set it to a useful value
    if(lastDayReset == 0 || lastDayReset > 31) lastDayReset = day();
    lastMinute = minute();
    lastHour = hour();
    upTime = 0;

    
    // start the timer interrupt
    MsTimer2::set(5, Every5ms); // 5ms period
    MsTimer2::start();
    


}

// check and update all counters every 5ms.
void Every5ms()
{
    for(byte i = 0; i < NUMSENSORS; i++)
    {
        sensors[i]->CheckSensor();
    }
}

void loop()
{
    
    // get the actual time
    iDay = day();
    iHour = hour();
    iMinute = minute();
    // reset counters when todays day is different from the last day the counters were reset
    if(iDay != lastDayReset)
    {
    for(byte i = 0; i < NUMSENSORS; i++)
        {
            sensors[i]->Reset();
        }
     lastDayReset = iDay;
        // store today as the date of the last counter reset
      WriteSD(EE_RESETDAY, lastDayReset);
    }

    // hour has changed
    // cannot simply check the change of an hour because 'updatetime' can also change the hour
    // therefore we also check that the minutes are 0
    if(iHour != lastHour && iMinute == 0)
    {
        lastHour = iHour;
        upTime++;
        // save the daily values every hour
        for(byte i = 0; i < NUMSENSORS; i++)
        {
            sensors[i]->Save();
        }
        // sync the time at fixed interval
     if(lastHour == 2 || lastHour == 6 || lastHour == 10 || lastHour == 14 || lastHour == 18 || lastHour == 22  )
        {
          setTime( getLocalTime() ); // update the time
        }
           } 

    // update every minute
    if(iMinute != lastMinute)
    {
        lastMinute = iMinute;
        for(byte i = 0; i < NUMSENSORS; i++)
        {
            sensors[i]->CalculateActuals();
        }

    // update every 5 minutes or whatever is set in userdefs
        if((lastMinute%UPDATEINTERVAL) == 0)
        {
          SendToPvOutput(sensors);
          Console.println("SendToPVoutput");

           // reset the maximum for pvoutput
            for(byte i = 0; i < NUMSENSORS; i++)
            {
                sensors[i]->ResetPeak();
            }
        }
      }
       // let all sensors do other stuff
    for(byte i = 0; i < NUMSENSORS; i++)
    {
      sensors[i]->Loop(lastMinute);
    }
      delay(50);
           
   
    
  // every xx milliseconds send an update of sensor data via MQTT
  // The SendToMQTT function will check if MQTT is connected again, but will not try to reconnect
  
   if(millis() - lastMillis > 15000) {
        lastMillis = millis();
        Console.println("SendToMQTT");

        SendToMQTT();  
       }

      
      //Check for new total/today value once in a minute
  if(millis() - lastUpdateMillis > 15000) {
    lastUpdateMillis = millis();
    checkAPI();
 
  }


}
