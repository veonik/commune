#ifndef TRANSMITTER

#include <Arduino.h>
#include <CC1101Radio.h>
#include <Adafruit_VS1053.h>

#define SHIELD_RESET  -1     // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

#define CARDCS 4     // Card chip select pin
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
Radio *radio;

void play() {
    Serial.println(F("Playing track 001"));
    musicPlayer.startPlayingFile("01.mp3");
}

void sendStatus()
{
    String body;
    if (musicPlayer.playingMusic) {
        body += "Now playing: ";
        body += musicPlayer.currentTrack.name();
    } else {
        body += "Music not playing";
    }
    Message reply(body);
    if (!radio->send(reply)) {
        Serial.println(F("failed to send status"));
    } else {
        Serial.println(F("status sent"));
    }
}

void msghandler(Message message) {
    Serial.println(F("handling message. Contents:"));
    String messageBody = message.getBody();
    Serial.print(messageBody);
    Serial.println();
    if (messageBody.startsWith(F("getstatus"))) {
        sendStatus();
    } else if (messageBody.startsWith(F("play"))) {
        play();
    }
}

void setup()
{
    Serial.begin(38400);
    Serial.println(F("receiver start"));
    if (!musicPlayer.begin()) {
        Serial.println(F("Couldn't initialize VS1053"));
        while (1);
    }
    Serial.println(F("VS1053 found"));

    SD.begin(CARDCS);

    musicPlayer.setVolume(20,20);
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

    radio = new CC1101Radio();
    radio->listen(msghandler);
}

void loop()
{
    radio->tick();
}

#endif