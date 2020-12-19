
<!-- TOC -->

- [1. S0_PVOutput_MQTT for Arduino Yun Only](#1-s0_pvoutput_mqtt-for-arduino-yun-only)
- [2. Introduction](#2-introduction)
    - [2.1. High over functional description](#21-high-over-functional-description)
- [3. How To](#3-how-to)
    - [3.1. Prepare Arduino Yun](#31-prepare-arduino-yun)
    - [3.2. Setup additional software on the Arduino](#32-setup-additional-software-on-the-arduino)

<!-- /TOC -->

# 1. S0_PVOutput_MQTT for Arduino Yun Only #

Github Repository: https://github.com/arvdsar/S0_PVOutput_MQTT_Yun

Website: https://www.vdsar.net/s0-counter-with-arduino-yun/

S0 Counter which counts pulses from an energy meter (or other pulse sensors like water meters) and upload the details to pvoutput.org and additionally send the values via MQTT topics for use in your domotica setup. (i.e. Domoticz or Node-Red).

The code is partially based on Solarmeter of Harold65 and S0 Uploader of [Dylantje](https://github.com/Dylantje/PVOutput-S0-Uploader) but completely updated to work on the Arduino Yun only! The Arduino Yun has both a 32u4 microcontroller and a Linux processor. The microcontroller is doing the counting stuff, the linux side is handling sending data to pvoutput.org, MQTT and providing an API to set some initial numbers via a webbrowser. 
 
This version does not contain any smart meter (Slimme Meter) data. You could use DSMRReader on a Raspberry Pi to get the data from your smart meter to Node-Red and/or Pvoutput.org
  
You have to use a microSD card with the Arduino Yun. This saves your EEPROM from being ruined and this sketch depends on the SDCard. Some data variables are stored on the SD Card for use by this sketch only (you can't read it yourself).

# 2. Introduction #
Dutch smart meters  only deliver electricity stats on imported and exported electricity from and to the grid. PV inverters sometimes have an API or RS-232 or other interface to deliver information on generated energy.

Years ago I had an Arduino Mega running with a modified version of an early version of Solarmeter by Harold65. I kept extending the code to provide more information and integrate better in my domotica setup. This has been running for some years until we moved to a new home. The older code can still be found [here](https://www.vdsar.net/diy-energy-meter/).

Recently we’ve installed solar panels on our home and although the inverter is delivered including an app, it is more fun to build something yourself. There is also no API available to integrate with my domotica system. The original Solarmeter software is not updated anymore and my older version is not compatible with recent smart meters in the Netherlands. The Arduino Mega runs into its limits and cannot keep up with the data transfer of the new smart meters.

In my new setup, data of our smart meter is read by [DSMRReader](https://github.com/dsmrreader/dsmr-reader) which runs on a Raspberry Pi. DSMRReader can send the data using MQTT. Additionally this data is also uploaded to pvoutput.org.

I was planning to use the Arduino Mega in a stripped version for only reading the S0 pulses from a kWh meter which is connected to the solar panel installation.

On the day the solar panels were installed, it turned out that we could re-use the cables to the washing machine. This saved us from drilling holes. The ‘downside’ was that the kWh meter is now installed in the addict and I did not foresee a wired network connection near the washing machine ;-). I have to use WiFi now.

## 2.1. High over functional description ##
I had an Arduino Yun laying around for years, but never used it. This was the moment to start tinkering. The Arduino Yun is a board with a 32u4 microcontroller and a Linux processor and native Ethernet and WiFi onboard. The microcontroller is used to count the pulses of the S0 kWh meter and generates the Actual, Peak, Today generated and Total generated numbers. This is what the microcontroller is good at. The microcontroller can trigger (Python) scripts on the Linux part and send data to those scripts. The Linux processor will handle sending this data to MQTT, PvOutput and also provide a simple API to set initial values for ‘Total generated’. The Arduino Yun is equipped with a SDCard reader. In the original software the Today generated and Total generated numbers were stored in EEPROM. In the new version this is stored in seperate files on the SDCard. 

# 3. How To

## 3.1. Prepare Arduino Yun ##
Follow the setup steps described on the Arduino website:

Setup Yun, start reading: https://www.arduino.cc/en/Guide/ArduinoYun

Setup SDCard: https://www.arduino.cc/en/Tutorial/ExpandingYunDiskSpace

Download my source code from GitHub: https://github.com/arvdsar/S0_PVOutput_MQTT_Yun

Update the **userdefs.h** with your own information (like PVOutput API key).  Upload the sketch to your Arduino Yun.
In the repository you find a folder: Linux Side. You have to copy those files to the SD card on the Yun. Instead of copying the files, I just copy and paste the contents in new files on the Arduino. You can update them right away with your details.

Connect to your Arduino Yun using SSH:
``` bash
# ssh root@IP_of_arduino
```
Enter your password (same as in the Yun web interface)

Go to the correct location on SD card:
``` bash
# cd /mnt/sda1
```

To make sure that the nano editor works:
``` bash
# TERM=xterm-color
```

Edit sendmqtt.py file:
``` bash
# nano sendmqtt.py
```

Copy and paste the contents of the file from the repository and meanwhile update with your information (IP address and credentials of your MQTT server.)

Do the same for pvoutput.py and localtime.py.
Next, change file permissions of the files:

``` bash
# chmod 755 sendmqtt.py 
# chmod 755 pvoutput.py
# chmod 755 localtime.py
```

Create a folder for the files that contain the total and today values from the script:
``` bash
# mkdir store 
```

## 3.2. Setup additional software on the Arduino ##
I got some of the step below from this site:  https://forum.arduino.cc/index.php?topic=544274.0;wap2

Install Mosquitto on Linux (on the Yun)
``` bash
# opkg update
# opkg install mosquitto mosquitto-client libmosquitto 
# wget https://pypi.python.org/packages/source/p/pip/pip-6.0.8.tar.gz#md5=2332e6f97e75ded3bddde0ced01dbda3\  –no-check-certificate
# easy_install pip-6.0.8.tar.gz
# pip install paho-mqtt
# pip install requests
```

Now you should be ready to run. Make sure the time is set properly on your Arduino. You can do that using the web interface of the Yun. The time is taken from the linux side of the Yun and synced to the Arduino side.

On PVOutput.org you officially have to deliver the generation values first (solar energy) and the NET values (from the smartmeter) a bit later. In that case PVOutput will do the proper calculations. 

The values from DSMRreader and the Arduino are collected in [Node-Red](https://nodered.org) where I can forward the information to [Domoticz](https://www.domoticz.com), [Grafana](https://grafana.com), [Blynk](https://blynk.io/en/getting-started) and [Complicated](https://github.com/arvdsar/node-red-contrib-complicated)

