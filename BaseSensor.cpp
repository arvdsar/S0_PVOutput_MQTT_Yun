#include "BaseSensor.h"

BaseSensor::BaseSensor(int p,long sid, int f)
{
    ppu = p;
    SID = sid;
    Factor = f;
}

void BaseSensor::Begin(byte i)
{
    Actual = 0;
    Peak = 0;
    Today = 0;
    ee = (i+20) * 4; // it was the eeprom address of this sensor where the last value is saved, now it is the SDcard filename
    todayCnt = aReadSD(ee); 
    ee2 = (i+40) * 4; // it was the eeprom address for the total counters, now it is the SDcard filename
    Midnight = aReadSD(ee2);
    pulseLength = 0;
    lastMillis = 0;
    prevPulse = 0; //demo stuff
}

void BaseSensor::CheckSensor()
{
    // Check sensor must be done by the derived sensor
}

void BaseSensor::Loop(int m)
{
    // Derived sensors can execute non time critical actions here
}

void BaseSensor::Save()
{
    aWriteSD(ee, todayCnt); 
}

void BaseSensor::Update(long Value)
{
    todayCnt = Value;
    Save();
}

void BaseSensor::NewTotal(long value)
{
    // store the value of totalcounter at last midnight
    Midnight = value - Today;
    aWriteSD(ee2, Midnight);
}

void BaseSensor::Reset()
{
    // update the totals
    Midnight += Today;
    // store the new totalcounter
    aWriteSD(ee2, Midnight);
    // reset the day counter
    todayCnt = 0;
    Today = 0;
}

void BaseSensor::ResetPeak()
{
    Peak = 0;
}

void BaseSensor::CalculateActuals()
{
    // Was the last BaseSensor pulse more than 5 minutes ago?
    if(millis() - lastMillis > 300000)
    {
        Actual = 0;  // then we have no output
    }
    else
    {
        if(pulseLength != 0)  // prevent division by zero
        {
            // convert to W
            Actual = 3600000000 / ppu;
            Actual /= pulseLength;
            //Peak = Actual; //FOR testing
            if(Peak < abs(Actual)) Peak = Actual;
        }
    }
    Today = todayCnt * 1000 / ppu;
}



long BaseSensor::ActualVal(){
    return Actual/Factor;
  }

long BaseSensor::PeakVal(){
    return Peak/Factor;
  }

 long BaseSensor::TodayVal(){
    return Today/Factor;
  }

 long BaseSensor::TotalVal(){
  return Midnight + Today;
 }








long BaseSensor::aReadSD(int id) {

  //open the file for reading:
  char path[30];
  long content;
  sprintf(path, "/mnt/sda1/store/%0d.txt", id);

  File myFile = FileSystem.open(path, FILE_READ);
  Console.print("Read File from Basesensor: ");
  Console.println(path);
  
  if (myFile) {
    
    // read from the file until there's nothing else in it:
    myFile.read((byte*)&content, sizeof(long)); // read 4 bytes

      myFile.close();
    }
  else {
    // if the file didn't open, print an error:
    Console.println("error opening file (read in Sensor)");
  }
 Console.print("data: ");
 Console.println(content);

return(content);
}




long BaseSensor::aWriteSD(int id, long getal) {
 char path[30];
    sprintf(path, "/mnt/sda1/store/%0d.txt", id);
  Console.print("Write File from Basesensor: ");
  Console.println(path);

    File dataFile = FileSystem.open(path, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.write((byte*)&getal, sizeof(long)); // write 4 bytes
    dataFile.close();
    
  }
  // if the file isn't open, pop up an error:
  else {
    Console.println("error opening file (write in sensor)");
  }
}
