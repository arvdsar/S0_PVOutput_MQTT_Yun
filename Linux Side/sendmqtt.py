#!/usr/bin/python
import sys
import paho.mqtt.client as mqtt
client = mqtt.Client("S0yun")
client.username_pw_set(username="YOUR_MQTT_USERID",password="YOUR_MQTT_PASSWORD")
client.connect("YOUR MQTT SERVER IP ADDRESS LIKE: 192.168.0.200")

client.publish(sys.argv[1],sys.argv[2])
client.publish(sys.argv[3],sys.argv[4])
client.publish(sys.argv[5],sys.argv[6])
client.publish(sys.argv[7],sys.argv[8])