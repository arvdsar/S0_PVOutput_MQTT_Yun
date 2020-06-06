# S0_PVOutput_MQTT for Arduino Yun Only

S0 Counter which can count pulses from an energy meter (or other sensors like water meters) and upload the details to pvoutput.org and additionally send the values via MQTT topics for use in you domotica setup. (i.e. Domoticz or Node-Red).

The code is partially based on Solarmeter of Harold65 and S0 Uploader of Dylantje but completely adjusted to work on the Arduino Yun only! The Arduino Yun has both a 32u4 microcontroller and a Linux processor. The microcontroller is doing the counting stuff, the linux side is handling sending data to pvoutput.org, MQTT and providing an API to set some initial numbers via a webbrowser. 
 
This version does not contain any smart meter (Slimme Meter) data. You could use DSMRReader on a Raspberry Pi to get the data from your smart meter to Node-Red and/or Pvoutput.org
  
You have to use an microSD card with the Arduino Yun. This saves your EEPROM from being ruined and the sketch depends on the SDCard. Some data variables are stored on the SD Card for use by this sketch only (you can't read it yourself).
  
The main reason why I moved away from the Arduino Mega to the Arduino Yun is the fact that I have to use WiFi which is not by default available on the Arduino Mega. Besides I had the Arduino Yun available. 

copy the files from the 'linux side' folder onto the SD Card of the Yun (/mnt/sda1/)
Also create a folder (/mnt/sda1/store) on the SDCard

Additional information and instructions will follow. The system is now moved from just testing into 'production' so hopefully it remains stable



