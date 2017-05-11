/*
 JmriBlackhawk.cpp - Library for JMRIBlackhawk project - library should help arduino to do some stupid commands like turn the servo, or highlight light alarm
 Copyright (c) 2017 Roman Shuvaryk.  All right reserved.
 */

// include this library's description file
#include "JmriBlackhawk.h"

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

#include "PubSubClient.h"

#include "Servo.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

JmriBlackhawk::JmriBlackhawk(String inBoardsChannel, String inSensorsChannel, String inBoardName, PubSubClient inClient, String inDevices[])
{
  // initialize this instance's variables
  String sensorsChannel = inSensorsChannel;
  String boardsChannel = inBoardsChannel;
  String boardName = inBoardName;
  PubSubClient client = inClient;
  String* devices = inDevices;
  Serial.begin(9600);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

String JmriBlackhawk::captureCommand(char* topic, char* payload, unsigned int length) {
    char boardsChannelChr[7];
    boardsChannel.toCharArray(boardsChannelChr, 7);
    
    String command = String (payload);
    String pongCommand = "PONG::" + boardName;
    char pongCommandChr[16];
    pongCommand.toCharArray(pongCommandChr, 16);
    command = command.substring(0, length);
    
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.print(command);
    Serial.println();
    
    if (String(topic).equals(boardsChannel)) {
        if (command.equals("PING")) {
            client.publish(boardsChannelChr, pongCommandChr);
        }
        
        if (command.equals("REGISTER")) {
            registerBoard();
        }
    }
    
    return command;
}

void JmriBlackhawk::reconnect() {
    char boardsChannelChr[7];
    char sensorsChannelChr[15];
    boardsChannel.toCharArray(boardsChannelChr, 7);
    sensorsChannel.toCharArray(sensorsChannelChr, 15);
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("arduinoClient")) {
            Serial.println("connected");
            client.subscribe(boardsChannelChr);
            client.subscribe(sensorsChannelChr);
            registerBoard();
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void JmriBlackhawk::servoRight(Servo drive, int startPos = 160, int endPos = 20, int delayPos = 10) {
    for (int pos = startPos; pos >= endPos; pos -= 3) {
        drive.write(pos);
        delay(delayPos);
    }
    Serial.println("Turn RIGHT");
}

void JmriBlackhawk::servoLeft(Servo drive, int startPos = 20, int endPos = 160, int delayPos = 10) {
    for (int pos = startPos; pos <= endPos; pos += 3) {
        drive.write(pos);
        delay(delayPos);
    }
    Serial.println("Turn LEFT");
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void JmriBlackhawk::registerBoard() {
    char registerCommandChr[90];
    char boardsChannelChr[7];
    char sensorsChannelChr[15];
    String registerCommand = "REGISTER::" + boardName;
    
    boardsChannel.toCharArray(boardsChannelChr, 7);
    sensorsChannel.toCharArray(sensorsChannelChr, 15);

    for(int i = 0; i < sizeof(devices) - 1; i++) {
        registerCommand += "::" + devices[i];
    }
    
    registerCommand.toCharArray(registerCommandChr, 90);
    client.publish(boardsChannelChr, registerCommandChr);
    Serial.println("SEND: " + registerCommand);
}
