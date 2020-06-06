#ifndef BaseSensor_h
#define BaseSensor_h

#include "Arduino.h"

#include <FileIO.h>


class BaseSensor
{
  public:
  BaseSensor(int ppu, long sid, int f);// constructor
  virtual void Begin(byte index);              // initialize all variables
  virtual void CheckSensor();                  // check the input and update counters (called from 5ms interrupt)
  virtual void Loop(int m);                    // Called from main loop				
  virtual void CalculateActuals();             // Convert all counters according to the pulses per unit
  virtual void Reset();                        // Reset day-counter
  virtual void Save();                         // Save day-counter
  virtual void Update(long Value);             // Update and save the day counter
  virtual void NewTotal(long value);           // Update and save the total counter
  virtual void ResetPeak();                    // reset peak so new peak measurement can start
  virtual long ActualVal();                    // Return Actual as string (for use with MQTT)
  virtual long PeakVal();                      // Return Peak as string (for use with MQTT)
  virtual long TodayVal();                     // Return Today as string (for use with MQTT)
  virtual long TotalVal();                     // return Total Lifetime as String (for use with MQTT)
  virtual long aReadSD(int id);
  virtual long aWriteSD(int id, long getal);

  long Midnight;                               // The total counter value at the last midnight
  long Today;                                  // Total for today in correct units. Reset at midnight
  long Actual;                                 // Actual measured value in correct units
  long Peak;                                   // Peak value of the last period
  long SID;                            // System id where this sensor logs to
  byte Type;                                   // Variable of PvOutput to log to. See userdefs.h for explanation  
  int Factor;                                  // Values will be divided by this number before sending them to pvOutput
  
  protected:
  long todayCnt;                               // today's pulse count
  int  ppu;                                    // the pulses per unit (kWh or m3) for this counter
  long lastMillis;                             // the time of the last pulse (ms)
  long pulseLength;                            // the time between the last two pulses (ms)
  int ee;                                      // the address to store the day counter in case of a reset
  int ee2;                                     // the address to store the total counter in case of a reset
  int prevPulse;  //demo stuff
};

#endif
