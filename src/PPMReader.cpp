
/*
  PPMReader.cpp
*/

#include "PPMReader.h"

PPMReader *PPMReader::ppm = nullptr;

void PPMReader::PPM_ISR(void) {
    ppm->handleInterrupt();
}

PPMReader::PPMReader(byte interruptPin, byte channelAmount, bool inverted)
    : interruptPin(interruptPin), channelAmount(channelAmount) {

    rawValues = new unsigned[channelAmount];
    for (int i = 0; i < channelAmount; ++i) {
        rawValues[i] = 0;
    }

    pinMode(interruptPin, INPUT);
    if (ppm == nullptr) {
        ppm = this;
        attachInterrupt(digitalPinToInterrupt(interruptPin), PPM_ISR, inverted ? FALLING : RISING);
    }
}

PPMReader::~PPMReader(void) {
    detachInterrupt(digitalPinToInterrupt(interruptPin));
    if (ppm == this) ppm = nullptr;
    delete[] rawValues;
}

void PPMReader::handleInterrupt(void) {
    unsigned long previousMicros = microsAtLastPulse;
    microsAtLastPulse = micros();
    unsigned long time = microsAtLastPulse - previousMicros;

    if (time > blankTime) {
        pulseCounter = 0;
    } else {
        if (pulseCounter < channelAmount) {
            rawValues[pulseCounter] = time;
            ++pulseCounter;
        }
    }
}

unsigned PPMReader::rawChannelValue(byte channel) {
    unsigned value = 0;
    if (channel >= 1 && channel <= channelAmount) {
        noInterrupts();
        value = rawValues[channel - 1];
        interrupts();
    }
    return value;
}

unsigned PPMReader::latestValidChannelValue(byte channel, unsigned defaultValue) {
    unsigned value = defaultValue;
    unsigned long timeout;
    noInterrupts();
    timeout = micros() - microsAtLastPulse;
    interrupts();

    if ((timeout < failsafeTimeout) && (channel >= 1) && (channel <= channelAmount)) {
        noInterrupts();
        value = rawValues[channel - 1];
        interrupts();

        if (value >= minChannelValue - channelValueMaxError && value <= maxChannelValue + channelValueMaxError) {
            value = constrain(value, minChannelValue, maxChannelValue);
        } else {
            value = defaultValue;
        }
    }
    return value;
}
