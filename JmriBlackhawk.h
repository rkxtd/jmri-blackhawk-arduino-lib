/*
  JmriBlackhawk.h - Library for JMRIBlackhawk project - library should help arduino to do some stupid commands like turn the servo, or highlight light alarm
  Copyright (c) 2017 Roman Shuvaryk.  All right reserved.
*/

// ensure this library description is only included once
#ifndef JmriBlackhawk_h
#define JmriBlackhawk_h
#include "PubSubClient.h"
#include "Servo.h"

// library interface description
class JmriBlackhawk
{
  // user-accessible "public" interface
  public:
    JmriBlackhawk(String inBoardsChannel, String inSensorsChannel, String inBoardName, PubSubClient inClient, String inDevices[]);
    String captureCommand(char* topic, char* payload, unsigned int length);
    void reconnect(void);
    void servoRight(Servo drive, int startPos = 160, int endPos = 20, int delayPos = 10);
    void servoLeft(Servo drive, int startPos = 20, int endPos = 160, int delayPos = 10);
    
  // library-accessible "private" interface
  private:
    String boardName;
    String sensorsChannel;
    String boardsChannel;
    PubSubClient client;
    String* devices;
    
    void registerBoard(void);
};

#endif

