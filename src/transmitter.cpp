#ifndef RECEIVER

#include <Arduino.h>
#include <CC1101Radio.h>

Radio *radio;

void setup()
{
    Serial.begin(38400);
    Serial.println(F("transmitter start"));

    radio = new CC1101Radio();
}

unsigned long sentTime;

void responseHandler(Message *message) {
    unsigned long recvTime = micros();
    Serial.println(F("Received response!"));
    Serial.println(message->getBody());
    Serial.print(F("Roundtrip delay: "));
    Serial.print((recvTime - sentTime) / 1000);
    Serial.println(F("ms"));
}

void waitForResponse(Message *message) {
    Serial.println(F("waiting for response..."));
    radio->listen(responseHandler);
}

void sendCommand(String command) {
    sentTime = micros();
    Message msg(command);
    msg.then(waitForResponse);
    if (!radio->send(&msg)) {
        Serial.println(F("message failed to send"));
        return;
    }
    Serial.print(F("sent "));
    Serial.print(command);
    Serial.println(F(" command"));
}

void loop()
{
    radio->tick();
    if (Serial.available()) {
        char cmd = (char) Serial.read();
        if (cmd == 'T') {
            sendCommand(F("getstatus"));
        } else if (cmd == 'P') {
            sendCommand(F("play"));
        }
    }
}

#endif